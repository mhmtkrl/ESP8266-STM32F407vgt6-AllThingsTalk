#include "stm32f4xx.h"                  // Device header

void bluetoothSendChar(uint8_t);
void bluetoothSendString(uint8_t *);

uint8_t packet[] = "Hello World!\r\n";
	
int main() {
	//////////HC05 Setup//////////
	RCC->AHB1ENR |= 1ul << 0;			//GPIOA Clock Enable
	RCC->APB1ENR|= 1ul << 17;   	//USART2 Clock Enable
	
	GPIOA->MODER |=0xA80000A0;		//PA2 and PA3 -> Alternate Function
	GPIOA->AFR[0] |= 0x00007700;	//Alternate fuction is AF7 for USART2

	USART2->BRR |= 16000000 / 115200ul;		//Baud Rate = 115200 kbps (Clock freq is set 16 MHz default)
	USART2->CR2 |= 0x00000000;				//1 Stop bit
	USART2->CR1 |= 1ul << 0;					//USART1 Enable	
	USART2->CR1 |= 0x0000202C;				//USART Enable, RXNE Interrupt Enable, TX & Rx Enable
	NVIC_EnableIRQ(USART2_IRQn);
	////////////////////////////////////////
	//////////Using LEDs on the board//////////
	RCC->AHB1ENR |= 1ul << 3; 		//GPIOD Clock Enable
	GPIOD->MODER |= 0x55000000;		//PD12, PD13, PD14, and PD15 set as an output
	
	GPIOD->ODR |= 1ul << 12;
	
	bluetoothSendString(packet);
}

void bluetoothSendChar(uint8_t txByte) {
	while(!(USART2->SR & 0x00000080));  //TDR is busy?
	USART2->DR = txByte;
}

void bluetoothSendString(uint8_t *txPacket) {
	while(*txPacket) {
		bluetoothSendChar(*txPacket);
		txPacket++;
	}
}
