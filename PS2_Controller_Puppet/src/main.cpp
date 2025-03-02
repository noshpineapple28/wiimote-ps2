#include <SPI.h>
#include "main.h"
#include "ps2_spi.h"

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;
  // put your setup code here, to run once:
  init_ps2_spi();
}

void loop()
{
  handle_spi();
}
