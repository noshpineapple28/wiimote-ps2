#include <Arduino.h>
#include <SPI.h>
#include "stdio.h"
#include "stdint.h"


#define MAX_COMMAND 32
#define ACK_PIN 9

volatile uint8_t digital_mode = 1;

// posible commands
// pressure sensor
volatile uint8_t config40[MAX_COMMAND] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
volatile uint8_t max_digital40 = 6;
volatile uint8_t max_analog40 = 6;
// check response data
volatile uint8_t config41[MAX_COMMAND] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
volatile uint8_t max_digital41 = 6;
volatile uint8_t max_analog41 = 6;
// poll
volatile uint8_t config42[MAX_COMMAND] = { 0xff ^ (1 << 6), 0xff ^ (1 << 6), 0xf7, 0xf7, 0xf7, 0xf7 };
volatile uint8_t max_digital42 = 2;
volatile uint8_t max_analog42 = 4;
// escape hatch
volatile uint8_t config430[MAX_COMMAND] = { 0xff, 0xff, 0x00, 0x00, 0x00, 0x00 };
volatile uint8_t config431[MAX_COMMAND] = { 0xff, 0xff, 0x00, 0x00, 0x00, 0x00 };
volatile uint8_t max_digital43 = 2;
volatile uint8_t max_analog43 = 4;

volatile uint8_t config44[MAX_COMMAND] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
volatile uint8_t max_digital44 = 6;
volatile uint8_t max_analog44 = 6;
// query model
volatile uint8_t config45[MAX_COMMAND] = { 0x02, 0x02, 0x00, 0x02, 0x00, 0x00 };
volatile uint8_t max_digital45 = 6;
volatile uint8_t max_analog45 = 6;
// random query stuff
volatile uint8_t config460[MAX_COMMAND] = { 0x00, 0x00, 0x00, 0x02, 0x00, 0x0A };
volatile uint8_t config461[MAX_COMMAND] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x14 };
volatile uint8_t max_digital46 = 6;
volatile uint8_t max_analog46 = 6;
// random query stuff
volatile uint8_t config47[MAX_COMMAND] = { 0x00, 0x00, 0x02, 0x00, 0x00, 0x00 };
volatile uint8_t max_digital47 = 6;
volatile uint8_t max_analog47 = 6;

volatile uint8_t config48[MAX_COMMAND] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
volatile uint8_t config49[MAX_COMMAND] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
volatile uint8_t config4a[MAX_COMMAND] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
volatile uint8_t config4b[MAX_COMMAND] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
// random query stuff
volatile uint8_t config4c0[MAX_COMMAND] = { 0x00, 0x00, 0x00, 0x04, 0x00, 0x00 };
volatile uint8_t config4c1[MAX_COMMAND] = { 0x00, 0x00, 0x00, 0x06, 0x00, 0x00 };
volatile uint8_t max_digital4c = 6;
volatile uint8_t max_analog4c = 6;
// set up motors??
volatile uint8_t config4d[MAX_COMMAND] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
volatile uint8_t max_digital4d = 6;
volatile uint8_t max_analog4d = 6;
volatile uint8_t config4e[MAX_COMMAND] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
volatile uint8_t config4f[MAX_COMMAND] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
volatile uint8_t max_digital4f = 6;
volatile uint8_t max_analog4f = 6;

volatile uint8_t header[5] = { 0xff, 0x41, 0x5a, 0xff, 0xff };

volatile uint8_t rx[MAX_COMMAND];
volatile uint8_t *tx = config42;
volatile uint8_t rx_pos = 0;
volatile uint8_t tx_pos = 0;
volatile uint8_t header_pos = 0;
volatile uint8_t process_it = 0;
volatile uint8_t max_transfer_size = 0;


char buf[7];

void init_ps2_spi() {
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

  pinMode(MISO, OUTPUT);  // sets the digital pin 12 as output
  pinMode(MOSI, INPUT);   // sets the digital pin 12 as output
  pinMode(SS, INPUT);     // sets the digital pin 12 as output
  pinMode(SCK, INPUT);    // sets the digital pin 12 as output
  pinMode(ACK_PIN, OUTPUT);    // sets the digital pin 12 as output

  // ENABLE SPI
  SPCR |= (1 << SPE);
  Serial.print("Completed setup");
}

void setup() {
  Serial.begin(115200);
  delay(2500);
  // put your setup code here, to run once:
  init_ps2_spi();
}

void set_next_command() {
  static uint8_t been_here_46 = 0;
  static uint8_t been_here_4c = 0;

  switch (rx[1]) {
    case 0x41:
      tx = config41;
      max_transfer_size = max_digital41;
      break;
    case 0x42:
      tx = config42;
      max_transfer_size = max_digital42;
      break;
    case 0x43:
      if (header[1] == 0x41) {
        tx = config431;
        header[1] = 0xf3;
      } else {
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
      if (!been_here_46) {
        tx = config460;
        been_here_46 = 1;
      } else {
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
      if (!been_here_4c) {
        tx = config4c0;
        been_here_4c = 1;
      } else {
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

void loop() {
  // put your main code here, to run repeatedly:
  while (!process_it) {
  }

  for (int i = 0; i < rx_pos; i++) {
    sprintf(buf, "%02x/%02x ", rx[i], i < 3 ? header[i] : tx[i - 3]);
    Serial.print(buf);
    rx[i] = 0;
  }
  Serial.print("\r\n");


  // prepare to send da next thing
  rx_pos = 0;
  tx_pos = 0;
  header_pos = 0;
  SPDR = header[header_pos++];
  process_it = 0;
}

// SPI interrupt routine
ISR(SPI_STC_vect) {
  // add to buffer if room
  rx[rx_pos++] = SPDR;  // grab byte from SPI Data Register
  // SPDR = 0xff;

  if (rx_pos == 2) {
    // set_next_command();
    static uint8_t been_here_46 = 0;
    static uint8_t been_here_4c = 0;

    switch (rx[1]) {
      case 0x41:
        tx = config41;
        max_transfer_size = max_digital41;
        break;
      case 0x42:
        tx = config42;
        max_transfer_size = max_digital42;
        break;
      case 0x43:
        if (header[1] == 0x41) {
          tx = config431;
          header[1] = 0xf3;
        } else {
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
        if (!been_here_46) {
          tx = config460;
          been_here_46 = 1;
        } else {
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
        if (!been_here_4c) {
          tx = config4c0;
          been_here_4c = 1;
        } else {
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

  if (header_pos < 3)
    SPDR = header[header_pos++];
  else {
    SPDR = tx[tx_pos++];
  }

  digitalWrite(ACK_PIN, LOW);
  delayMicroseconds(10);
  digitalWrite(ACK_PIN, HIGH);

  // example: newline means time to process buffer
  if (digitalRead(SS) == HIGH)
    process_it = true;
}  // end of interrupt routine SPI_STC_vect
