#include "stm32f4xx.h"                  // Device header

//////////Function Declaration//////////
void bluetoothSendByte(uint8_t);
void bluetoothSendPacket(uint8_t *);
void esp8266SendByte(uint8_t);
void esp8266SendPacket(uint8_t *txPacket);
void USART3_IRQHandler(void);
void delayMs(uint16_t);
////////////////////////////////////////
uint8_t packet[] = "*****STM32F407vgt6 UDP Example - AllThingsTalk*****\r\n";
uint8_t cmd[] = "AT\r\n";

int main() {
	//////////HC05 Init//////////
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
	//////////ESP8266 Init//////////
	RCC->AHB1ENR |= 1ul << 1;			//GPIOB Clock Enable
	RCC->APB1ENR |= 1ul << 18;	//USART3 Clock Enable
	
	GPIOB->MODER|=0x2AA00000;	//PB10 and PB11 -> Alternate Function
	GPIOB->AFR[1] |= 0x00007700;	//Alternate fuction is AF7 for USART3

	USART3->BRR |= 16000000 / 115200ul;		//Baud Rate = 115200 kbps (Clock freq is set 16 MHz default)
	USART3->CR2 |= 0x00000000;				//1 Stop bit
	USART3->CR1 |= 1ul << 0;					//USART1 Enable	
	USART3->CR1 |= 0x0000202C;				//USART Enable, RXNE Interrupt Enable, TX & Rx Enable
	NVIC_EnableIRQ(USART3_IRQn);
	////////////////////////////////////////
	//////////Using LEDs on the board//////////
	RCC->AHB1ENR |= 1ul << 3; 		//GPIOD Clock Enable
	GPIOD->MODER |= 0x55000000;		//PD12, PD13, PD14, and PD15 set as an output
	GPIOD->ODR |= 1ul << 12;
	////////////////////////////////////////
	//////////Send Test Text and Command//////////
	bluetoothSendPacket(packet);
	esp8266SendPacket(cmd);
	delayMs(400);
	////////////////////////////////////////
	while(1) {
		GPIOD->ODR |= 1ul << 14;
		delayMs(400);
		GPIOD->ODR &= ~(1ul << 14);
		delayMs(400);
	}
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

void USART3_IRQHandler() {
	if((USART3->SR & (1ul << 5))) {
		bluetoothSendByte(USART3->DR);
		USART3->SR &= ~(1ul << 5);
	}
}

void delayMs(uint16_t delay) {
	uint32_t time = 0;
	time = delay * 4000;
	while(time > 0) time--;
}
