#include "stm32f4xx.h"                  // Device header

void bluetoothInit(void);
void bluetoothSendByte(uint8_t);
void bluetoothSendPacket(uint8_t *);
