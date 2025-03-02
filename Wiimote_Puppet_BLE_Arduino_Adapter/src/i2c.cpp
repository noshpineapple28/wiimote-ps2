#include "i2c.h"
#include "ble_adapter.h"

/**
 * @brief initiated by the Puppet itself, sends data to it
 */
void send_controller_state(void)
{
    // get controller
    controller *cntrl = get_controller();
    Wire.beginTransmission(1);
    Wire.write(cntrl->l_dx);
    Wire.write(cntrl->l_dy);
    Wire.write(cntrl->r_dx);
    Wire.write(cntrl->r_dx);
    Wire.write(cntrl->button_map_1);
    Wire.write(cntrl->button_map_2);
    Wire.endTransmission();
}

void init_i2c()
{
    pinMode(I2C_IRQ_PIN, INPUT);
    attachInterrupt(I2C_IRQ_PIN, send_controller_state, FALLING);

    Wire.begin();
}
