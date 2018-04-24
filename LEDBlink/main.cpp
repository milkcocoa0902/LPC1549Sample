/*
 * main.cpp
 *
 *  Created on: 2017/06/21
 *      Author: Keita
 */

#include "chip.hpp"
#include "driver/DigitalIO.hpp"
#include "driver/systick.hpp"

const uint32_t OscRateIn = 12000000;
const uint32_t RTCOscRateIn = 32768;

int main(){
	// クロックソースを水晶として起動
	Chip_SetupXtalClocking();
	SystemCoreClockUpdate();

	// システムタイマの初期化(delay用)
	Driver::Tick::Init();

	// LED1 -> P0_18, LED2 -> P0_10 ,LED3 -> P0_11
	Driver::GPIO::Digital LED1{0, 18}, LED2{0, 10}, LED3{0, 11};

	// LEDピンの入出力方向
	LED1(Driver::GPIO::Direction::Out)(true);
	LED2(Driver::GPIO::Direction::Out)(false);
	LED3(Driver::GPIO::Direction::Out)(false);

	Driver::GPIO::Digital Switch{0, 4};
	Switch(Driver::GPIO::Direction::In);

	volatile static unsigned int cnt = 0;

	while(1){
		switch((cnt++) % 3){
		case 0:
			LED1(true);
			LED2(false);
			LED3(false);
			break;
		case 1:
			LED1(false);
			LED2(true);
			LED3(false);
			break;
		case 2:
			LED1(false);
			LED2(false);
			LED3(true);
			break;
		}
		Driver::Tick::DelayMs(200);
		while(!Switch());
	}
}


