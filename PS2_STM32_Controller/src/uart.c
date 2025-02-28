/*
 * UART.c
 *
 *  Created on: Nov 6, 2024
 *      Author: Noah
 */
#include "main.h"
#include "uart.h"
#include "stdarg.h"
#include "stdio.h"

static char BUFFER[100];

/*
 * UART Delay
 * Write string on USART2 for user to see the output
 * Enter Byte by Byte and add some wait time to ensure data is transmitted before writing next byte
 *
 */
void UART_Delay(uint32_t us)
{
	uint32_t time = 100 * us / 7;
	while (--time)
		;
}

/*
 * UART Receive
 *
 * Wait until data is received at USART 2
 * Return character only
 *
 */
char UART_ReceiveChar(void)
{
	while (!(USART2->ISR & USART_ISR_RXNE))
		; // Wait until data is received

	return ((char)(USART2->RDR & 0xFF));
}

/*
 * UART Transmit/Send Character
 *
 * Write a character on USART2 for user to see the output
 * wait to allow data is transmitted before sending next Character
 * Return character only
 *
 */
void UART_SendChar(char c)
{
	// Wait until the transmit data register is empty
	while (!(USART2->ISR & USART_ISR_TXE))
		;
	USART2->TDR = c; // Transmit the character
	// UART_Delay(50);

	// Writing USART_DR automatically clears the TXE flag
	while (!(USART2->ISR & USART_ISR_TC))
		; // wait until TC bit is set
	USART2->ISR &= ~USART_ISR_TC;
}

/*
 * UART Transmit/Send String
 *
 * Write string on USART2 for user to see the output
 * Enter Byte by Byte and add some wait time to ensure data is transmitted before writing next byte
 *
 */
void UART_SendString(char *str)
{
	while (*str)
	{
		UART_SendChar(*str++);
	}
}

/**
 * prints a string out, and adds a new line
 *
 * @param str	a pointer to a string to print
 *
 * @return		the number of characters printed
 */
int puts(const char *str)
{
	int ret = sprintf(BUFFER, "%s\r\n", str);
	UART_SendString((char *)BUFFER);
	return ret;
}

/**
 * prints a specific number of characters
 *
 * @param str	a pointer to a string to print
 * @param size	the number of characters from str to print
 *
 * @return		the number of characters printed
 */
int putns(const char *str, int size)
{
	int ret = snprintf(BUFFER, size + 1, "%s", str); // len of string plus \0
	UART_SendString((char *)BUFFER);
	UART_SendString((char *)"\r\n");
	return ret + 2;
}

/**
 * prints a given character to the console
 *
 * @param c	the value of a character to print out
 *
 * @return	the number of characters printed
 */
int putchar(int c)
{
	UART_SendChar(c);
	return 1;
}

/**
 * prints a string out, and allows insertion of data values into formatted
 * 		string
 *
 * @param format	a pointer to a string to print, along with format chars
 * @param ...		data values to insert into format char positions
 *
 * @return		the number of characters printed
 */
int printf(const char *format, ...)
{
	// formats the string
	va_list aptr;
	va_start(aptr, format);
	int ret = vsprintf(BUFFER, format, aptr);
	va_end(aptr);

	// writes to output
	UART_SendString((char *)BUFFER);

	// return string length
	return (ret);
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}
/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
void UART_Init(void)
{

	// 1. Enable the clock of USART
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN; // Enable USART 2 clock

	// 2. Select the USART1 clock source
	//
	// 00: PCLK selected as USART2 clock
	// 01: System clock (SYSCLK) selected as USART2 clock
	// 10: HSI16 clock selected as USART2 clock
	// 11: LSE clock selected as USART2 clock
	RCC->CCIPR &= ~RCC_CCIPR_USART2SEL;
	RCC->CCIPR |= RCC_CCIPR_USART2SEL_0;

	// 3. Enable USART2 clock (assuming PA2 for TX and PA3 for RX)
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

	// 4. USART 2 configuration
	// ********************** USART 2 ***************************
	// PA2 = USART2_TX (AF7)
	// PA3 = USART2_RX (AF7)
	// Alternate function, High Speed, Push pull, Pull up
	// **********************************************************
	// Input(00), Output(01), AlterFunc(10), Analog(11)
	GPIOA->MODER &= ~(3 << (2 * TX_PIN) | 3 << (2 * RX_PIN)); // Clear bits
	GPIOA->MODER |= 2 << (2 * TX_PIN) | 2 << (2 * RX_PIN);
	GPIOA->AFR[0] &= ~(0xF << (4 * TX_PIN) | 0xF << (4 * RX_PIN));
	GPIOA->AFR[0] |= 7 << (4 * TX_PIN) | 7 << (4 * RX_PIN);
	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	GPIOA->OSPEEDR |= 3 << (2 * TX_PIN) | 3 << (2 * RX_PIN);
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOA->PUPDR &= ~(3 << (2 * TX_PIN) | 3 << (2 * RX_PIN));
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1)
	GPIOA->OTYPER &= ~(1 << TX_PIN | 1 << RX_PIN);

	// Default setting:
	//     No hardware flow control, 8 data bits, no parity, 1 start bit and 1 stop bit
	USART2->CR1 &= ~USART_CR1_UE; // Disable USART

	// Configure word length to 8 bit
	USART2->CR1 &= ~USART_CR1_M; // M: 00 = 8 data bits, 01 = 9 data bits, 10 = 7 data bits

	// Configure oversampling mode: Oversampling by 16
	USART2->CR1 &= ~USART_CR1_OVER8; // 0 = oversampling by 16, 1 = oversampling by 8

	// Configure stop bits to 1 stop bit
	//   00: 1 Stop bit;      01: 0.5 Stop bit
	//   10: 2 Stop bits;     11: 1.5 Stop bit
	USART2->CR2 &= ~USART_CR2_STOP;

	// CSet Baudrate to 9600 using APB frequency (80,000,000 Hz)
	// If oversampling by 16, Tx/Rx baud = f_CK / USARTDIV,
	// If oversampling by 8,  Tx/Rx baud = 2*f_CK / USARTDIV
	// When OVER8 = 0, BRR = USARTDIV
	// USARTDIV = 80MHz/9600 = 8333 = 0x208D
	USART2->BRR = 694; // Limited to 16 bits

	// Transmitter and Receiver enable
	USART2->CR1 |= (USART_CR1_RE | USART_CR1_TE);

	USART2->ICR |= USART_ICR_TCCF;

	// USART enable
	USART2->CR1 |= USART_CR1_UE;

	// Verify that the USART is ready for reception
	while ((USART2->ISR & USART_ISR_TEACK) == 0)
		;
	// Verify that the USART is ready for transmission
	while ((USART2->ISR & USART_ISR_REACK) == 0)
		;
}
