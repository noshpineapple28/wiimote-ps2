#include "bluetooth_adapter.h"

BLEService service(ADAPTER_SERVICE_UUID);
BLEStringCharacteristic controller_state(
    ADAPTER_CHARACTERISTIC_UUID,
    BLEWriteWithoutResponse, 6);

// status of the controller
static controller cntrl = {
    .button_map_1 = 0xff,
    .button_map_2 = 0xff,
    .r_dx = 0x7f,
    .r_dy = 0x7f,
    .l_dx = 0x7f,
    .l_dy = 0x7f};

controller *get_controller(void)
{
    return &cntrl;
}

void update_controller_state(controller *rec_cntrl)
{
    cntrl.l_dx = rec_cntrl->l_dx;
    cntrl.l_dy = rec_cntrl->l_dy;
    cntrl.r_dx = rec_cntrl->r_dx;
    cntrl.r_dy = rec_cntrl->r_dy;
    cntrl.button_map_1 = rec_cntrl->button_map_1;
    cntrl.button_map_2 = rec_cntrl->button_map_2;
}

/**
 * initializes the bluetooth adapter for advertising to clients
 */
void initialize_adapter()
{
    Serial.print("Initializing BLE\r\n");
    // begin initialization
    if (!BLE.begin())
    {
        Serial.println("starting Bluetooth® Low Energy module failed!");
        while (1)
            ;
    }

    // setup
    BLE.setLocalName("PS2_Puppet_Adapter");
    BLE.setConnectionInterval(7, 7);
    // add service
    BLE.setAdvertisedService(service);
    service.addCharacteristic(controller_state);
    BLE.addService(service);

    // begin
    BLE.advertise();
    Serial.print("BLE Initialized\r\n");
}

void update_state()
{
    BLEDevice central_device = BLE.central();

    if (central_device)
    {
        Serial.print("Connection to central device: ");
        Serial.print(central_device.address());
        Serial.print(" successful!\r\n");

        while (central_device.connected())
        {
            if (controller_state.written())
            {
                String arr = controller_state.value();
                // assigns controller state values BUT ALSO it prints it :D
                cntrl.l_dx = (uint8_t)arr[0];
                cntrl.l_dy = (uint8_t)arr[1];
                cntrl.r_dx = (uint8_t)arr[2];
                cntrl.r_dy = (uint8_t)arr[3];
                cntrl.button_map_1 = (uint8_t)arr[4];
                cntrl.button_map_2 = (uint8_t)arr[5];
            }
        }

        Serial.print("Connection to central device: ");
        Serial.print(central_device.address());
        Serial.print(" ended :(\r\n");
    }
}
