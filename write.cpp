#include <iomanip>
#include <iostream>
#include <vector>

#include "../common/utils.hpp"

#include "simpleble/SimpleBLE.h"

int main() {
    auto adapter_optional = Utils::getAdapter();

    if (!adapter_optional.has_value()) {
        return EXIT_FAILURE;
    }

    auto adapter = adapter_optional.value();

    std::vector<SimpleBLE::Peripheral> peripherals;

    adapter.set_callback_on_scan_found([&](SimpleBLE::Peripheral peripheral) {
        std::cout << "Found device: " << peripheral.identifier() << " [" << peripheral.address() << "]" << std::endl;
        if (peripheral.is_connectable()) {
            peripherals.push_back(peripheral);
        }
    });

    adapter.set_callback_on_scan_start([]() { std::cout << "Scan started." << std::endl; });
    adapter.set_callback_on_scan_stop([]() { std::cout << "Scan stopped." << std::endl; });
    // Scan for 5 seconds and return.
    adapter.scan_for(5000);

    std::cout << "The following devices were found:" << std::endl;
    for (size_t i = 0; i < peripherals.size(); i++) {
        std::cout << "[" << i << "] " << peripherals[i].identifier() << " [" << peripherals[i].address() << "]"
                  << std::endl;
    }

    auto selection = Utils::getUserInputInt("Please select a device to connect to: ", peripherals.size() - 1);

    if (!selection.has_value()) {
        return EXIT_FAILURE;
    }
    auto peripheral = peripherals[selection.value()];
    std::cout << "Connecting to " << peripheral.identifier() << " [" << peripheral.address() << "]" << std::endl;
    peripheral.connect();

    std::cout << "Successfully connected, printing services and characteristics.." << std::endl;

    // Store all service and characteristic uuids in a vector.
    std::vector<std::pair<SimpleBLE::BluetoothUUID, SimpleBLE::BluetoothUUID>> uuids;
    for (auto service : peripheral.services()) {
        for (auto characteristic : service.characteristics()) {
            uuids.push_back(std::make_pair(service.uuid(), characteristic.uuid()));
        }
    }

    std::cout << "The following services and characteristics were found:" << std::endl;
    for (size_t i = 0; i < uuids.size(); i++) {
        std::cout << "[" << i << "] " << uuids[i].first << " " << uuids[i].second << std::endl;
    }

    selection = Utils::getUserInputInt("Please select a characteristic to write into: ", uuids.size() - 1);

    // std::string contents;
    // std::cout << "Please write the contents to be sent: ";
    // std::cin >> contents;
    uint8_t vals[32] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x30, 0x31, 0x32};
    std::cout << "Please write the contents to be sent: ";
    std::string contents(reinterpret_cast<char*>(vals), sizeof(vals));

    if (!selection.has_value()) {
        return EXIT_FAILURE;
    }

    // NOTE: Alternatively, `write_command` can be used to write to a characteristic too.
    // `write_request` is for unacknowledged writes.
    // `write_command` is for acknowledged writes.
    peripheral.write_request(uuids[selection.value()].first, uuids[selection.value()].second, contents);

    peripheral.disconnect();
    return EXIT_SUCCESS;
}
