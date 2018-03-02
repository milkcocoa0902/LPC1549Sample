/*
 * main.cpp
 *
 *  Created on: 2017/06/21
 *      Author: Keita
 */

#include "chip.hpp"
#include "DigitalIO.hpp"
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


	// LED3 -> P1_5
	Driver::GPIO::Digital LED3{1, 5};

	// LEDピンの入出力方向
	LED2(Driver::GPIO::Direction::Out)(true);
	LED3(Driver::GPIO::Direction::Out)(false);

	// コールバック関数を登録する
	Driver::Timer timer0(0);
	timer0.SetCallback(5, LED2Blink);

	// Timerのコールバックにはラムダ式も登録できる
	Driver::Timer timer1(1);
	timer1.SetCallback(5, [LED3]{LED3.Toggle();});

	timer0.Start();
	timer1.Start();

	while(1);
}


