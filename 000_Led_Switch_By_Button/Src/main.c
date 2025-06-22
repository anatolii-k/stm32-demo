/*
 * Board: Nucleo-F446RE
 * Application: When user click's on User Button on board, User Led on board is switched on/off
 * Pins:
 *   PA5  - User Led pin
 *   PC13 - User Button pin
 *
 * */

#include <stdint.h>

// REGISTERS

#define RCC_BASE (0x40023800UL)
#define RCC_AHB1ENR_OFFSET (0x30UL)
#define RCC_AHB1ENR (*(volatile uint32_t*)( RCC_BASE + RCC_AHB1ENR_OFFSET ))
#define RCC_GPIOAEN (1 << 0)
#define RCC_GPIOCEN (1 << 2)

#define GPIO_MODER_OFFSET (0x0)
#define GPIO_MODER5_MASK (0xC00)
#define GPIO_MODER13_MASK (0x0C000000UL)
#define GPIO_MODER5_OUTPUT (1<<10)
#define GPIO_ODR_OFFSET (0x14)
#define GPIO_PUPDR_OFFSET (0x0C)
#define GPIO_PUPDR13_MASK (0xC00)
#define GPIO_PUPDR13_PULLUP (1<<26)
#define GPIO_IDR_OFFSET (0x10)

#define GPIOA_BASE (0x40020000UL)
#define GPIOC_BASE (0x40020800UL)
#define GPIOA_MODER (*(volatile uint32_t*)(GPIOA_BASE + GPIO_MODER_OFFSET))
#define GPIOC_MODER (*(volatile uint32_t*)(GPIOC_BASE + GPIO_MODER_OFFSET))
#define GPIOA_ODR (*(volatile uint32_t*)(GPIOA_BASE + GPIO_ODR_OFFSET))
#define GPIOC_PUPDR (*(volatile uint32_t*)(GPIOC_BASE + GPIO_PUPDR_OFFSET))
#define GPIOC_IDR (*(volatile uint32_t*)(GPIOC_BASE + GPIO_IDR_OFFSET))

#define PIN_5 (1 << 5)
#define PIN_13 (1 << 13)

//////// APPLICATION /////////

#define LED_PIN PIN_5
#define SWICTH_BUTTON_PIN PIN_13
#define BUTTON_IS_PRESSED (0)

static int lastButtonState = 1;

void setup(){
	// RCC enable
	RCC_AHB1ENR |= RCC_GPIOAEN | RCC_GPIOCEN;

	// output mode for PA5
	GPIOA_MODER &= ~GPIO_MODER5_MASK;
	GPIOA_MODER |= GPIO_MODER5_OUTPUT;

	// input mode for PC13
	GPIOC_MODER &=~GPIO_MODER13_MASK;

	// pull-up for PC13
	GPIOC_PUPDR &= ~GPIO_PUPDR13_MASK;
	GPIOC_PUPDR |= GPIO_PUPDR13_PULLUP;
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
		int buttonState = GPIOC_IDR & SWICTH_BUTTON_PIN ? 1 : 0;
		if( lastButtonState != buttonState )
		{
			lastButtonState = buttonState;
			if(buttonState == BUTTON_IS_PRESSED)
			{
				GPIOA_ODR ^= LED_PIN;
			}
		}
		makeDelay();
	}
}
