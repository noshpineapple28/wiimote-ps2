#include <stdint.h>

#include "main.h"
#include "gpio.h"
#include "uart.h"
#include "clock.h"
#include "spi.h"
#include "systick.h"

int main(void)
{
    // init
    System_Clock_Init();
    GPIO_Init();
    UART_Init();
    SPI_Init();
    init_systick();

    // main code
    printf("Hello World!\r\n");
    uint8_t i = 0;

    uint8_t *header = get_header();
    uint8_t *rx = get_RX();
    uint8_t *tx = get_TX();
    uint8_t transfer_size = 0;

    while (1)
    {
        // put your main code here, to run repeatedly:
        while (!check_if_done())
        {
        }

        transfer_size = get_transfer_size();

        for (i = 0; i < transfer_size; i++)
        {
            printf("%02x/%02x ", rx[i], i < 3 ? header[i] : tx[i - 3]);
            rx[i] = 0;
        }
        i = 0;
        printf("\r\n");

        reset_spi();
    }
}
