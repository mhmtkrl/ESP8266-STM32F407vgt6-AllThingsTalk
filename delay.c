#include "delay.h"

void delayMs(uint16_t delay) {
	uint32_t time = 0;
	time = delay * 4000;
	while(time > 0) time--;
}
