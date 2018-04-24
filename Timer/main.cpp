/*
 * main.cpp
 *
 *  Created on: 2017/06/21
 *      Author: Keita
 */

#include "chip.hpp"
#include "driver/DigitalIO.hpp"
#include "driver/timer.hpp"

const uint32_t OscRateIn = 12000000;
const uint32_t RTCOscRateIn = 32768;

// LED2 -> PIO0_10
Driver::GPIO::Digital LED2{0, 10};

void LED2Blink(){
	LED2.Toggle();
}

int main(){
	// クロックソースを水晶として起動
	Chip_SetupXtalClocking();
	SystemCoreClockUpdate();

	// LED3 -> PIO0_11
	Driver::GPIO::Digital LED3{0, 11};

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


