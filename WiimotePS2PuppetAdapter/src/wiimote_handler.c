#include "wiimote_handle.h"
#include "wiiuse_src/wiiuse.h"
#include "ble_handler.h"

// the wiimote state written as a PS2 controller
controller cntrl = {
    .button_map_1 = 0xff,
    .button_map_2 = 0xff,
    .r_dx = 0x7f,
    .r_dy = 0x7f,
    .l_dx = 0x7f,
    .l_dy = 0x7f};
// pointer to wiimote connected to
wiimote *remote;

/**
 * @brief reads inputs from a wiimote to determine button presses
 * @param remote reference to the remote we are checking
 */
void handle_input(wiimote *remote)
{
    if (IS_JUST_PRESSED(remote, WIIMOTE_BUTTON_A))
        cntrl.button_map_2 &= ~BM2_X_MASK;
    else if (IS_RELEASED(remote, WIIMOTE_BUTTON_A))
        cntrl.button_map_2 |= BM2_X_MASK;

    if (IS_JUST_PRESSED(remote, WIIMOTE_BUTTON_B))
        cntrl.button_map_2 &= ~BM2_O_MASK;
    else if (IS_RELEASED(remote, WIIMOTE_BUTTON_B))
        cntrl.button_map_2 |= BM2_O_MASK;

    if (IS_JUST_PRESSED(remote, WIIMOTE_BUTTON_ONE))
        cntrl.button_map_2 &= ~BM2_SQUARE_MASK;
    else if (IS_RELEASED(remote, WIIMOTE_BUTTON_ONE))
        cntrl.button_map_2 |= BM2_SQUARE_MASK;

    if (IS_JUST_PRESSED(remote, WIIMOTE_BUTTON_TWO))
        cntrl.button_map_2 &= ~BM2_TRIANGLE_MASK;
    else if (IS_RELEASED(remote, WIIMOTE_BUTTON_TWO))
        cntrl.button_map_2 |= BM2_TRIANGLE_MASK;

    if (IS_JUST_PRESSED(remote, WIIMOTE_BUTTON_HOME))
        cntrl.button_map_1 &= ~BM1_START_MASK;
    else if (IS_RELEASED(remote, WIIMOTE_BUTTON_HOME))
        cntrl.button_map_1 |= BM1_START_MASK;

    // D PAD
    if (IS_JUST_PRESSED(remote, WIIMOTE_BUTTON_UP))
        cntrl.button_map_1 &= ~BM1_UP_MASK;
    else if (IS_RELEASED(remote, WIIMOTE_BUTTON_UP))
        cntrl.button_map_1 |= BM1_UP_MASK;

    if (IS_JUST_PRESSED(remote, WIIMOTE_BUTTON_RIGHT))
        cntrl.button_map_1 &= ~BM1_RIGHT_MASK;
    else if (IS_RELEASED(remote, WIIMOTE_BUTTON_RIGHT))
        cntrl.button_map_1 |= BM1_RIGHT_MASK;

    if (IS_JUST_PRESSED(remote, WIIMOTE_BUTTON_DOWN))
        cntrl.button_map_1 &= ~BM1_DOWN_MASK;
    else if (IS_RELEASED(remote, WIIMOTE_BUTTON_DOWN))
        cntrl.button_map_1 |= BM1_DOWN_MASK;

    if (IS_JUST_PRESSED(remote, WIIMOTE_BUTTON_LEFT))
        cntrl.button_map_1 &= ~BM1_LEFT_MASK;
    else if (IS_RELEASED(remote, WIIMOTE_BUTTON_LEFT))
        cntrl.button_map_1 |= BM1_LEFT_MASK;
}

uint8_t init_wiimote_handler()
{
    wiimote **wiimotes_found;
    wiimotes_found = wiiuse_init(1);

    if (!wiiuse_find(wiimotes_found, 1, 5))
    {
        printf("NO WIIMOTES FOUND\n");
        return 1;
    }

    // set wiimotes
    if (!wiiuse_connect(wiimotes_found, 1))
    {
        printf("Failed to connect to remote\n");
        return 1;
    }
    remote = wiimotes_found[0];
    wiiuse_set_leds(remote, 0x00);
    wiiuse_toggle_rumble(remote, 1);
    Sleep(200);
    wiiuse_toggle_rumble(remote, 0);

    return 0;
}

short any_wiimote_connected(wiimote **wm, int wiimotes)
{
    int i;
    if (!wm)
    {
        return 0;
    }

    for (i = 0; i < wiimotes; i++)
    {
        if (wm[i] && WIIMOTE_IS_CONNECTED(wm[i]))
        {
            return 1;
        }
    }

    return 0;
}

void begin_polling()
{
    while (any_wiimote_connected(&remote, 1))
    {
        if (wiiuse_poll(&remote, 1))
        {
            /*
             *  This happens if something happened on any wiimote.
             *  So go through each one and check if anything happened.
             */
            switch (remote->event)
            {
            case WIIUSE_EVENT:
                /* a generic event occurred */
                handle_input(remote);
                write_to_ps2(&cntrl);
                break;

            default:
                break;
            }

            // joystick inputs
            // handle_joystick(remote[0]->exp.nunchuk.js.x, remote[0]->exp.nunchuk.js.y);
            cntrl.r_dx = ((remote->exp.nunchuk.js.x + 1) / 2) * 0xff;
            cntrl.r_dy = ((remote->exp.nunchuk.js.y + 1) / 2) * 0xff;
            Sleep(50);
        }
    }
    wiiuse_cleanup(&remote, 1);
}
