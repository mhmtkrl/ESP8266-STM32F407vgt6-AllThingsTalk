#include "timer7Driver.h"

void timer7Init(void) {
	////////////////-1 second Interrupt-//////////////////
	RCC->APB1ENR |= 1ul << 5;	//TIM7 clock is enabled
	
	TIM7->CR1 |= 1ul << 2;	  //Only counter overflow generates an update interrupt
	TIM7->DIER |= 1ul << 0;		//TIM7 Update Interrupt Enable
	TIM7->CNT = 0;
	TIM7->PSC = 7999;					//RM0090 page: 706 => The Counter Clock = f_ck_psk / (TIM7->PSC + 1)
	TIM7->ARR = 2000;       
	TIM7->CR1 |= 1ul << 0;
  NVIC_EnableIRQ(TIM7_IRQn);
}
