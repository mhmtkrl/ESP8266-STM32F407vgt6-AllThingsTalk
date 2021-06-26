#include "stm32f4xx.h"                  // Device header
#include "bluetoothUARTdriver.h"
#include "esp8266UARTdriver.h"
#include "delay.h"

uint8_t packet[] = "*****STM32F407vgt6 UDP Example - AllThingsTalk*****\r\n";
uint8_t cmd[] = "AT\r\n";

int main() {
	bluetoothInit();	//HC05 Init
	esp8266Init();		//ESP8266 Init
	//////////Using LEDs on the board//////////
	RCC->AHB1ENR |= 1ul << 3; 		//GPIOD Clock Enable
	GPIOD->MODER |= 0x55000000;		//PD12, PD13, PD14, and PD15 set as an output
	GPIOD->ODR |= 1ul << 12;
	//////////Send Test Text and Command//////////
	bluetoothSendPacket(packet);
	esp8266SendPacket(cmd);
	delayMs(400);

	while(1) {
		GPIOD->ODR |= 1ul << 14;
		delayMs(1000);
		GPIOD->ODR &= ~(1ul << 14);
		delayMs(1000);
	}
}

void USART3_IRQHandler() {
	if((USART3->SR & (1ul << 5))) {
		bluetoothSendByte(USART3->DR);
		USART3->SR &= ~(1ul << 5);
	}
}

