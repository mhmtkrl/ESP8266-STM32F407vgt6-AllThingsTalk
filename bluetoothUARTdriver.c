#include "bluetoothUARTdriver.h"

void bluetoothInit(void) {
	RCC->AHB1ENR |= 1ul << 0;			//GPIOA Clock Enable
	RCC->APB1ENR|= 1ul << 17;   	//USART2 Clock Enable
	
	GPIOA->MODER |=0xA80000A0;		//PA2 and PA3 -> Alternate Function
	GPIOA->AFR[0] |= 0x00007700;	//Alternate fuction is AF7 for USART2

	USART2->BRR |= 16000000 / 115200ul;		//Baud Rate = 115200 kbps (Clock freq is set 16 MHz default)
	USART2->CR2 |= 0x00000000;				//1 Stop bit
	USART2->CR1 |= 1ul << 0;					//USART1 Enable	
	USART2->CR1 |= 0x0000202C;				//USART Enable, RXNE Interrupt Enable, TX & Rx Enable
	NVIC_EnableIRQ(USART2_IRQn);
}

void bluetoothSendByte(uint8_t txByte) {
	while(!(USART2->SR & 0x00000080));  //TDR is busy?
	USART2->DR = txByte;
}

void bluetoothSendPacket(uint8_t *txPacket) {
	while(*txPacket) {
		bluetoothSendByte(*txPacket);
		txPacket++;
	}
}
