#include "CharacteristicCallbacks.h"

void CharacteristicCallbacks::onWrite(BLECharacteristic *pCharacteristic)
{
    controller *cntrl = (controller *)pCharacteristic->getData();
    Serial.printf("stickL %x:%x stickR %x:%x FIRST %x SECOND %x\r\n",
                  cntrl->l_dx,
                  cntrl->l_dy,
                  cntrl->r_dx,
                  cntrl->r_dy,
                  cntrl->button_map_1,
                  cntrl->button_map_2);
}
