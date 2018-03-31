/*
 * main.cpp
 *
 *  Created on: 2017/06/21
 *      Author: Keita
 */

#include "chip.hpp"
#include "DigitalIO.hpp"
#include "PWM.hpp"

const uint32_t OscRateIn = 12000000;
const uint32_t RTCOscRateIn = 32768;

int main(){
	// クロックソースを水晶として起動
	Chip_SetupXtalClocking();
	SystemCoreClockUpdate();

	// GPIO0_9をPWM0として設定
	Driver::PWM pwm({0, 13}, Driver::PWM::PWMCh::Ch0);

	// duty比を40%に
	pwm(40);

	while(1);
}
