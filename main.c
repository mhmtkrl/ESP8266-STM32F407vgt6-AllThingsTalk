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

uint8_t debugPacket[] = "*****STM32F407vgt6 UDP Example - AllThingsTalk*****\r\n";
uint8_t cmdTest[] = "AT\r\n";												//Test Command
uint8_t cmdCheckVersionNumber[] = "AT+GMR\r\n";			//Device Version
uint8_t cmdEspWIFIMode[] = "AT+CWMODE=1\r\n";			  //Station Mode
uint8_t cmdListAvailabelAPs[] = "AT+CWLAP\r\n";			//List Available Access Points
uint8_t cmdDisconnectFromAP[] = "AT+CWQAP\r\n";			//Disconnect from the AP
uint8_t cmdConnectAP[127];												  //Connect to the Hotspot/Access Point
uint8_t cmdGetIPAddress[] = "AT+CIFSR\r\n";				  //Get IP Address

uint8_t cmdCloseUDPsocket[] = "AT+CIPCLOSE\r\n";		//Close UDP socket
uint8_t cmdCreateUDPsocket[] = "";									//Create an UDP socket
uint8_t UDP_PACKET[127];														//UDP packet
uint16_t UDP_PACKET_LENGTH = 0;											//UDP packet length
uint8_t cmdSendUDPdata[127];												//Send UDP data

int main() {
	bluetoothInit();	//HC05 Init
	esp8266Init();		//ESP8266 Init
	//////////Using LEDs on the board//////////
	RCC->AHB1ENR |= 1ul << 3; 		//GPIOD Clock Enable
	GPIOD->MODER |= 0x55000000;		//PD12, PD13, PD14, and PD15 set as an output
	GPIOD->ODR |= 1ul << 12;
	//////////Send Test Text and Command//////////
	bluetoothSendPacket(debugPacket);
	
	esp8266SendPacket(cmdTest);
	delayMs(800);
	esp8266SendPacket(cmdCheckVersionNumber);
	delayMs(800);
	esp8266SendPacket(cmdEspWIFIMode);
	delayMs(800);
	esp8266SendPacket(cmdDisconnectFromAP);
	delayMs(2000);
	esp8266SendPacket(cmdListAvailabelAPs);
	delayMs(5000);
	sprintf((char *)cmdConnectAP, "AT+CWJAP=\"%s\",\"%s\"\r\n", WIFI_SSID, WIFI_PASSWORD); 
	esp8266SendPacket(cmdConnectAP);
	delayMs(8000);
	esp8266SendPacket(cmdGetIPAddress);
	delayMs(5000);
	
	esp8266SendPacket(cmdCloseUDPsocket);
	delayMs(800);
	sprintf((char *)cmdCreateUDPsocket, "AT+CIPSTART=\"UDP\",\"%s\",%d,%d\r\n", DESTINATION_IP, DESTINATION_PORT, LOCAL_PORT);
	esp8266SendPacket(cmdCreateUDPsocket);
	delayMs(800);
	UDP_PACKET_LENGTH = sprintf((char *)UDP_PACKET, "uin5IqUVr2KbwPYa1KHyEbem\nmaker:4ckc0qA4ONSsXv2yFItdB7f5eKzVKIclAdCphrEn\n{\"rawData\":{\"value\":98}}\r\n");
	sprintf((char *)cmdSendUDPdata, "AT+CIPSEND=%d\r\n", UDP_PACKET_LENGTH);
	esp8266SendPacket(cmdSendUDPdata);
	delayMs(400);
	esp8266SendPacket(UDP_PACKET);
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

