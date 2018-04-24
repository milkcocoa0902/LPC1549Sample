/*
 * Timer.hpp
 *
 *  Created on: 2017/03/07
 *      Author: Keita
 */

#ifndef DRIVER_TIMER_HPP_
#define DRIVER_TIMER_HPP_

#include <chip.hpp>
#include "functional"
#include "../util/type.hpp"
#include "stdint.h"
#include <array>

namespace Driver{
	enum TimerNumber{
		TIMER_SYSTEM = 0,
		TIMER_ADC  = 1,
		TIMER_RUNNING = 2,
		TIMER_UNUSED = 3

	};
	class Timer{
	private:
		static std::array<Util::CallBack, MRT_CHANNELS_NUM> CallBack;
		uint32_t ch;
	public:
		Timer() = default;
		Timer(const uint32_t _ch);
		virtual ~Timer();
		const Timer& SetCallback(const uint32_t _hz, const Util::CallBackRef _handler = nullptr);
		const Timer& SetCallback(const uint32_t _hz, const Util::CallBackRRef _handler);
		static Util::CallBackRef GetCallback(const int _ch);
		const Timer& Start();
		const Timer& Stop();

	};
}

#endif /* DRIVER_TIMER_HPP_ */
