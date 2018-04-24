/*
 * Timer.cpp
 *
 *  Created on: 2017/03/07
 *      Author: Keita
 */

#include "timer.hpp"
#include "array"

namespace Driver{
	std::array<Util::CallBack, MRT_CHANNELS_NUM> Timer::CallBack;
	Timer::Timer(const uint32_t _ch):ch(_ch){
		/* Enable the clock to the register interface */
		Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_MRT);

		Chip_MRT_SetDisabled(Chip_MRT_GetRegPtr(ch));
		CallBack[ch] = nullptr;

		NVIC_EnableIRQ(MRT_IRQn);
	}

	const Timer& Timer::SetCallback(const uint32_t _hz, const Util::CallBackRef _callback) {

		auto* p = Chip_MRT_GetRegPtr(ch);
		CallBack[ch] = _callback;

		if (CallBack[ch] != nullptr) {
			//Chip_MRT_SetDisabled(p); //一回動作を止める。
			Stop();
			Chip_MRT_SetInterval(p, (SystemCoreClock / _hz) | MRT_INTVAL_LOAD); //強制書き換え
			Chip_MRT_SetMode(p, MRT_MODE_REPEAT);
			//Chip_MRT_SetEnabled(p);
		} else {
			//Chip_MRT_SetDisabled(p);
			Stop();
		}
		return *this;
	}

	const Timer& Timer::SetCallback(const uint32_t _hz, const Util::CallBackRRef _callback) {

		auto* p = Chip_MRT_GetRegPtr(ch);

		if ((CallBack[ch] = move(_callback)) != nullptr) {
			//Chip_MRT_SetDisabled(p); //一回動作を止める。
			Stop();
			Chip_MRT_SetInterval(p, (SystemCoreClock / _hz) | MRT_INTVAL_LOAD); //強制書き換え
			Chip_MRT_SetMode(p, MRT_MODE_REPEAT);
			//Chip_MRT_SetEnabled(p);
		} else {
			Stop();
			//Chip_MRT_SetDisabled(p);
		}
		return *this;
	}

	const Timer& Timer::Start(){
		Chip_MRT_SetEnabled(Chip_MRT_GetRegPtr(ch));
		return *this;
	}

	const Timer& Timer::Stop(){
		Chip_MRT_SetDisabled(Chip_MRT_GetRegPtr(ch));
		return *this;
	}

	Timer::~Timer(){
		//Chip_MRT_SetDisabled(Chip_MRT_GetRegPtr(ch));
		Stop();
		CallBack[ch] = nullptr;

	}

	Util::CallBackRef Timer::GetCallback(const int _ch){
		return CallBack[_ch];
	}

	extern "C" void MRT_IRQHandler(void) {
		uint32_t int_pend;
		/* Get and clear interrupt pending status for all timers */
		int_pend = Chip_MRT_GetIntPending();

		Chip_MRT_ClearIntPending(int_pend);

		for (int ch = 0; ch < MRT_CHANNELS_NUM; ch++) {
			if (int_pend & MRTn_INTFLAG(ch)) {
				auto& func = Timer::GetCallback(ch);
				if (func != nullptr)
					func();
			}
		}
	}
}
