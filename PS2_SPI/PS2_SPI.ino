#include <Arduino.h>
#include <stdint.h>
#include "PS2_SPI.h"

static volatile uint8_t byte_index = 0;
static volatile uint8_t data_read = 0;
static volatile uint8_t rx_index = 0;
static volatile uint8_t rx[16];

void init_ps2_spi() {
  Serial.begin(9600);
  pinMode(SCK_PIN, INPUT);
  pinMode(MISO, OUTPUT);
  pinMode(MOSI, INPUT);
  pinMode(SS_PIN, INPUT);
  pinMode(ACK_PIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(SS_PIN), end_read, RISING);
  attachInterrupt(digitalPinToInterrupt(SCK_PIN), read_bytes, RISING);
}

void setup() {
  init_ps2_spi();
  for (int i = 0; i < 16; i++)
    rx[i] = 0;
}

void loop() {
  if (data_read) {
    for (int i = 0; i < 16; i++)
      Serial.printf("%02x ", rx[i]);
    Serial.print("\r\n");
    data_read = 0;
    byte_index = 0;
    rx_index = 0;
    for (int i = 0; i < 16; i++)
      rx[i] = 0;
  }
}

void end_read() {
  data_read = 1;
}

void read_bytes() {
  uint8_t val = digitalRead(MOSI);
  rx[rx_index] |= (val << byte_index);
  if (++byte_index >= 8) {
    byte_index = 0;
    rx_index++;
  }
}