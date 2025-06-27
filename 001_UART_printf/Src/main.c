#include <stdint.h>
#include <stdio.h>

#include "usart.h"

int main(void)
{
	uart2_tx_init();

	for(uint32_t i = 0; 1 ; ++i)
	{
		printf("Hello from STM32 printf. index=%lu \n\r", i);
	}
}

