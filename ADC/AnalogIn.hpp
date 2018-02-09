/*
 * AnalogIn.hpp
 *
 *  Created on: 2018/02/06
 *      Author: Keita
 */

#ifndef DRIVER_ANALOGIN_HPP_
#define DRIVER_ANALOGIN_HPP_

#include <chip.hpp>
#include <vector>
#include <functional>
#include <array>

namespace Driver{
	namespace GPIO{
	using port_t = uint8_t;
	using pin_t = uint8_t;
	using module_t = uint8_t;
	using id_t = uint8_t;

		class AnalogPin{
		public:
			port_t port;
			pin_t pin;
			module_t module;
			id_t id;

			AnalogPin() = default;
			AnalogPin(const uint8_t _port, const uint8_t _pin, const uint8_t _module, const uint8_t _id);
			AnalogPin(const AnalogPin&) = default;
			uint8_t port_()const;
			uint8_t pin_()const;
			uint8_t module_()const;
			uint8_t id_()const;
			const AnalogPin& operator()(const CHIP_SWM_PIN_FIXED_T)const;
		};

		class Analog{
		private:
			static bool isInitialized;
			static constexpr uint32_t ADCSampleClock = 50000;	// ADCのサンプリングレート[Hz]
			static constexpr double Vref = 2.5;
			static constexpr float Resolution = 12;
			AnalogPin pin;

		public:
			Analog(const uint8_t _port, const uint8_t _pin, const uint8_t _module, const uint8_t _id);
			Analog(const Analog&) = default;
			virtual ~Analog();
			void Trigger();
			void SetInput();	// ADCにピンを登録する
			uint32_t GetInputRaw();	// ADCの値をそのまま返す
		};

	}
}


#endif /* DRIVER_ANALOGIN_HPP_ */
