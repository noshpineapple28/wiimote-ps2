#include <stdint.h>
#include "systick.h"

void init_systick()
{
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk;
    SysTick->LOAD = 80;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void wait_us(uint8_t us)
{
    while (us)
    {
        while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
        us--;
    }
}
