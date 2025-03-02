#include "ble_adapter.h"
#include "ServerCallbacks.h"
#include "CharacteristicCallbacks.h"

// created up here so they don't get garbage collected
BLEService *puppet_service;
BLECharacteristic *controller_states;
BLEServer *server;

// callbacks and states
uint8_t server_advertising = 0;
ServerCallbacks *server_callbacks = new ServerCallbacks();
CharacteristicCallbacks *characteristic_callbacks = new CharacteristicCallbacks();

// status of the controller
controller cntrl = {0};

void initialize_adapter()
{
    // NAMING
    BLEDevice::init("PS2PuppetAdapter");
    BLEServer *server = BLEDevice::createServer();
    server->setCallbacks(server_callbacks);

    // services
    puppet_service = server->createService(ADAPTER_SERVICE_UUID);

    // characteristics
    controller_states = puppet_service->createCharacteristic(
        ADAPTER_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE);
    double val = 0xffffffffffff;
    controller_states->setValue(val);
    controller_states->setCallbacks(characteristic_callbacks);

    // start the service
    puppet_service->start();

    // begin advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID("3784ee57-bda7-499b-a1a6-146e063a62e3");
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
    server_advertising = 1;
}

void update_state()
{
    // check if the server is connected to anything and if we arent advertising
    if (!server_callbacks->get_device_state() && !server_advertising)
    {
        delay(500);                 // give the bluetooth stack the chance to get things ready
        server->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        server_advertising = 1;
    }
    else if (server_callbacks->get_device_state())
    {
        server_advertising = 0;
    }
}

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
