/*
 * gpio.hpp
 *
 *  Created on: 2017/03/07
 *      Author: Keita
 */

#ifndef DRIVER_GPIO_HPP_
#define DRIVER_GPIO_HPP_

#include <chip.hpp>
#include <stdint.h>

namespace Driver{

	using port_t = uint8_t;
	using pin_t = uint8_t;
	using module_t = uint8_t;
	using id_t = uint8_t;

	namespace GPIO{
		enum class Direction:bool{
			Out = false,
			In,
		};

		class Digital{
		public:
			port_t port;
			pin_t pin;

			Digital() = default;
			Digital(const uint8_t _port, const uint8_t _pin);
			Digital(const Digital&) = default;
			void Toggle()const;
			uint8_t port_()const;
			uint8_t pin_()const;
			const Digital& Open()const;
			void operator()(const bool)const;
			void operator<<(const bool)const;
			bool operator()()const;
			void operator>>(bool)const;
			const Digital& operator()(const Direction)const;
			const Digital& operator()(const CHIP_SWM_PIN_MOVABLE_T)const;
			const Digital& operator()(const CHIP_SWM_PIN_FIXED_T)const;
			//設定用関数
		};
	}
}



#endif /* DRIVER_GPIO_HPP_ */
