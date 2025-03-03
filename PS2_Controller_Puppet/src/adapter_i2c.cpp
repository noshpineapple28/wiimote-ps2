#include "adapter_i2c.h"

uint8_t i2c_awaiting = 0;
uint8_t buf_1[6];
uint8_t buf_2[6];

void poll_controller_status(uint8_t num)
{
    // write low to tell the adapter to send data
    uint8_t *buf = (num == 1) ? buf_1 : buf_2;
    for (uint8_t i = 0; i < 6; i++)
    {
        Wire.requestFrom(0x07, 1);
        while (Wire.available())
        {
            buf[i] = Wire.read();
        }
    }
}

uint8_t compare_buffs()
{
    for (uint8_t i = 0; i < 6; i++)
    {
        if (buf_1[i] != buf_2[i])
            return 0;
    }

    Serial.print("\tREC: ");
    char print_buf[6];
    for (uint8_t i = 0; i < 6; i++)
    {
        sprintf(print_buf, "0x%02x ", buf_1[i]);
        Serial.print(print_buf);
    }
    Serial.println();
    return 1;
}

void init_i2c(void)
{
    Serial.print("Initializing I2C\r\n");
    pinMode(I2C_SCL_PIN, INPUT_PULLUP);
    pinMode(I2C_SDA_PIN, INPUT_PULLUP);
    // set the alert pin to output
    // initialize I2C slave
    Wire.begin();
    Serial.print("I2C Initialized\r\n");
}
