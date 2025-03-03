#ifndef __BLUETOOTH_ADAPTER_H_
#define __BLUETOOTH_ADAPTER_H_

#include <ArduinoBLE.h>

#include "main.h"

// UUID defines
#define ADAPTER_SERVICE_UUID "3784ee57-bda7-499b-a1a6-146e063a62e3"
#define ADAPTER_CHARACTERISTIC_UUID "680f8715-b97a-4ff5-bb0c-4ca4f37d9b94"

typedef struct controller {
    uint8_t l_dx;
    uint8_t l_dy;
    uint8_t r_dx;
    uint8_t r_dy;
    uint8_t button_map_1;
    uint8_t button_map_2;
} controller;

/**
 * @brief Gets a controller struct representing the status of the controller.
 */
controller *get_controller(void);

/**
 * @brief Sets the state of the controller.
 * @param [in] cntrl A controller to reference when setting state.
 */
void update_controller_state(controller *rec_cntrl);

/**
 * initializes the bluetooth adapter for advertising to clients
 */
void initialize_adapter();

/**
 * to be called during the main program loop, checks for device
 *      connection statuses, and updates state accordingly
 */
void update_state();

#endif
