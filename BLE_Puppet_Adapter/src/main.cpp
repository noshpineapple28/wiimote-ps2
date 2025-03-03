#include "main.h"
#include "i2c.h"
#include "bluetooth_adapter.h"

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  init_i2c();
  initialize_adapter();
}

void loop() {
  update_state();
}
