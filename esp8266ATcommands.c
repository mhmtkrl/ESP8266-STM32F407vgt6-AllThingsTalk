#include "esp8266ATcommands.h"
#include "esp8266UARTdriver.h"
#include "delay.h"
#include <stdio.h>

ESP8266_COMMANDS esp8266Commands = {"AT\r\n", 						//Test Command
																		"AT+GMR\r\n",					//Device Version
																		"AT+CWMODE=1\r\n",
																		"AT+CWLAP\r\n",
																		"AT+CWQAP\r\n",
																		"",
																		"AT+CIFSR\r\n",
																		"AT+CIPCLOSE\r\n",
																		"",
																		"",
																		0,
																		"",
																		80};

void TestCommand(void) {
	esp8266SendPacket(esp8266Commands.espTestCommand);
	delayMs(800);
	esp8266SendPacket(esp8266Commands.espCheckVersionNumber);
	delayMs(800);
}

void ConnectHotspot(uint8_t ssid[], uint8_t psd[]) {
	esp8266SendPacket(esp8266Commands.espEspWIFIMode);
	delayMs(800);
	esp8266SendPacket(esp8266Commands.espDisconnectFromAP);
	delayMs(2000);
	esp8266SendPacket(esp8266Commands.espListAvailabelAPs);
	delayMs(5000);
	sprintf((char *)esp8266Commands.espConnectAP, "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, psd); 
	esp8266SendPacket(esp8266Commands.espConnectAP);
	delayMs(8000);
	esp8266SendPacket(esp8266Commands.espGetIPAddress);
	delayMs(5000);
}

void CreateUDPSocket(uint8_t destination_ip[], uint32_t destination_port, uint32_t local_port) {
	esp8266SendPacket(esp8266Commands.espCloseUDPsocket);
	delayMs(800);
	sprintf((char *)esp8266Commands.espCreateUDPsocket, "AT+CIPSTART=\"UDP\",\"%s\",%d,%d\r\n", destination_ip, destination_port, local_port);
	esp8266SendPacket(esp8266Commands.espCreateUDPsocket);
	delayMs(800);
}

void sendUDPData(uint8_t assetName[], uint8_t data) {
	esp8266Commands.espUDP_PACKET_LENGTH = sprintf((char *)esp8266Commands.espUDP_PACKET, "%s\n%s\n{\"%s\":{\"value\":%d}}\r\n", DEVICE_ID, DEVICE_TOKENS, assetName, data);
	sprintf((char *)esp8266Commands.espSendUDPdata, "AT+CIPSEND=%d\r\n", esp8266Commands.espUDP_PACKET_LENGTH);
	esp8266SendPacket(esp8266Commands.espSendUDPdata);
	delayMs(400);
	esp8266SendPacket(esp8266Commands.espUDP_PACKET);
	delayMs(800);
}
