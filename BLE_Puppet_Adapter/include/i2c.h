#ifndef __I2C_H_
#define __I2C_H_

#include <Wire.h>
#include "main.h"

#define I2C_SDA_PIN WIRE_SDA_PIN
#define I2C_SCL_PIN WIRE_SCL_PIN

/**
 * @brief initializes I2C pins for communication
 */
void init_i2c(void);

#endif
