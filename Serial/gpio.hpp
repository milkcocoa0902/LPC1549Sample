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
		typedef enum Direction_e{
			DIRECTION_OUTPUT = 0,
			DIRECTION_INPUT,
		}Direction;

		struct Digital{
			port_t mPort;
			pin_t mPin;

			void Toggle()const;
			const Digital& Open()const;
			void operator()(bool)const;
			bool operator()()const;
			const Digital& operator()(Direction)const;
			const Digital& operator()(CHIP_SWM_PIN_MOVABLE_T)const;
			const Digital& operator()(CHIP_SWM_PIN_FIXED_T)const;
			//設定用関数

		private:
			void Set(bool)const;
			bool Get()const;
			const Digital& Din()const;
			const Digital& Dout()const;
			const Digital& Move(CHIP_SWM_PIN_MOVABLE_T)const;
			const Digital& Fix(CHIP_SWM_PIN_FIXED_T)const;
		};

		struct Analog{
			port_t mPort;
			pin_t mPin;
			module_t mModule;
			id_t mId;

			const Analog& operator()(CHIP_SWM_PIN_FIXED_T)const;
			Digital operator ()()const{
				return Digital{mPort, mPin};
			}

		private:
			const Analog& Init(CHIP_SWM_PIN_FIXED_T)const;
			Digital AsDigital()const{
				return Digital{mPort, mPin};
			}
		};
	}
}



#endif /* DRIVER_GPIO_HPP_ */
