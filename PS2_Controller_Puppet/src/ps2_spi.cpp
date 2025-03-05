#include "ps2_spi.h"
#include "adapter_i2c.h"

volatile uint8_t digital_mode = 1;
// this will hold the bits related to the custom map!
volatile uint32_t custom_map_set = 0;
volatile uint8_t process_it = 0;

// controller state
static controller *cntrl;

// buffers
char buf[4];
volatile uint8_t rx[MAX_COMMAND];
volatile uint8_t *tx = (uint8_t *)cntrl;
volatile uint8_t header[3] = {0xff, 0x41, 0x5a};
// positions
volatile uint8_t rx_pos = 0;
volatile uint8_t tx_pos = 0;
volatile uint8_t header_pos = 0;

// size of the transfer to be done
volatile uint8_t max_transfer_size = 0;

void init_ps2_spi()
{
    Serial.print("Initializing SPI\r\n");
    // SPI_MODE 3
    SPCR = 0;

    // SET PERIPHERAL MODE
    SPCR &= ~(1 << MSTR);

    // SET TO RECEIVE DATA LSB
    SPCR |= (1 << DORD);

    // CLOCK IDLES HIGH
    SPCR |= (1 << CPOL);

    // CLOCK READS ON HIGH
    SPCR &= ~(1 << CPHA);

    // enable interrupt
    SPCR |= (1 << SPIE);

    SPDR = header[header_pos++];

    pinMode(MISO, OUTPUT);    // sets the digital pin 12 as output
    pinMode(MOSI, INPUT);     // sets the digital pin 12 as output
    pinMode(SS, INPUT);       // sets the digital pin 12 as output
    pinMode(SCK, INPUT);      // sets the digital pin 12 as output
    pinMode(ACK_PIN, OUTPUT); // sets the digital pin 12 as output

    // get reference to the controller state
    cntrl = get_controller();
    // ENABLE SPI
    SPCR |= (1 << SPE);
    Serial.print("SPI initialized\r\n");
}

void handle_spi(void)
{
    // loops until data is done being sent
    while (!process_it)
        ;

    for (int i = 0; i < rx_pos; i++)
    {
        sprintf(buf, "%02x/%02x ", rx[i], i < 3 ? header[i] : (i < 9) ? tx[i - 3] : 0xff);
        Serial.print(buf);
        rx[i] = 0;
    }
    Serial.print("\r\n");

    // reset buffer states
    rx_pos = 0;
    tx_pos = 0;
    header_pos = 0;
    SPDR = header[header_pos++];
    process_it = 0;
}

void set_header_dependent_on_analog_mode()
{
    if (digital_mode)
    {
        header[1] = 0x41;
    }
    else
    {
        if (custom_map_set)
        {
            header[1] = 0x79;
        }
        else
        {
            header[1] = 0x73;
        }
    }
}

/**
 * @brief changes the data being sent based on the received headers
 */
void set_next_command()
{
    static uint8_t been_here_46 = 0;
    static uint8_t been_here_4c = 0;

    switch (rx[1])
    {
    case 0x41:
        tx = config41;
        header[1] = 0xf3;
        break;
    case 0x42:
        set_header_dependent_on_analog_mode();
        tx = (uint8_t *)cntrl;
        break;
    case 0x43:
        // enter escape mode
        if (header[1] == 0x41)
        {
            tx = (uint8_t *)cntrl;
            header[1] = 0xf3;
        }
        // exit escape mode
        else
        {
            set_header_dependent_on_analog_mode();
            tx = (uint8_t *)cntrl;
        }
        break;
    case 0x44:
        header[1] = 0xf3;
        digital_mode = 0;
        tx = config44;
        break;
    case 0x45:
        header[1] = 0xf3;
        tx = config45;
        // this bit represent whether the analog LED is on
        if (digital_mode)
            tx[2] = 0x00;
        else
            tx[2] = 0x01;
        break;
    case 0x46:
        header[1] = 0xf3;
        if (!been_here_46)
        {
            tx = config460;
            been_here_46 = 1;
        }
        else
        {
            been_here_46 = 0;
            tx = config461;
        }
        break;
    case 0x47:
        header[1] = 0xf3;
        tx = config47;
        break;
    case 0x4C:
        header[1] = 0xf3;
        if (!been_here_4c)
        {
            tx = config4c0;
            been_here_4c = 1;
        }
        else
        {
            tx = config4c1;
            been_here_4c = 0;
        }
        break;
    case 0x4d:
        header[1] = 0xf3;
        // TODO: thos response should follow the data sent
        tx = config4d;
        break;
    default:
        set_header_dependent_on_analog_mode();
        tx = (uint8_t *)cntrl;
        break;
    }
}

/**
 * @brief ISR for SPI. Handles data process and prepares next byte of data to send
 */
ISR(SPI_STC_vect)
{
    // add to buffer if room
    rx[rx_pos++] = SPDR; // grab byte from SPI Data Register
    // SPDR = 0xff;

    // ONLY do this for pos 2
    if (rx_pos == 2)
    {
        set_next_command();
    }
    // does this for all header positions
    if (header_pos < 3)
    {
        SPDR = header[header_pos++];
    }
    else if (tx_pos >= 6)
    {
        SPDR = 0xFF;
    }
    else
    {
        SPDR = tx[tx_pos++];
    }

    // alert the PS2 that we've processed all data
    digitalWrite(ACK_PIN, LOW);
    delayMicroseconds(10);
    digitalWrite(ACK_PIN, HIGH);

    // if this line is high, data transmition has ended
    if (digitalRead(SS) == HIGH)
        process_it = true;
}
