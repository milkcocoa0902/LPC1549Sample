/*
 * systick.cpp
 *
 *  Created on: 2017/06/06
 *      Author: Keita
 */


#include "systick.hpp"
#include <chip.hpp>

namespace Driver{
	namespace Tick{
		volatile static uint64_t tick;
		static constexpr uint64_t freq = 100000; // T = 10[us]. 過負荷かもしれないからもしかしたら分解能下げる
		static uint64_t unUsedValue;

		void Init(){
			SysTick_Config(SystemCoreClock / freq);
			tick = 0;
		}

		void ClearTick(){
			tick = 0;
		}

		static inline auto CurrentTick(){
			return tick;
		}

		static inline auto CurrentTickUs(){
			return CurrentTick();
		}

		static inline auto CurrentTickMs(){
			return CurrentTick() / 100;
		}

		void __DelayUs(const uint64_t _us){
			ClearTick();
			while(CurrentTickUs() <= (_us / 10)){
				// __NOP()だとCPUが止まるから
				unUsedValue++;
			}
		}

		void DelayMs(const uint64_t _ms){
			ClearTick();
			while(CurrentTickMs() < _ms) {
				unUsedValue++;
			}
		}

		void Delay(const uint64_t _sec){
			auto time = _sec;
			while(time > 0){
				DelayMs(1000);
				time--;
			}
		}

		extern "C" void SysTick_Handler(){
			tick++;
		}
	}
}
