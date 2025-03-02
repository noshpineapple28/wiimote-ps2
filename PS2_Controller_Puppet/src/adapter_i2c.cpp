#include "adapter_i2c.h"

uint8_t i2c_awaiting = 0;

/**
 * @brief event handler for I2C end of transmission
 * @param [in] len the number of bytes received from master
 */
void handle_date_in(int32_t len)
{
    char buffer[5];
    while (Wire.available())
    {
        sprintf(buffer, "0x%02x", Wire.read());
        Serial.print(buffer);
    }
    Serial.println();
    // end i2c block
    i2c_awaiting = 0;
}

void poll_controller_status(void)
{
    // write low to tell the adapter to send data
    digitalWrite(I2C_IRQ_PIN, LOW);
    i2c_awaiting = 1;
    while (i2c_awaiting)
        ;
    // reset line for I2C adapter
    digitalWrite(I2C_IRQ_PIN, HIGH);
}

void init_i2c(void)
{
    // set the alert pin to output
    pinMode(I2C_IRQ_PIN, OUTPUT);
    digitalWrite(I2C_IRQ_PIN, HIGH);
    // initialize I2C slave
    Wire.begin(1);
    Wire.onReceive(handle_date_in);
}
