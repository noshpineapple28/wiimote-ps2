#ifndef __PS2_SPI_H_
#define __PS2_SPI_H_

#include <SPI.h>
#include "main.h"

#define MAX_COMMAND 32
#define ACK_PIN 9

// posible commands
// pressure sensor
volatile static uint8_t config40[MAX_COMMAND] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
volatile static uint8_t max_digital40 = 6;
volatile static uint8_t max_analog40 = 6;
// check response data
volatile static uint8_t config41[MAX_COMMAND] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
volatile static uint8_t max_digital41 = 6;
volatile static uint8_t max_analog41 = 6;
// poll
volatile static uint8_t config42[MAX_COMMAND] = { 0xff ^ (1 << 6), 0xff ^ (1 << 6), 0xf7, 0xf7, 0xf7, 0xf7 };
volatile static uint8_t max_digital42 = 2;
volatile static uint8_t max_analog42 = 4;
// escape hatch
volatile static uint8_t config430[MAX_COMMAND] = { 0xff, 0xff, 0x00, 0x00, 0x00, 0x00 };
volatile static uint8_t config431[MAX_COMMAND] = { 0xff, 0xff, 0x00, 0x00, 0x00, 0x00 };
volatile static uint8_t max_digital43 = 2;
volatile static uint8_t max_analog43 = 4;

volatile static uint8_t config44[MAX_COMMAND] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
volatile static uint8_t max_digital44 = 6;
volatile static uint8_t max_analog44 = 6;
// query model
volatile static uint8_t config45[MAX_COMMAND] = { 0x02, 0x02, 0x00, 0x02, 0x00, 0x00 };
volatile static uint8_t max_digital45 = 6;
volatile static uint8_t max_analog45 = 6;
// random query stuff
volatile static uint8_t config460[MAX_COMMAND] = { 0x00, 0x00, 0x00, 0x02, 0x00, 0x0A };
volatile static uint8_t config461[MAX_COMMAND] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x14 };
volatile static uint8_t max_digital46 = 6;
volatile static uint8_t max_analog46 = 6;
// random query stuff
volatile static uint8_t config47[MAX_COMMAND] = { 0x00, 0x00, 0x02, 0x00, 0x00, 0x00 };
volatile static uint8_t max_digital47 = 6;
volatile static uint8_t max_analog47 = 6;

volatile static uint8_t config48[MAX_COMMAND] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
volatile static uint8_t config49[MAX_COMMAND] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
volatile static uint8_t config4a[MAX_COMMAND] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
volatile static uint8_t config4b[MAX_COMMAND] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
// random query stuff
volatile static uint8_t config4c0[MAX_COMMAND] = { 0x00, 0x00, 0x00, 0x04, 0x00, 0x00 };
volatile static uint8_t config4c1[MAX_COMMAND] = { 0x00, 0x00, 0x00, 0x06, 0x00, 0x00 };
volatile static uint8_t max_digital4c = 6;
volatile static uint8_t max_analog4c = 6;
// set up motors??
volatile static uint8_t config4d[MAX_COMMAND] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
volatile static uint8_t max_digital4d = 6;
volatile static uint8_t max_analog4d = 6;
volatile static uint8_t config4e[MAX_COMMAND] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
volatile static uint8_t config4f[MAX_COMMAND] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
volatile static uint8_t max_digital4f = 6;
volatile static uint8_t max_analog4f = 6;

/**
 * @brief initializes SPI peripheral for usage on the PS2
 */
void init_ps2_spi(void);

/**
 * @brief waits for SPI transfer to end and processes received data from it. BLOCKING
 */
void handle_spi(void);

#endif
