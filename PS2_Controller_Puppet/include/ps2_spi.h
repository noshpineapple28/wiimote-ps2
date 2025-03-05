#ifndef __PS2_SPI_H_
#define __PS2_SPI_H_

#include <SPI.h>
#include "main.h"

#define MAX_COMMAND 32
#define ACK_PIN 9

// posible commands
// pressure sensor
volatile static uint8_t config40[MAX_COMMAND] = { 0x00, 0x00, 0x02, 0x00, 0x00, 0x5a };
// check response data
volatile static uint8_t config41[MAX_COMMAND] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
// 0x43 = escape hatch

// toggle lock on analog mode
volatile static uint8_t config44[MAX_COMMAND] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

// query model
volatile static uint8_t config45[MAX_COMMAND] = { 0x03, 0x02, 0x00, 0x02, 0x00, 0x00 };
// random query stuff
volatile static uint8_t config460[MAX_COMMAND] = { 0x00, 0x00, 0x00, 0x02, 0x00, 0x0A };
volatile static uint8_t config461[MAX_COMMAND] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x14 };
// random query stuff
volatile static uint8_t config47[MAX_COMMAND] = { 0x00, 0x00, 0x02, 0x00, 0x00, 0x00 };
// random query stuff
volatile static uint8_t config4c0[MAX_COMMAND] = { 0x00, 0x00, 0x00, 0x04, 0x00, 0x00 };
volatile static uint8_t config4c1[MAX_COMMAND] = { 0x00, 0x00, 0x00, 0x06, 0x00, 0x00 };

// set up motors??
volatile static uint8_t config4d[MAX_COMMAND] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
volatile static uint8_t config4f[MAX_COMMAND] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x5a };

// hold which buttons should send pressure sensitivity
volatile static uint8_t pressures[MAX_COMMAND] = {0};

/**
 * @brief initializes SPI peripheral for usage on the PS2
 */
void init_ps2_spi(void);

/**
 * @brief waits for SPI transfer to end and processes received data from it. BLOCKING
 */
void handle_spi(void);

#endif
