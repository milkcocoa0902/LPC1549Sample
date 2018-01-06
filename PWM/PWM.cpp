/*
 * PWM.cpp
 *
 *  Created on: 2017/11/04
 *      Author: Keita
 */

#include "PWM.hpp"

namespace Driver {
	uint32_t PWM::freq = 0;

	PWM::PWM(const std::pair<uint8_t, uint8_t> _pin, const uint8_t _ch, const uint32_t _freq):
		pin(Driver::GPIO::Digital(_pin.first, _pin.second)),
		ch(_ch){
		// TODO Auto-generated constructor stub

		// 初期化するのは最初のインスタンス生成時のみ
		// 周波数は最初のインスタンスに依存
		if(freq == 0){
			Chip_SCTPWM_Init(LPC_SCT1);
			Chip_SCTPWM_SetRate(LPC_SCT1, _freq);
		}


		Chip_SCTPWM_SetOutPin(LPC_SCT1, ch+1, ch);
		Chip_SCTPWM_SetDutyCycle(LPC_SCT1, ch+1, 0);

		// 割り当てられたピンとチャンネルに応じてSwitch Matrixを解決する
		if(ch < 3){
			// SCT1_OUT[0-2]
			// この出力はどのピンにも割り当てられる
			pin(GPIO::DIRECTION_OUTPUT)((CHIP_SWM_PIN_MOVABLE_T)(SWM_SCT1_OUT0_O + ch));
		}else if(ch < 8){
			// SCT1_OUT[3-7]
			// このピンは割り当てを固定されている
			pin(GPIO::DIRECTION_OUTPUT)((CHIP_SWM_PIN_FIXED_T)(SWM_FIXED_SCT1_OUT3 + ch - 3));
		}

		if(freq == 0){
			Chip_SCTPWM_Start(LPC_SCT1);
			freq = _freq;
		}
	}

	PWM::~PWM() {
		// TODO Auto-generated destructor stub
	}

	void PWM::SetFreq(const uint32_t _freq){
		Chip_SCTPWM_SetRate(LPC_SCT1, _freq);
		Chip_SCTPWM_Start(LPC_SCT1);
		freq = _freq;
	}

	void PWM::SetDuty(const int8_t _duty)const{
		Chip_SCTPWM_SetDutyCycle(LPC_SCT1, ch+1, Chip_SCTPWM_PercentageToTicks(LPC_SCT1, _duty));
	}

	void PWM::operator()(const int8_t _duty)const{
		Chip_SCTPWM_SetDutyCycle(LPC_SCT1, ch+1, Chip_SCTPWM_PercentageToTicks(LPC_SCT1, _duty));
	}
} /* namespace Driver */
