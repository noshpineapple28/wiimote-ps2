#ifndef __SPI_H
#define __SPI_H

#include <stdint.h>

#define MAX_COMMAND 32

void reset_spi(void);

uint8_t check_if_done(void);

uint8_t get_transfer_size(void);

uint8_t *get_header(void);

uint8_t *get_RX(void);

uint8_t *get_TX(void);

void SPI_Init(void);

#endif
