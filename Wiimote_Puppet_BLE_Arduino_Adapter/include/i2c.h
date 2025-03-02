#ifndef __I2C_H_
#define __I2C_H_

#include <Wire.h>
#include "main.h"

#define I2C_IRQ_PIN D10
#define I2C_SDA_PIN A4
#define I2C_SCL_PIN A5

/**
 * @brief initializes I2C pins for communication
 */
void init_i2c(void);

#endif
