/*
 * main.cpp
 *
 *  Created on: 2017/06/21
 *      Author: Keita
 */

#include "chip.hpp"
#include "gpio.hpp"
#include "timer.hpp"

const uint32_t OscRateIn = 12000000;
const uint32_t RTCOscRateIn = 32768;

// LED2 -> P1_8
Driver::GPIO::Digital LED2{1, 4};

void LED2Blink(){
	LED2.Toggle();
}

int main(){
	// クロックソースを水晶として起動
	Chip_SetupXtalClocking();
	SystemCoreClockUpdate();

	// スイッチマトリクスモジュールにクロックを供給
	Chip_SWM_Init();

	// GPIOモジュールにクロックを供給
	Chip_GPIO_Init(LPC_GPIO);

	// タイマモジュールの初期化
	Driver::Timer::Init();

	// LED3 -> P1_5
	Driver::GPIO::Digital LED3{1, 5};

	// LEDピンの入出力方向
	LED2(Driver::GPIO::DIRECTION_OUTPUT)(true);
	LED3(Driver::GPIO::DIRECTION_OUTPUT)(false);

	// コールバック関数を登録する
	Driver::Timer::SetCallback(0, 5, LED2Blink);

	// Timerのコールバックにはラムダ式も登録できる
	Driver::Timer::SetCallback(1, 5, [&LED3](){LED3.Toggle();});

	while(1);
}


