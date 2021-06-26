#include "stm32f4xx.h"                  // Device header

int main() {
	RCC->AHB1ENR |= 1ul << 3; 		//GPIOD Clock Enable
	GPIOD->MODER |= 0x55000000;		//PD12, PD13, PD14, and PD15 set as an output
	
	GPIOD->ODR |= 1ul << 12;
}
