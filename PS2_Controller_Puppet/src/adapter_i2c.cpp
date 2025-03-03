#include "adapter_i2c.h"

uint8_t i2c_awaiting = 0;

void poll_controller_status(void)
{
    // write low to tell the adapter to send data
    Serial.print("\tREC: ");
    char print_buf[6];
    for (uint8_t i = 0; i < 6; i++)
    {
        Wire.requestFrom(0x07, 1);
        while (Wire.available())
        {
            sprintf(print_buf, "0x%02x ", Wire.read());
            Serial.print(print_buf);
        }
    }
    Serial.println();
}

void init_i2c(void)
{
    Serial.print("Initializing I2C\r\n");
    // set the alert pin to output
    // initialize I2C slave
    Wire.begin();
    Serial.print("I2C Initialized\r\n");
}
