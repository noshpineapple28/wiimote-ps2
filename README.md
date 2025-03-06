# wiimote-ps2

The Wii Remote is such an amazing controller, wouldn't it be awesome to play a similarly amazing console with it?

## Project Summary

A completed reverse engineering project focused on analyzing PS2 controllers and being able to simulate inputs to a stock PS2. The final goal is to connect a wii remote to the arduino and translate inputs from a wii remote to a PS2.

This provides real time button presses with close to 0 latency.

As well, from testing this also can work with PSX/PS1 games.

## Project Tools

- Arduino Uno R3
  - This was picked due to the ease of programming SPI communication on it.
  - Setup is done with bare metal register access, allowing for quick setup. All operations are documented within source code
- Arduino Uno R4 Wifi
  - Becuase the R3 doesn't have BLE, I subbed in the nearest BLE capable device I had which was the R4. This connects to the R3 using I2C. This device can easily be replaced with something equally as capable with minimal edits (such as a Nano ESP32 w/ a level shifter)
- A desktop
  - I had a hard time interfacing directly with a wii remote using just Arduinos, so instead a desktop runs a application written primarily in C for best performance. This connects to any wii remotes connected to your desktop
    - If you need resources on connecting a Wii Remote to your desktop, [WiiPair](https://github.com/jordanbtucker/WiiPair) is an amazing tool.
- A PlayStation 2

## The stack

The overall structure of this project is 5 linked devices:
 - The PlayStation 2
 - The PS2 Controller "Puppet"
    - The puppet is the simulated controller that interacts with the PlayStation 2.
    - The puppet is linked to the PS2 using SPI, and to the BLE adapter using I2C.
        - For obvious reasons I will be using the Controller/Peripheral terminology when referring to SPI pins
    - The PS2 uses an extra "Acknowledgment" line originally intended it seems to allow Dualshock controllers extra time to process data. When finished processing data, they pulse the line to alert that the device is ready to handle more information.
 - The BLE Puppet adapter
    - Using I2C, the adapter interfaces with the Puppet, sending data about the state of the Wii Remote
    - Using BLE, a desktop can write to the adapter information about wii remotes
 - A desktop
    - Compile the `WiimotePS2PuppetAdapter` with CMake and begin connecting to a Wii Remote and BLE adapter. Afterwards, nothing is left to be done!

## Wiring

### The PlayStation 2 Dualshock Port Wiring

The dualshock is comprised of 9 wires, in the following order (this is the representation of the controller port if looked at from face forward, __AGAIN THIS IS THE ORDER OF THE PIN HOLES ON THE PS2__)
```
                                          i   o
      k   k    k          w   d     b   s   s
     c   n    c     s   o   n     i   o   i
    A   U   S     C   P   G     V   M   M
==============================================
|   o   o   o     o   o   o     o   o   o    |
==============================================
```
 - Ack - Acknowledgment line
    - The PS2 uses an extra "Acknowledgment" line originally intended it seems to allow Dualshock controllers extra time to process data. When finished processing data, they pulse the line to alert that the device is ready to handle more information.
 - Unk - Unknown line
    - Unused, fun fact most 3rd party controllers just omit this line, its not needed
 - Sck - Serial Clock Line
    - The SPI clock line
 - CS - Chip Select Line
    - The SPI CS line
 - Pow - Power Line
 - Gnd - Ground Line
 - Vib - Vibration Line
    - This line is designed to send power directly to the rumble motors of a Dualshock controller. When the controller is supposed to rumble, it powers the motors directly! Be careful, I haven't implemented rumble as its signal is above the operating voltage of 5v which arduinos use!
 - MOSI - Controller Out Peripheral In Line
    - Standard SPI MOSI Line
 - MISO - Controller In Peripheral Out Line
    - Standard SPI MISO Line

### Arduino Wiring

```
UNO R3                 UNO R4 Wifi
-------------------------------------
A5 [I2C SDA]    <->    A5 [I2C SDA]
A4 [I2C SCK]    <->    A4 [I2C SCK]
<!-- Always link to common ground -->
5V              <->    Vin
Gnd             <->    Gnd
```
```
UNO R3                 PlayStation 2
-------------------------------------
D13 [SPI SCK]   <->    Sck
D12 [SPI MISO]  <->    MISO
D11 [SPI MOSI]  <->    MOSI
D10 [SPI CS]    <->    CS
D8 [ACK]        <->    Ack
<!-- Always link to common ground -->
Vin             <->    Pow
Gnd             <->    Gnd
```
*Note: The PS2 Power line is not enough to power the R4 or R3 alone, so it is advised that you use some external power source to power the R3, then link the R4 and PS2 to it. A Simple usb cable to your desktop suffices.
