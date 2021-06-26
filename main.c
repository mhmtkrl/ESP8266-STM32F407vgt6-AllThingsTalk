#include "stm32f4xx.h"                  // Device header
#include "bluetoothUARTdriver.h"
#include "esp8266UARTdriver.h"
#include "delay.h"
#include <stdio.h>

#define WIFI_SSID 				"MyHotspot"
#define WIFI_PASSWORD 		""

#define DESTINATION_IP		"20.61.15.37"
#define DESTINATION_PORT	8891
#define LOCAL_PORT				5555

uint8_t packet[] = "*****STM32F407vgt6 UDP Example - AllThingsTalk*****\r\n";
uint8_t cmdTest[] = "AT\r\n";
uint8_t cmdCheckVersionNumber[] = "AT+GMR\r\n";
uint8_t cmdEspWIFIMode[] = "AT+CWMODE=1\r\n";			//Station Mode
uint8_t cmdListAvailabelAPs[] = "AT+CWLAP\r\n";			//List Available Access Points
uint8_t cmdDisconnectFromAP[] = "AT+CWQAP\r\n";			//List Available Access Points
uint8_t cmdConnectAP[] = "";

uint8_t cmdCreateUDPsocket[] = "";

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
	
	sprintf((char *)cmdCreateUDPsocket, "AT+CIPSTART=\"UDP\",\"%s\",%d,%d\r\n", DESTINATION_IP, DESTINATION_PORT, LOCAL_PORT);
	esp8266SendPacket(cmdCreateUDPsocket);
	delayMs(2000);

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

