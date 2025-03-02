#include "ServerCallbacks.h"

ServerCallbacks::ServerCallbacks()
{
    device_connected = 0;
}

uint8_t ServerCallbacks::get_device_state()
{
    return device_connected;
}

void ServerCallbacks::onConnect(BLEServer *pServer)
{
    device_connected = 1;
};

void ServerCallbacks::onDisconnect(BLEServer *pServer)
{
    device_connected = 0;
}
