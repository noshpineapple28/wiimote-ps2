#include "i2c.h"
#include "bluetooth_adapter.h"

uint8_t interrupt_occurred = 0;

/**
 * @brief initiated by the Puppet itself, sends data to it
 */
void send_controller_state()
{
    static uint8_t i = 0;
    // get controller
    uint8_t *arr = (uint8_t *)get_controller();
    Wire.write(arr[i++]);
    if (i >= 6)
        i = 0;
    interrupt_occurred = 1;
}

void init_i2c()
{
    Serial.print("Initializing I2C\r\n");
    pinMode(I2C_IRQ_PIN, INPUT);
    Wire.onRequest(send_controller_state);

    Wire.begin(0x07);
    Serial.print("I2C Initialized\r\n");
}

void i2c_loop()
{
    if (interrupt_occurred)
    {
        Serial.print("I2C Transmission ocurred\r\n");
        interrupt_occurred = 0;
    }
}
