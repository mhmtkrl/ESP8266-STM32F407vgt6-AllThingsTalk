#include "stm32f4xx.h"                  // Device header

void esp8266Init(void);
void esp8266SendByte(uint8_t);
void esp8266SendPacket(uint8_t *txPacket);
void USART3_IRQHandler(void);
