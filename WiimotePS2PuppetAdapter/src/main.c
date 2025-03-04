#include "main.h"
#include "ble_handler.h"
#include "wiimote_handle.h"

uint8_t main()
{
    // start ble, exit if failed
    printf("Initializing BLE\n");
    if (initialize_ble())
        return 0;
    printf("Initializing wiimote\n");
    if (init_wiimote_handler())
        return 0;

    // begin polling
    printf("Beginning polling\n");
    begin_polling();
}
