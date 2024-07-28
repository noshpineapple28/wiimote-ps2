/*
  LED Control

  This example scans for Bluetooth® Low Energy peripherals until one with the advertised service
  "19b10000-e8f2-537e-4f6c-d104768a1214" UUID is found. Once discovered and connected,
  it will remotely control the Bluetooth® Low Energy peripheral's LED, when the button is pressed or released.

  The circuit:
  - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
    Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.
  - Button with pull-up resistor connected to pin 2.

  You can use it with another board that is compatible with this library and the
  Peripherals -> LED example.

  This example code is in the public domain.
*/

#include <ArduinoBLE.h>

const char* UUID = "19b10000-e8f2-537e-4f6c-d104768a1214";

BLEDevice peripheral;
BLECharacteristic ledCharacteristic;

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;

  // initialize the Bluetooth® Low Energy hardware
  BLE.begin();

  Serial.println("Bluetooth® Low Energy Central - LED control");

  // start scanning for peripherals
  BLE.scanForUuid(UUID);

  // check if a peripheral has been discovered
  peripheral = BLE.available();

  if (peripheral) {
    // discovered a peripheral, print out address, local name, and advertised service
    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();

    if (peripheral.localName() != "LED") {
      return;
    }

    // stop scanning
    BLE.stopScan();

    controlLed(peripheral);

    // peripheral disconnected, start scanning again
    BLE.scanForUuid(UUID);
  }
}

void loop() {
  if (!peripheral.connected()) {
    return;
  }

  if (ledCharacteristic.valueUpdated()) {
    Serial.println("Value Updated");
    ledCharacteristic.read();
    printData(ledCharacteristic.value(), ledCharacteristic.valueLength());
    Serial.println();
    static uint8_t value[6];
    ledCharacteristic.readValue(value, 6);
    Serial.print("beats index1: ");
    Serial.println(value[1]);  //base 10
  }
}

void printData(const unsigned char data[], int length) {
  for (int i = 0; i < length; i++) {
    unsigned char b = data[i];

    Serial.printf("%02x ", b);
  }
}

void controlLed(BLEDevice peripheral) {
  // connect to the peripheral
  Serial.println("Connecting ...");

  if (peripheral.connect()) {
    Serial.println("Connected");
  } else {
    Serial.println("Failed to connect!");
    return;
  }

  // discover peripheral attributes
  Serial.println("Discovering attributes ...");
  if (peripheral.discoverAttributes()) {
    Serial.println("Attributes discovered");
  } else {
    Serial.println("Attribute discovery failed!");
    peripheral.disconnect();
    return;
  }

  // retrieve the LED characteristic
  ledCharacteristic = peripheral.characteristic(UUID);

  Serial.println("Subscribing to simple key characteristic ...");
  if (!ledCharacteristic) {
    Serial.println("no simple key characteristic found!");
    peripheral.disconnect();
    return;
  } else if (!ledCharacteristic.canSubscribe()) {
    Serial.println("characteristic is not subscribable!");
    peripheral.disconnect();
    return;
  } else if (!ledCharacteristic.subscribe()) {
    Serial.println("subscription failed!");
    peripheral.disconnect();
    return;
  }

  Serial.println("Peripheral disconnected");
}