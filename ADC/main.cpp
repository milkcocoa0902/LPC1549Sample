/*
 * main.cpp
 *
 *  Created on: 2018/02/09
 *      Author: Keita
 */


#include "chip.hpp"
#include "AnalogIn.hpp"
#include "systick.hpp"

const uint32_t OscRateIn = 12000000;
const uint32_t RTCOscRateIn = 32768;

int main(){
	// クロックソースを水晶として起動
	Chip_SetupXtalClocking();
	SystemCoreClockUpdate();

	// スイッチマトリクスモジュールにクロックを供給
	Chip_SWM_Init();

	// GPIOモジュールにクロックを供給
	Chip_GPIO_Init(LPC_GPIO);

	Driver::GPIO::Analog analog(0, 13, 1, 6);
	uint32_t result;

	while(1){
		result = analog;
		Driver::Tick::DelayMs(500);
	}
}

