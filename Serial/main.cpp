/*
 * main.cpp
 *
 *  Created on: 2017/06/23
 *      Author: Keita
 */


#include <chip.hpp>
#include "uart.hpp"
#include "DigitalIO.hpp"

const uint32_t OscRateIn = 12000000;
const uint32_t RTCOscRateIn = 32768;

int main(){
	// クロックソースを水晶として起動
	Chip_SetupXtalClocking();
	SystemCoreClockUpdate();

	Driver::Serial serial({0, 18}, {0, 13}, 0);

	while(1){
		if(!serial.IsEmpty()){
			serial << serial;
		}
	}
}

