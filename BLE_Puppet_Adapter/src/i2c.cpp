#include "i2c.h"
#include "bluetooth_adapter.h"

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
}

void init_i2c()
{
    Serial.print("Initializing I2C\r\n");
    Wire.onRequest(send_controller_state);

    Wire.begin(0x07);
    Serial.print("I2C Initialized\r\n");
}
