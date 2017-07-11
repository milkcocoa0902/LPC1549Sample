/*
 * gpio.cpp
 *
 *  Created on: 2017/03/07
 *      Author: Keita
 */

#include "gpio.hpp"
#include <chip.hpp>


namespace Driver{
	namespace GPIO{
		void Digital::Set(bool _flag)const {
			Chip_GPIO_SetPinState(LPC_GPIO, mPort, mPin, _flag);
		}

		bool Digital::Get() const{
			return Chip_GPIO_GetPinState(LPC_GPIO, mPort, mPin);
		}

		void Digital::Toggle() const{
			Chip_GPIO_SetPinToggle(LPC_GPIO, mPort, mPin);
		}

		const Digital& Digital::Din()const {
			Chip_IOCON_PinMuxSet(LPC_IOCON, mPort, mPin,
			IOCON_DIGMODE_EN | IOCON_MODE_PULLUP);
			Chip_GPIO_SetPinDIRInput(LPC_GPIO, mPort, mPin);
			return *this;
		}

		const Digital& Digital::Dout()const {
			Chip_IOCON_PinMuxSet(LPC_IOCON, mPort, mPin,
			IOCON_MODE_INACT | IOCON_DIGMODE_EN);
			Chip_GPIO_SetPinDIROutput(LPC_GPIO, mPort, mPin);
			Chip_GPIO_SetPinState(LPC_GPIO, mPort, mPin, true);
			return *this;
		}

		const Digital& Digital::Open()const {
			Chip_IOCON_PinMuxSet(LPC_IOCON, mPort, mPin,
			IOCON_ADMODE_EN | IOCON_MODE_PULLDOWN); //アナログ化することで解放にする。
			return *this;
		}

		const Digital& Digital::Move(CHIP_SWM_PIN_MOVABLE_T _func)const{
			Chip_SWM_MovablePortPinAssign(_func, mPort, mPin);
			return *this;
		}

		const Digital& Digital::Fix(CHIP_SWM_PIN_FIXED_T _fix)const{
			Chip_SWM_FixedPinEnable(_fix, true);
			return *this;
		}

		void Digital::operator()(bool _flag) const{
			this->Set(_flag);
		}

		bool Digital::operator()()const{
			return this->Get();
		}

		const Digital& Digital::operator()(Direction _dir)const{
			if(_dir == Direction_e::DIRECTION_OUTPUT)
				return this->Dout();
			else
				return this->Din();
		}

		const Digital& Digital::operator()(CHIP_SWM_PIN_MOVABLE_T _func)const{
			return this->Move(_func);
		}

		const Digital& Digital::operator()(CHIP_SWM_PIN_FIXED_T _fix)const{
			return this->Fix(_fix);
		}

		const Analog& Analog::Init(CHIP_SWM_PIN_FIXED_T _pin)const{
			Chip_IOCON_PinMuxSet(LPC_IOCON, mPort, _pin,
			IOCON_ADMODE_EN | IOCON_MODE_PULLDOWN);
			Chip_SWM_FixedPinEnable(_pin, true);
			return *this;
		}

		const Analog& Analog::operator()(CHIP_SWM_PIN_FIXED_T _pin)const{
			return this->Init(_pin);
		}
	}
}

