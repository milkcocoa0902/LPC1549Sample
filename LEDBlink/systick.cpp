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
		static constexpr uint64_t freq = 100000; // T = 10[us].
		volatile static uint32_t unUsedValue;
		volatile uint64_t upTime;

		void Init(){
			SysTick_Config(SystemCoreClock / freq);
			tick = 0;
			upTime = 0;
		}

		static inline void ClearTick(){
			tick = 0;
		}

		static inline uint64_t CurrentTick(){
			return tick;
		}

		static inline uint64_t CurrentTickUs(){
			return CurrentTick();
		}

		static inline uint64_t CurrentTickMs(){
			return CurrentTick() / 100;
		}

		void DelayUs(const uint64_t _us){
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

		inline uint64_t GetSystemTimeUs(){
			return upTime * 10;
		}

		inline uint64_t GetSystemTimeMs(){
			return GetSystemTimeUs() / 1000;
		}

		inline uint64_t GetSystemTimeSec(){
			return GetSystemTimeMs() / 1000;
		}

		inline uint64_t GetSystemTimeMin(){
			return GetSystemTimeSec() / 1000;
		}

		extern "C" void SysTick_Handler(){
			tick++;
			upTime++;
		}
	}
}
