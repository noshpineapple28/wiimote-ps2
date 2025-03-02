#include "main.h"
#include "ble_adapter.h"

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;

  // start the BLE adapter for advertising
  Serial.printf("Initializing BLE\r\n");
  initialize_adapter();
  Serial.printf("BLE Initialized\r\n");

  // start I2C
}

void loop()
{
  // BLE state update check
  update_state();
}
