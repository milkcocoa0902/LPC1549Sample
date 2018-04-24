/*
 * PWM.cpp
 *
 *  Created on: 2017/11/04
 *      Author: Keita
 */

#include "PWM.hpp"

namespace Driver {
	std::array<uint32_t, 4> PWM::freq = {0, 0, 0, 0};
	std::array<LPC_SCT_T*, 4> PWM::PwmBase = {LPC_SCT0, LPC_SCT1, LPC_SCT2, LPC_SCT3};

	PWM::PWM(const std::pair<uint8_t, uint8_t> _pin, const PWMCh _ch, const uint32_t _freq):
		pin(Driver::GPIO::Digital(_pin.first, _pin.second)){
		// TODO Auto-generated constructor stub


		// 与えられたPWMChからmoduleとchを割り出す
		module = ((int)_ch >> 4);
		ch = ((int)_ch & 0x0F);

		// 初期化するのは最初のインスタンス生成時のみ
		// 周波数は最初のインスタンスに依存
		if(freq[module] == 0){
			Chip_SCTPWM_Init(PwmBase[module]);
			Chip_SCTPWM_SetRate(PwmBase[module], _freq);
		}


		Chip_SCTPWM_SetOutPin(PwmBase[module], ch + 1, ch);
		Chip_SCTPWM_SetDutyCycle(PwmBase[module], ch + 1, 0);

		// 割り当てられたピンとチャンネルに応じてSwitch Matrixを解決する
		if(ch < 3){
			// SCT[0:3]_OUT[0:2]
			// この出力はどのピンにも割り当てられる
			int swm = SWM_SCT0_OUT0_O;
			for(int i = 0;i < module * 3 + ch;i++){
				swm++;
				if((swm & 0x0F) == 0x04){
					//swm -= 0x04;swm += 0x10;
					swm += 0x0c;
				}
			}
			pin(GPIO::Direction::Out)((CHIP_SWM_PIN_MOVABLE_T)(swm));
		}else if(ch < 8){
			// SCT[0:3]_OUT[3:7]
			// このピンは割り当てを固定されている
			auto add = (ch - 3) + (module == 3 ? 13 : module * 5);
			pin(GPIO::Direction::Out)((CHIP_SWM_PIN_FIXED_T)(SWM_FIXED_SCT0_OUT3 + add));
		}

		if(freq[module] == 0){
			Chip_SCTPWM_Start(PwmBase[module]);
			freq[module] = _freq;
		}
	}

	PWM::~PWM() {
		// TODO Auto-generated destructor stub
		Chip_SCTPWM_SetDutyCycle(PwmBase[module], ch + 1, 0);
	}

	void PWM::SetFreq(const uint32_t _freq){
		Chip_SCTPWM_SetRate(PwmBase[module], _freq);
		Chip_SCTPWM_Start(PwmBase[module]);
		freq[module] = _freq;
	}

	void PWM::SetDuty(const int8_t _duty)const{
		Chip_SCTPWM_SetDutyCycle(PwmBase[module], ch+1, Chip_SCTPWM_PercentageToTicks(PwmBase[module], _duty));
	}

	void PWM::operator()(const int8_t _duty)const{
		Chip_SCTPWM_SetDutyCycle(PwmBase[module], ch+1, Chip_SCTPWM_PercentageToTicks(PwmBase[module], _duty));
	}
} /* namespace Driver */
