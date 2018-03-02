/*
 * PWM.hpp
 *
 *  Created on: 2017/11/04
 *      Author: Keita
 */

#ifndef DRIVER_PWM_HPP_
#define DRIVER_PWM_HPP_

#include <chip.hpp>
#include <stdint.h>
#include <vector>
#include "DigitalIO.hpp"

namespace Driver {

	class PWM {
	private:
		Driver::GPIO::Digital pin;	// Output Pin
		uint8_t ch; // OutputChannel
		static uint32_t freq;	// Frequency

	public:
		PWM() = delete;

		/// @brief  Default Constructor
		/// @param _pin  {Output Pin Group, Output Pin ID)
		/// @param _ch  Output Channel
		/// @param _clock  PWM Frequency(Default:150[kHz])
		PWM(const std::pair<uint8_t, uint8_t> _pin, const uint8_t _ch, const uint32_t _freq = 150000);

		/// @brief  Copy Constructor
		PWM(const PWM&) = default;

		/// @brief  Default DeConstructor
		virtual ~PWM();

		/// @breif  Set All of PWM Frequency
		/// @param _freq  PWM Frequency(Default:150[kHz])
		static void SetFreq(uint32_t _freq = 150000);

		/// @brief  Set PWM Duty
		/// @param _duty  PWM Duty
		void SetDuty(const int8_t _duty)const;

		/// @brief  Set PWM Duty
		/// @param _duty  PWM Duty
		void operator()(const int8_t _duty)const;
	};

} /* namespace Driver */

#endif /* DRIVER_PWM_HPP_ */
