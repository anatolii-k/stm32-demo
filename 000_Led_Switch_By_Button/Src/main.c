/*
 * Board: Nucleo-F446RE
 * Application: When user click's on User Button on board, User Led on board is switched on/off
 * Pins:
 *   PA5  - User Led pin
 *   PC13 - User Button pin
 *
 * */

#include <stdint.h>
#include "stm32f446xx.h"

#define LED_ODR GPIO_ODR_OD5
#define SWICTH_BUTTON_IDR GPIO_IDR_ID13
#define BUTTON_IS_PRESSED (0)

static int lastButtonState = 1;

void setup(){
	// RCC enable
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;

	// output mode for PA5
	GPIOA->MODER |=  GPIO_MODER_MODER5_0;
	GPIOA->MODER &= ~GPIO_MODER_MODER5_1;

	// input mode for PC13
	GPIOC->MODER &= ~GPIO_MODER_MODER13;

	// pull-up for PC13
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD13_1;
	GPIOC->PUPDR |=  GPIO_PUPDR_PUPD13_0;
}

void makeDelay()
{
	for(volatile int i = 0; i < 100000; ++i) {}
}

int main(void)
{
	setup();
	for(;;)
	{
		int buttonState = GPIOC->IDR & SWICTH_BUTTON_IDR ? 1 : 0;
		if( lastButtonState != buttonState )
		{
			lastButtonState = buttonState;
			if(buttonState == BUTTON_IS_PRESSED)
			{
				GPIOA->ODR ^= LED_ODR;
			}
		}
		makeDelay();
	}
}
