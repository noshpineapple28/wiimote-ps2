#ifndef __SYSTICK_H
#define __SYSTICK_H

#include <stdint.h>
#include "main.h"

void init_systick();

void wait_us(uint8_t us);

#endif
