#include <stdint.h>

#include "main.h"
#include "spi.h"
#include "uart.h"
#include "systick.h"

volatile uint8_t header[5] = {0xff, 0x41, 0x5a, 0xff, 0xff};

volatile uint8_t digital_mode = 1;

// posible commands
// pressure sensor
volatile uint8_t config40[MAX_COMMAND] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
volatile uint8_t max_digital40 = 6;
volatile uint8_t max_analog40 = 6;
// check response data
volatile uint8_t config41[MAX_COMMAND] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
volatile uint8_t max_digital41 = 6;
volatile uint8_t max_analog41 = 6;
// poll
volatile uint8_t config42[MAX_COMMAND] = {0xff ^ (1 << 6), 0xff ^ (1 << 6), 0xf7, 0xf7, 0xf7, 0xf7};
volatile uint8_t max_digital42 = 2;
volatile uint8_t max_analog42 = 4;
// escape hatch
volatile uint8_t config430[MAX_COMMAND] = {0xff, 0xff, 0x00, 0x00, 0x00, 0x00};
volatile uint8_t config431[MAX_COMMAND] = {0xff, 0xff, 0x00, 0x00, 0x00, 0x00};
volatile uint8_t max_digital43 = 2;
volatile uint8_t max_analog43 = 4;

volatile uint8_t config44[MAX_COMMAND] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
volatile uint8_t max_digital44 = 6;
volatile uint8_t max_analog44 = 6;
// query model
volatile uint8_t config45[MAX_COMMAND] = {0x02, 0x02, 0x00, 0x02, 0x00, 0x00};
volatile uint8_t max_digital45 = 6;
volatile uint8_t max_analog45 = 6;
// random query stuff
volatile uint8_t config460[MAX_COMMAND] = {0x00, 0x00, 0x00, 0x02, 0x00, 0x0A};
volatile uint8_t config461[MAX_COMMAND] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x14};
volatile uint8_t max_digital46 = 6;
volatile uint8_t max_analog46 = 6;
// random query stuff
volatile uint8_t config47[MAX_COMMAND] = {0x00, 0x00, 0x02, 0x00, 0x00, 0x00};
volatile uint8_t max_digital47 = 6;
volatile uint8_t max_analog47 = 6;

volatile uint8_t config48[MAX_COMMAND] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
volatile uint8_t config49[MAX_COMMAND] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
volatile uint8_t config4a[MAX_COMMAND] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
volatile uint8_t config4b[MAX_COMMAND] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// random query stuff
volatile uint8_t config4c0[MAX_COMMAND] = {0x00, 0x00, 0x00, 0x04, 0x00, 0x00};
volatile uint8_t config4c1[MAX_COMMAND] = {0x00, 0x00, 0x00, 0x06, 0x00, 0x00};
volatile uint8_t max_digital4c = 6;
volatile uint8_t max_analog4c = 6;
// set up motors??
volatile uint8_t config4d[MAX_COMMAND] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
volatile uint8_t max_digital4d = 6;
volatile uint8_t max_analog4d = 6;
volatile uint8_t config4e[MAX_COMMAND] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
volatile uint8_t config4f[MAX_COMMAND] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
volatile uint8_t max_digital4f = 6;
volatile uint8_t max_analog4f = 6;

volatile uint8_t rx[MAX_COMMAND];
volatile uint8_t *tx = config42;
volatile uint8_t rx_pos = 0;
volatile uint8_t tx_pos = 0;
volatile uint8_t header_pos = 0;
volatile uint8_t process_it = 0;
volatile uint8_t max_transfer_size = 0;

void set_next_command()
{
    static uint8_t been_here_46 = 0;
    static uint8_t been_here_4c = 0;

    switch (rx[1])
    {
    case 0x41:
        tx = config41;
        max_transfer_size = max_digital41;
        break;
    case 0x42:
        tx = config42;
        max_transfer_size = max_digital42;
        break;
    case 0x43:
        if (header[1] == 0x41)
        {
            tx = config431;
            header[1] = 0xf3;
        }
        else
        {
            header[1] = 0x41;
            tx = config430;
        }
        max_transfer_size = max_digital43;
        break;
    case 0x44:
        tx = config44;
        header[1] = 0x71;
        max_transfer_size = max_digital44;
        break;
    case 0x45:
        tx = config45;
        max_transfer_size = max_digital45;
        break;
    case 0x46:
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
        max_transfer_size = max_digital46;
        break;
    case 0x47:
        tx = config47;
        max_transfer_size = max_digital47;
        break;
    case 0x4C:
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
        max_transfer_size = max_digital4c;
        break;
    case 0x4d:
        tx = config4d;
        max_transfer_size = max_digital4d;
        break;
    default:
        tx = config42;
        max_transfer_size = max_digital42;
        break;
    }
}

void SPI1_IRQHandler(void)
{
    // add to buffer if room
    rx[rx_pos++] = SPI1->DR; // grab byte from SPI Data Register
    // SPDR = 0xff;

    if (rx_pos == 2)
    {
        set_next_command();
    }

    if (header_pos < 3)
        SPI1->DR = header[header_pos++];
    else
    {
        SPI1->DR = tx[tx_pos++];
    }

    GPIOA->ODR |= (GPIO_ODR_OD1);
    wait_us(10);
    GPIOA->ODR &= ~(GPIO_ODR_OD1);

    // example: newline means time to process buffer
    if (~GPIOA->IDR & GPIO_IDR_ID4_Msk)
        process_it = 1;
}

void reset_spi(void)
{
    // prepare to send da next thing
    rx_pos = 0;
    tx_pos = 0;
    header_pos = 0;
    SPI1->DR = header[header_pos++];
    process_it = 0;
}

uint8_t check_if_done(void)
{
    return process_it;
}

uint8_t get_transfer_size(void)
{
    return rx_pos;
}

uint8_t *get_header(void)
{
    return header;
}

uint8_t *get_RX(void)
{
    return rx;
}

uint8_t *get_TX(void)
{
    return tx;
}

void SPI_Init(void)
{
    // ********** CR1 REGISTER ********** //
    // reset register
    SPI1->CR1 &= 0x00;

    // configure CPOL and CPHA to SPI MODE 3
    SPI1->CR1 |= (SPI_CR1_CPOL);

    // configure to LSBFIRST
    SPI1->CR1 |= (SPI_CR1_LSBFIRST);

    // ********** CR2 REGISTER ********** //
    // reset register
    SPI1->CR2 &= 0x0700;

    // set data transfer length (8 bits)
    SPI1->CR2 |= (0x07 << SPI_CR2_DS_Pos);

    // FRXTH bit, RXFIFO aligns with size access size of SPI1_DR (RXNEIE for RXFIFO interrupt)
    SPI1->CR2 |= SPI_CR2_FRXTH;

    // enable interrupt on RXNE event
    SPI1->CR2 |= SPI_CR2_RXNEIE;
    NVIC_SetPriority(SPI1_IRQn, 0);
    NVIC_EnableIRQ(SPI1_IRQn);

    // ********** SPI ENABLE ********** //
    // set DR
    SPI1->DR = header[header_pos++];
    // start SPI
    SPI1->CR1 |= SPI_CR1_SPE;
}
