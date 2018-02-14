#include <stm32f4xx.h>
#include <stm32f4xx_rcc.h>
#include <Timer.h>

void TIM_Delay_Init(){
	RCC_ClocksTypeDef RCC_Clocks;

	// Use internal clock to scale delays
	RCC_GetClocksFreq(&RCC_Clocks);
	__multiplier = RCC_Clocks.HCLK_Frequency / 4000000;
}

void TIM_Delay_Micro(uint32_t msecs) {
	uint32_t millis = 1000*msecs*__multiplier - 10;
	while(millis--) {}; // delay happens here
}
