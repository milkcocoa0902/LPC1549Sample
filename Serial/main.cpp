/*
 * main.cpp
 *
 *  Created on: 2017/06/23
 *      Author: Keita
 */


#include <chip.hpp>
#include "uart.hpp"
#include "gpio.hpp"

const uint32_t OscRateIn = 12000000;
const uint32_t RTCOscRateIn = 32768;

int main(){
	Chip_SetupXtalClocking();
	SystemCoreClockUpdate();
	Chip_SWM_Init();
	Chip_GPIO_Init(LPC_GPIO);
	Driver::Serial serial({0, 18}, {0, 13}, 0);


	while(1){
		if(!serial.IsEmpty()){
			if(serial.IsLine())
				serial << serial;
		}
	}
}

