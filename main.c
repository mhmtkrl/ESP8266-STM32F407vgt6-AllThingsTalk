#include "stm32f4xx.h"                  // Device header
#include "bluetoothUARTdriver.h"
#include "esp8266UARTdriver.h"
#include "delay.h"

uint8_t packet[] = "*****STM32F407vgt6 UDP Example - AllThingsTalk*****\r\n";
uint8_t cmdTest[] = "AT\r\n";
uint8_t cmdCheckVersionNumber[] = "AT+GMR\r\n";
uint8_t cmdEspWIFIMode[] = "AT+CWMODE=1\r\n";			//Station Mode
uint8_t cmdListAvailabelAPs[] = "AT+CWLAP\r\n";			//List Available Access Points
uint8_t cmdConnectAP[] = "AT+CWJAP=\"MyHotspot\",\"\"\r\n";			//Connect to AP -> SSID: MyHotspot, Password: None

int main() {
	bluetoothInit();	//HC05 Init
	esp8266Init();		//ESP8266 Init
	//////////Using LEDs on the board//////////
	RCC->AHB1ENR |= 1ul << 3; 		//GPIOD Clock Enable
	GPIOD->MODER |= 0x55000000;		//PD12, PD13, PD14, and PD15 set as an output
	GPIOD->ODR |= 1ul << 12;
	//////////Send Test Text and Command//////////
	bluetoothSendPacket(packet);
	
	esp8266SendPacket(cmdTest);
	delayMs(400);
	esp8266SendPacket(cmdCheckVersionNumber);
	delayMs(400);
	esp8266SendPacket(cmdEspWIFIMode);
	delayMs(400);
	esp8266SendPacket(cmdListAvailabelAPs);
	delayMs(5000);
	esp8266SendPacket(cmdConnectAP);
	delayMs(800);

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

