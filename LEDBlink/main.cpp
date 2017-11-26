/*
 * main.cpp
 *
 *  Created on: 2017/06/21
 *      Author: Keita
 */

#include "chip.hpp"
#include "gpio.hpp"
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

	// システムタイマの初期化(delay用)
	Driver::Tick::Init();

	// LED1 -> P1_8, LED2 -> P1_4 ,LED3 -> P1_5
	Driver::GPIO::Digital LED1{1, 8}, LED2{1, 4}, LED3{1, 5};

	// LEDピンの入出力方向
	LED2(Driver::GPIO::DIRECTION_OUTPUT)(true);
	LED3(Driver::GPIO::DIRECTION_OUTPUT)(false);

	while(1){
		LED2.Toggle();
		LED3.Toggle();
		Driver::Tick::DelayMs(200);
	}
}


