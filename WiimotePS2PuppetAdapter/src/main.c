#include "main.h"
#include "ble_handler.h"
#include "wiimote_handle.h"

uint8_t main()
{
    // start ble, exit if failed
    if (initialize_ble())
        return 0;
    if (init_wiimote_handler())
        return 0;

    // begin polling
    begin_polling();
}
