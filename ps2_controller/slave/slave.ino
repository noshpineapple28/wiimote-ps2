#include <ESP32SPISlave.h>
#include "helper.h"

ESP32SPISlave slave;

static constexpr size_t BUFFER_SIZE = 8;
static constexpr size_t QUEUE_SIZE = 1;
uint8_t tx_buf[BUFFER_SIZE]{ 1, 2, 3, 4, 5, 6, 7, 8 };
uint8_t rx_buf[BUFFER_SIZE]{ 0, 0, 0, 0, 0, 0, 0, 0 };

void setup() {
  Serial.begin(9600);

  delay(2000);

  slave.setDataMode(SPI_MODE0);    // default: SPI_MODE0
  slave.setQueueSize(QUEUE_SIZE);  // default: 1, requres 2 in this example

  // begin() after setting
  if (!slave.begin(HSPI)) {
    Serial.println("Failed to start");
  }  // default: HSPI (please refer README for pin assignments)

  Serial.println("start spi slave");
}

void loop() {
  while (digitalRead(SS) == HIGH) {}

  // initialize tx/rx buffers
    initializeBuffers(tx_buf, rx_buf, BUFFER_SIZE);

    // start and wait to complete one BIG transaction (same data will be received from slave)
    const size_t received_bytes = slave.queue(tx_buf, rx_buf, BUFFER_SIZE);
    const std::vector<size_t> b = slave.wait();

    // verify and dump difference with received data
    if (verifyAndDumpDifference("slave", tx_buf, BUFFER_SIZE, "master", rx_buf, received_bytes)) {
        Serial.println("successfully received expected data from master");
    } else {
        Serial.println("unexpected difference found between master/slave data");
        Serial.printf("%02x ", rx_buf[0]);
    }
}