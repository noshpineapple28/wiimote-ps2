#include "main.h"
#include "ps2_spi.h"
#include "adapter_i2c.h"

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;
  // put your setup code here, to run once:
  init_ps2_spi();
  init_i2c();
}

void loop()
{
  handle_spi();
  poll_controller_status();
}
