#include "wiiuse.h"
#include "main.h"
#include "ble_handler.h"

uint8_t main()
{
    // start ble, exit if failed
    if (initialize_ble())
        return 0;

    controller cntrl = {
        .l_dx = 0x44,
        .l_dy = 0x11,
        .r_dx = 0x00,
        .r_dy = 0x74,
        .button_map_1 = 0x41,
        .button_map_2 = 0x14};
    write_to_ps2(&cntrl);
    write_to_ps2(&cntrl);
    write_to_ps2(&cntrl);
    write_to_ps2(&cntrl);

    // loop to keep the program going
    while (1)
        ;
}
