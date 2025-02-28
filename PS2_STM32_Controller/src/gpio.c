#include "main.h"
#include "gpio.h"

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
void GPIO_Init(void)
{
    // Enable GPIOA clock
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    // spi enable
    RCC->APB2ENR|=RCC_APB2ENR_SPI1EN; //enable clock access to SPI1
    // reset AFRs
    GPIOA->AFR[0] = 0x00000000;
    // reset OSPEEDR
    GPIOA->OSPEEDR = 0x0C000000;

    // SPI HARDWARE
    // Set PA1 to input mode
    GPIOA->MODER &= ~(GPIO_MODER_MODE1); // clear PA4
    GPIOA->MODER |= (GPIO_MODER_MODE1_0); // set PA4 to input
    // set PA1 to HIGH speed
    GPIOA->OSPEEDR |= (3 << GPIO_OSPEEDR_OSPEED1_Pos);

    // Set PA4 to input mode
    GPIOA->MODER &= ~(GPIO_MODER_MODE4); // clear PA4
    GPIOA->MODER |= (GPIO_MODER_MODE4_1); // set PA4 to input
    // reset AF4 (SPI NSS) for PA4
    GPIOA->AFR[0] |= (0x05 << GPIO_AFRL_AFSEL4_Pos);
    // set PA4 to HIGH speed
    GPIOA->OSPEEDR |= (3 << GPIO_OSPEEDR_OSPEED4_Pos);

    // Set PA5 to input mode
    GPIOA->MODER &= ~(GPIO_MODER_MODE5); // clear PA5
    GPIOA->MODER |= (GPIO_MODER_MODE5_1); // set PA5 to input
    // reset AF5 (SPI SCK) for PA5
    GPIOA->AFR[0] |= (0x05 << GPIO_AFRL_AFSEL5_Pos);
    // set PA5 to HIGH speed
    GPIOA->OSPEEDR |= (3 << GPIO_OSPEEDR_OSPEED5_Pos);

    // Set PA6 (MISO) to output mode
    GPIOA->MODER &= ~(GPIO_MODER_MODE6); // Clear PA5 mode bits
    GPIOA->MODER |= GPIO_MODER_MODE6_0;  // Set PA5 to output mode
    // reset AF5 (SPI MISO) for PA6
    GPIOA->AFR[0] |= (0x05 << GPIO_AFRL_AFSEL6_Pos);
    // set PA6 to HIGH speed
    GPIOA->OSPEEDR |= (3 << GPIO_OSPEEDR_OSPEED6_Pos);

    // Set PA7 to input mode
    GPIOA->MODER &= ~(GPIO_MODER_MODE7); // clear PA7
    GPIOA->MODER |= (GPIO_MODER_MODE7_1); // set PA7 to input
    // reset AF5 (SPI MOSI) for PA7
    GPIOA->AFR[0] |= (0x05 << GPIO_AFRL_AFSEL7_Pos);
    // set PA7 to HIGH speed
    GPIOA->OSPEEDR |= (3 << GPIO_OSPEEDR_OSPEED7_Pos);
}
