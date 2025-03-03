#include "bluetooth_adapter.h"

BLEService service(ADAPTER_SERVICE_UUID);
BLEDoubleCharacteristic controller_state(
    ADAPTER_CHARACTERISTIC_UUID,
    BLERead | BLEWrite);

// status of the controller
static controller cntrl = {0};

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
    // add service
    BLE.setAdvertisedService(service);
    service.addCharacteristic(controller_state);
    BLE.addService(service);
    controller_state.writeValue(0xffffffffffff);

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
                double val = controller_state.value();
                uint8_t *arr = (uint8_t *)&val;
                char buf[40];
                // assigns controller state values BUT ALSO it prints it :D
                sprintf(buf, "0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x \r\n",
                        (cntrl.l_dx = arr[0]),
                        (cntrl.l_dy = arr[1]),
                        (cntrl.r_dx = arr[2]),
                        (cntrl.r_dy = arr[3]),
                        (cntrl.button_map_1 = arr[4]),
                        (cntrl.button_map_2 = arr[5]));
                Serial.print(buf);
            }
        }

        Serial.print("Connection to central device: ");
        Serial.print(central_device.address());
        Serial.print(" ended :(\r\n");
    }
}
