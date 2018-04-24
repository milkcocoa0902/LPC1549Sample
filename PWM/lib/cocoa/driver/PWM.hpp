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
#include <array>

#include "DigitalIO.hpp"

namespace Driver {

	class PWM {
	private:
		Driver::GPIO::Digital pin;	// Output Pin
		uint8_t module;
		uint8_t ch; // OutputChannel
		static std::array<uint32_t, 4> freq;	// Frequency
		static std::array<LPC_SCT_T*, 4> PwmBase;

	public:
		enum class PWMCh: uint32_t{
			Ch0 = 0x00,		// SCT0_OUT0
			Ch1 = 0x01,		// SCT0_OUT1
			Ch2 = 0x02,		// SCT0_OUT2
			Ch3 = 0x03,		// SCT0_OUT3
			Ch4 = 0x04,		// SCT0_OUT4
			Ch5 = 0x05,		// SCT0_OUT5
			Ch6 = 0x06,		// SCT0_OUT6
			Ch7 = 0x07,		// SCT0_OUT7
			Ch8 = 0x10,		// SCT1_OUT0
			Ch9 = 0x11,		// SCT1_OUT1
			Ch10 = 0x12,	// SCT1_OUT2
			Ch11 = 0x13,	// SCT1_OUT3
			Ch12 = 0x14,	// SCT1_OUT4
			Ch13 = 0x15,	// SCT1_OUT5
			Ch14 = 0x16,	// SCT1_OUT6
			Ch15 = 0x17,	// SCT1_OUT7
			Ch16 = 0x20,	// SCT2_OUT0
			Ch17 = 0x21,	// SCT2_OUT1
			Ch18 = 0x22,	// SCT2_OUT2
			Ch19 = 0x23,	// SCT2_OUT3
			Ch20 = 0x24,	// SCT2_OUT4
			Ch21 = 0x25,	// SCT2_OUT5
			Ch22 = 0x30,	// SCT3_OUT0
			Ch23 = 0x31,	// SCT3_OUT1
			Ch24 = 0x32,	// SCT3_OUT2
			Ch25 = 0x33,	// SCT3_OUT3
			Ch26 = 0x34,	// SCT3_OUT4
			Ch27 = 0x35,	// SCT3_OUT5
		};

		PWM() = delete;

		/// @brief  Default Constructor
		/// @param _pin  {Output Pin Group, Output Pin ID)
		/// @param _ch  Output Channel
		/// @param _clock  PWM Frequency(Default:150[kHz])
		PWM(const std::pair<uint8_t, uint8_t> _pin, const PWMCh _ch, const uint32_t _freq = 150000);

		/// @brief  Copy Constructor
		PWM(const PWM&) = default;

		/// @brief  Default DeConstructor
		virtual ~PWM();

		/// @breif  Set All of PWM Frequency
		/// @param _freq  PWM Frequency(Default:150[kHz])
		void SetFreq(uint32_t _freq = 150000);

		/// @brief  Set PWM Duty
		/// @param _duty  PWM Duty
		void SetDuty(const int8_t _duty)const;

		/// @brief  Set PWM Duty
		/// @param _duty  PWM Duty
		void operator()(const int8_t _duty)const;
	};

} /* namespace Driver */

#endif /* DRIVER_PWM_HPP_ */
