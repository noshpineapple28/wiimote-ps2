#include <SPI.h>
#include "helper.h"

SPIClass *master;

static constexpr size_t BUFFER_SIZE = 8;
uint8_t tx_buf[BUFFER_SIZE]{ 1, 2, 3, 4, 5, 6, 7, 8 };
uint8_t rx_buf[BUFFER_SIZE]{ 0, 0, 0, 0, 0, 0, 0, 0 };

void setup() {
  Serial.begin(115200);

  delay(2000);

  master = new SPIClass(HSPI);
  master->begin(D13, D12, D11, D10);
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH);

  delay(2000);

  Serial.println("start spi master");
}

void loop() {
  // initialize tx/rx buffers
  initializeBuffers(tx_buf, rx_buf, BUFFER_SIZE);

  digitalWrite(SS, LOW);
  master->beginTransaction(SPISettings(500000, MSBFIRST, SPI_MODE0));
  master->transferBytes(tx_buf, rx_buf, BUFFER_SIZE);
  digitalWrite(SS, HIGH);
  master->endTransaction();

  // verify and dump difference with received data
  if (verifyAndDumpDifference("master", tx_buf, BUFFER_SIZE, "slave", rx_buf, BUFFER_SIZE)) {
    Serial.println("successfully received expected data from slave");
  } else {
    Serial.println("unexpected difference found between master/slave data");
  }

  delay(2000);
}