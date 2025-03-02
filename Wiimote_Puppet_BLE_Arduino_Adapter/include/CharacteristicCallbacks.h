#ifndef __CHARACTERISTICCALLBACKS_H_
#define __CHARACTERISTICCALLBACKS_H_

#include "ble_adapter.h"

class CharacteristicCallbacks : public BLECharacteristicCallbacks
{
    public:
        /**
         * @brief DEPRECATED! Callback function to support a write request. Called only if onWrite(,) not overrided.
         * @param [in] pCharacteristic The characteristic that is the source of the event.
         */
        void onWrite(BLECharacteristic *pCharacteristic);
};

#endif
