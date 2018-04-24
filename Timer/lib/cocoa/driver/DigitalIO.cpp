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
		Digital LED1{1, 8};
		Digital LED2{1, 4};
		Digital LED3{1, 5};
		bool Digital::isInitialized = false;
		std::array<Util::CallBack, 8> Digital::IntCallback;

		Digital::Digital(const uint8_t _port, const uint8_t _pin):
				port(_port),
				pin(_pin){
			if(!isInitialized){
				Chip_GPIO_Init(LPC_GPIO);
				Chip_PININT_Init(LPC_GPIO_PIN_INT);
				Chip_SWM_Init();
				Chip_SWM_DisableFixedPin(SWM_FIXED_SWDIO);
				Chip_SWM_DisableFixedPin(SWM_FIXED_SWCLK_TCK);
				isInitialized = true;
			}
			Chip_GPIO_SetPinDIROutput(LPC_GPIO, port, pin);
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, IOCON_DIGMODE_EN);
			Chip_GPIO_SetPinState(LPC_GPIO, port, pin, false);

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

		void Digital::operator()(const bool _flag) const{
			Chip_GPIO_SetPinState(LPC_GPIO, port, pin, _flag);
		}

		void Digital::operator<<(const bool _flag)const{
			Chip_GPIO_SetPinState(LPC_GPIO, port, pin, _flag);
		}

		bool Digital::operator()()const{
			return Chip_GPIO_GetPinState(LPC_GPIO, port, pin);
		}

		void Digital::operator>>(bool& _in)const{
			_in = Chip_GPIO_GetPinState(LPC_GPIO, port, pin);
		}

		const Digital& Digital::operator()(const Direction _dir)const{
			Chip_GPIO_SetPinDIR(LPC_GPIO, port, pin, (bool)_dir);
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, IOCON_DIGMODE_EN);

			return *this;
		}

		const Digital& Digital::operator()(const Option _option)const{
			Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin,
					IOCON_DIGMODE_EN | (int)_option);
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

		const Digital& Digital::EnableInt(const uint32_t _ch, const IntEdge _edge, const Util::CallBackRef _callback){
			IntCh = _ch;
			IntCallback[IntCh] = _callback;
			switch(_edge){
			case IntEdge::Negative:
				Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH(IntCh));
				break;
			case IntEdge::Positive:
				Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH(IntCh));
				break;
			case IntEdge::Both:
				Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH(IntCh));
				Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH(IntCh));
				break;
			}
			Chip_INMUX_PinIntSel(IntCh, port, pin);
			NVIC_ClearPendingIRQ((IRQn)((long)IRQn::PIN_INT0_IRQn + IntCh));
			NVIC_EnableIRQ((IRQn)((long)IRQn::PIN_INT0_IRQn + IntCh));

			return *this;
		}

		const Digital& Digital::EnableInt(const uint32_t _ch, const IntEdge _edge, const Util::CallBackRRef _callback){
			IntCh = _ch;
			IntCallback[IntCh] = move(_callback);
			switch(_edge){
			case IntEdge::Negative:
				Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH(IntCh));
				break;
			case IntEdge::Positive:
				Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH(IntCh));
				break;
			case IntEdge::Both:
				Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH(IntCh));
				Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH(IntCh));
				break;
			}
			Chip_INMUX_PinIntSel(IntCh, port, pin);
			NVIC_ClearPendingIRQ((IRQn)((long)IRQn::PIN_INT0_IRQn + IntCh));
			NVIC_EnableIRQ((IRQn)((long)IRQn::PIN_INT0_IRQn + IntCh));

			return *this;
		}

		const Digital& Digital::DisableInt(){
			Chip_PININT_DisableIntLow(LPC_GPIO_PIN_INT, PININTCH(IntCh));
			Chip_PININT_DisableIntHigh(LPC_GPIO_PIN_INT, PININTCH(IntCh));
			NVIC_ClearPendingIRQ((IRQn)((long)IRQn::PIN_INT0_IRQn + IntCh));
			NVIC_DisableIRQ((IRQn)((long)IRQn::PIN_INT0_IRQn + IntCh));

			return *this;
		}


		extern "C" void PIN_INT0_IRQHandler(void) {
			auto& callback = Digital::IntCallback[0];
			if (callback != nullptr)
				callback();
			Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH0);
		}

		extern "C" void PIN_INT1_IRQHandler(void) {
			auto& callback = Digital::IntCallback[1];
			if (callback != nullptr)
				callback();
			Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH1);
		}

		extern "C" void PIN_INT2_IRQHandler(void) {
			auto& callback = Digital::IntCallback[2];
			if (callback != nullptr)
				callback();
			Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH2);
		}

		extern "C" void PIN_INT3_IRQHandler(void) {
			auto& callback = Digital::IntCallback[3];
			if (callback != nullptr)
				callback();
			Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH3);
		}

		extern "C" void PIN_INT4_IRQHandler(void) {
			auto& callback = Digital::IntCallback[4];
			if (callback != nullptr)
				callback();
			Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH4);
		}

		extern "C" void PIN_INT5_IRQHandler(void) {
			auto& callback = Digital::IntCallback[5];
			if (callback != nullptr)
				callback();
			Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH5);
		}

		extern "C" void PIN_INT6_IRQHandler(void) {
			auto& callback = Digital::IntCallback[6];
			if (callback != nullptr)
				callback();
			Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH6);
		}

		extern "C" void PIN_INT7_IRQHandler(void) {
			auto& callback = Digital::IntCallback[7];
			if (callback != nullptr)
				callback();
			Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH7);
		}

		void Init() {
		}

	}
}

