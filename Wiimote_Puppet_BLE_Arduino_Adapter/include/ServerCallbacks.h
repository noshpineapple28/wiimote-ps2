#ifndef __SERVERCALLBACKS_H_
#define __SERVERCALLBACKS_H_

#include "ble_adapter.h"

class ServerCallbacks : public BLEServerCallbacks
{
    public:
        /**
         * @brief creates a ServerCallbacks instance with states appropriately set
         */
        ServerCallbacks();
        /**
         * @brief retrieves the state of the server, and if its connected to a client
         * @returns truthy if connected, falsy if not
         */
        uint8_t get_device_state();
        /**
         * handles the connection event, setting internal states
         * @param [in] pServer the server that received the connection
         */
        void onConnect(BLEServer *pServer);
        /**
         * handles the disconection event, setting internal states
         * @param [in] pServer the server that received the connection
         */
        void onDisconnect(BLEServer *pServer);
    private:
        // server state
        uint8_t device_connected = 0;
};

#endif
