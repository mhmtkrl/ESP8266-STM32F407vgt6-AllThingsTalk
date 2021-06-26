#include "esp8266UARTdriver.h"

void esp8266Init(void) {
	RCC->AHB1ENR |= 1ul << 1;			//GPIOB Clock Enable
	RCC->APB1ENR |= 1ul << 18;	//USART3 Clock Enable
	
	GPIOB->MODER|=0x2AA00000;	//PB10 and PB11 -> Alternate Function
	GPIOB->AFR[1] |= 0x00007700;	//Alternate fuction is AF7 for USART3

	USART3->BRR |= 16000000 / 115200ul;		//Baud Rate = 115200 kbps (Clock freq is set 16 MHz default)
	USART3->CR2 |= 0x00000000;				//1 Stop bit
	USART3->CR1 |= 1ul << 0;					//USART1 Enable	
	USART3->CR1 |= 0x0000202C;				//USART Enable, RXNE Interrupt Enable, TX & Rx Enable
	NVIC_EnableIRQ(USART3_IRQn);
}

void esp8266SendByte(uint8_t txByte) {
	while(!(USART3->SR & 0x00000080));  //TDR is busy?
	USART3->DR=txByte;
}

void esp8266SendPacket(uint8_t *txPacket) {
	while(*txPacket) {
		esp8266SendByte(*txPacket);
		txPacket++;
	}
}
