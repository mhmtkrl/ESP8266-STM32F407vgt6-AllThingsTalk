#include "stm32f4xx.h"                  // Device header

#define DEVICE_ID					"uin5IqUVr2KbwPYa1KHyEbem"
#define DEVICE_TOKENS 		"maker:4ckc0qA4ONSsXv2yFItdB7f5eKzVKIclAdCphrEn"

void TestCommand(void);
void ConnectHotspot(char ssid[16], char psd[16]);
void CreateUDPSocket(char destination_ip[], uint32_t destination_port, uint32_t local_port);
void sendUDPData(char assetName[], uint8_t data);

typedef struct {
	uint8_t espTestCommand[64];
	uint8_t espCheckVersionNumber[64];
	uint8_t espEspWIFIMode[64];
	uint8_t espListAvailabelAPs[64];
	uint8_t espDisconnectFromAP[64];
	uint8_t espConnectAP[127];
	uint8_t espGetIPAddress[64];
	uint8_t espCloseUDPsocket[64];
	uint8_t espCreateUDPsocket[64];
	uint8_t espUDP_PACKET[127];
	uint8_t espUDP_PACKET_LENGTH;
	uint8_t espSendUDPdata[127];
	uint8_t espUDP_RAW_DATA;
}ESP8266_COMMANDS;


