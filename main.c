#include "stm32f4xx.h"                  // Device header
#include "bluetoothUARTdriver.h"
#include "esp8266UARTdriver.h"
#include "delay.h"
#include <stdio.h>
#include "esp8266ATcommands.h"

#define WIFI_SSID 				"MyHotspot"
#define WIFI_PASSWORD 		""

#define DESTINATION_IP		"20.61.15.37"
#define DESTINATION_PORT	8891
#define LOCAL_PORT				5555

#define ASSET_NAME 				"rawData"

uint8_t debugPacket[] = "*****STM32F407vgt6 UDP Example - AllThingsTalk*****\r\n";
uint8_t testValue = 0;

int main() {
	bluetoothInit();	//HC05 Init
	esp8266Init();		//ESP8266 Init
	//////////Using LEDs on the board//////////
	RCC->AHB1ENR |= 1ul << 3; 		//GPIOD Clock Enable
	GPIOD->MODER |= 0x55000000;		//PD12, PD13, PD14, and PD15 set as an output
	//////////Send Test Text and Command//////////
	bluetoothSendPacket(debugPacket);
	
	TestCommand();
	ConnectHotspot(WIFI_SSID, WIFI_PASSWORD);
	CreateUDPSocket(DESTINATION_IP, DESTINATION_PORT, LOCAL_PORT);
	
	while(1) {
		sendUDPData(ASSET_NAME, testValue);
		testValue += 10;
		if(testValue > 100) testValue = 0;
	}
}

void USART3_IRQHandler() {
	if((USART3->SR & (1ul << 5))) {
		bluetoothSendByte(USART3->DR);
		USART3->SR &= ~(1ul << 5);
	}
}

