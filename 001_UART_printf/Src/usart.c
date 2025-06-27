#include <stdint.h>
#include <stdio.h>
#include <stm32f446xx.h>

#include "usart.h"

#define ALTER_FUNCTION_AF7 (0x7)
#define CLOCK_FREQ_16_MHz (16000000)
#define USART2_BAUDRATE (115200)

static void gpio_PA2_init();
static void set_USART_BRR(USART_TypeDef * pUSART, uint32_t clkFrequence, uint32_t baudRate);

int __io_putchar(int ch)
{
	uart2_write(ch);
	return ch;
}

void uart2_tx_init(){

	gpio_PA2_init();

	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	set_USART_BRR( USART2, CLOCK_FREQ_16_MHz, USART2_BAUDRATE);

	// Enable USART and TX
	USART2->CR1 = USART_CR1_UE | USART_CR1_TE;
}

void uart2_write(char ch){
	while(!(USART2->SR & USART_SR_TXE)) {}
	USART2->DR = (uint32_t)ch;
}


static void gpio_PA2_init(){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	GPIOA->MODER &= ~GPIO_MODER_MODER2_0;
	GPIOA->MODER |=  GPIO_MODER_MODER2_1;

	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL2_Msk;
	GPIOA->AFR[0] |= ALTER_FUNCTION_AF7 << GPIO_AFRL_AFSEL2_Pos;
}

static void set_USART_BRR(USART_TypeDef * pUSART, uint32_t clkFrequence, uint32_t baudRate)
{
	// non standard formula. But it's simple and it do work.
	pUSART->BRR = (clkFrequence + baudRate/2) / baudRate;
}

