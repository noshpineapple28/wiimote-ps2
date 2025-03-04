#ifndef __ADAPTER_I2C_H_
#define __ADAPTER_I2C_H_

#include <Wire.h>

#include "main.h"

#define I2C_SDA_PIN PIN_WIRE_SDA
#define I2C_SCL_PIN PIN_WIRE_SCL
#define I2C_IRQ_PIN 8

typedef struct controller {
    uint8_t button_map_1;
    uint8_t button_map_2;
    uint8_t r_dx;
    uint8_t r_dy;
    uint8_t l_dx;
    uint8_t l_dy;
} controller;

/**
 * @brief sends a request to the adapter for controller status. BLOCKING
 */
void poll_controller_status(uint8_t num);

/**
 * @brief initializes I2C and necessary events/outputs
 */
void init_i2c(void);

uint8_t compare_buffs();

#endif
