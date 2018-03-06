/*
 * gpio.cpp
 *
 *  Created on: 2017/03/07
 *      Author: Keita
 */

#include "DigitalIO.hpp"

#include <chip.hpp>


namespace Driver{
	namespace GPIO{
		Digital::Digital(const uint8_t _port, const uint8_t _pin):
				port(_port),
				pin(_pin){

		}

		void Digital::Toggle() const{
			Chip_GPIO_SetPinToggle(LPC_GPIO, port, pin);
		}

		uint8_t Digital::port_()const{
			return port;
		}
		uint8_t Digital::pin_()const{
			return pin;
		}

		const Digital& Digital::Open()const {
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
			IOCON_ADMODE_EN | IOCON_MODE_PULLDOWN); //アナログ化することで解放にする。
			return *this;
		}

		const Digital& Digital::operator()(const bool _flag) const{
			Chip_GPIO_SetPinState(LPC_GPIO, port, pin, _flag);
			return *this;
		}

		void Digital::operator<<(const bool _flag)const{
			Chip_GPIO_SetPinState(LPC_GPIO, port, pin, _flag);
		}

		bool Digital::operator()()const{
			return Chip_GPIO_GetPinState(LPC_GPIO, port, pin);
		}

		void Digital::operator>>(bool _in)const{
			_in = Chip_GPIO_GetPinState(LPC_GPIO, port, pin);
		}

		const Digital& Digital::operator()(const Direction _dir)const{
			Chip_GPIO_SetPinDIR(LPC_GPIO, port, pin, (bool)_dir);
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, IOCON_DIGMODE_EN);

			return *this;
		}

		const Digital& Digital::operator()(const std::vector<Option> _option)const{
			uint32_t options = 0;
			for(opt : _option){
				options |= (uint32_t)opt;
			}

			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
					IOCON_DIGMODE_EN | options);
			return *this;
		}

		const Digital& Digital::operator()(const CHIP_SWM_PIN_MOVABLE_T _func)const{
			Chip_SWM_MovablePortPinAssign(_func, port, pin);
			return *this;
		}

		const Digital& Digital::operator()(const CHIP_SWM_PIN_FIXED_T _fix)const{
			Chip_SWM_FixedPinEnable(_fix, true);
			return *this;
		}
	}
}

