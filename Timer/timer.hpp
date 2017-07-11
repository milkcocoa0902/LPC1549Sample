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

namespace Driver{
	namespace Timer{

		//コールバック関数の型
		using CallBack_t = std::function<void(void)>;
		using CallBackRef_t = const std::function<void(void)>&;
		using CallBackRRef_t = std::function<void(void)>&&;

		void Init();
		void SetCallback(uint8_t _ch, uint32_t _hz, CallBackRef_t _handler = nullptr);
		void SetCallback(uint8_t _ch, uint32_t _hz, CallBackRRef_t _handler);
		void Finish();
	}
}

#endif /* DRIVER_TIMER_HPP_ */
