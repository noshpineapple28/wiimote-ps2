// Turns an Arduino Nano ESP32 into a Bluetooth® Low Energy peripheral.
// This BLE peripheral is providing a service that allows a BLE central
// to switch on and off the internal LED of the Arduino Nano ESP32.
// https://tutoduino.fr/
// Copyleft 2023
#include <ArduinoBLE.h>
BLEService ledService("19b10000-e8f2-537e-4f6c-d104768a1214");  // Bluetooth® Low Energy LED Service
// Bluetooth® Low Energy LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLECharacteristic switchCharacteristic("19b10000-e8f2-537e-4f6c-d104768a1214", BLERead | BLEWrite, 32, 1);
const int ledPin = 14;  // internal LED pin

void setup() {
  Serial.begin(9600);
  delay(2000);
  // set LED pin to output mode
  pinMode(ledPin, OUTPUT);
  // BLE initialization
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");
    while (1)
      ;
  }
  Serial.println("started Bluetooth® Low Energy!");
  // set advertised local name and service UUID:
  BLE.setLocalName("LED");
  BLE.setAdvertisedService(ledService);
  // add the characteristic to the service
  ledService.addCharacteristic(switchCharacteristic);
  // add service
  BLE.addService(ledService);
  // set the initial value for the characeristic:
  switchCharacteristic.writeValue(0);
  // start advertising
  BLE.advertise();
  Serial.println("BLE LED Peripheral");
}
void loop() {
  // wait for a Bluetooth® Low Energy central
  BLEDevice central = BLE.central();
  // check if a central is connected to this peripheral
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());
    // while the central is still connected to peripheral:
    while (central.connected()) {
      // if the remote device wrote to the characteristic,
      // use the value to control the LED:
      if (switchCharacteristic.written()) {
        Serial.println("Value Updated");
        switchCharacteristic.read();
        printData(switchCharacteristic.value(), switchCharacteristic.valueLength());
        Serial.println();
        static uint8_t value[6];
        switchCharacteristic.readValue(value, 6);
        Serial.print("beats index1: ");
        Serial.println(value[1]);            //base 10

        // if (switchCharacteristic.value()) {  // any value other than 0
        //   Serial.println("LED on");
        //   digitalWrite(ledPin, LOW);  // will turn the LED on
        // } else {                      // a 0 value
        //   Serial.println(F("LED off"));
        //   digitalWrite(ledPin, HIGH);  // will turn the LED off
        // }
      }
    }
    // the central has disconnected
    Serial.println("Disconnected from central: ");
  }
}

void printData(const unsigned char data[], int length) {
  for (int i = 0; i < length; i++) {
    unsigned char b = data[i];

    Serial.printf("%02x ", b);
  }
}