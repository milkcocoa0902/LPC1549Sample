/*
 * systick.hpp
 *
 *  Created on: 2017/06/06
 *      Author: Keita
 */

#ifndef DRIVER_SYSTICK_HPP_
#define DRIVER_SYSTICK_HPP_

#include "stdint.h"

namespace Driver{
	namespace Tick{
		/// @brief  Initialize Systick Timer
		/// @return none
		void Init();

		/// @brief     Delay for [us]
		/// @param _us Sleep time in [us]
		/// @return    none
		void DelayUs(const uint64_t _us);

		/// @brief     Delay for [ms]
		/// @param _ms Sleep time in [ms]
		/// @return    none
		void DelayMs(const uint64_t _ms);

		/// @brief      Delay for [s]
		/// @param _sec Sleep time in [sec]
		/// @return     none
		void Delay(const uint64_t _sec);

		/// @brief      Get System Time from Boot in [us]
		/// @param      none
		/// @return     System Time in [us]
		inline uint64_t GetSystemTimeUs();

		/// @brief      Get System Time from Boot in [ms]
		/// @param      none
		/// @return     System Time in [ms]
		inline uint64_t GetSystemTimeMs();

		/// @brief      Get System Time from Boot in [sec]
		/// @param      none
		/// @return     System Time in [sec]
		inline uint64_t GetSystemTimeSec();

		/// @brief      Get System Time from Boot in [min]
		/// @param      none
		/// @return     System Time in [min]
		inline uint64_t GetSystemTimeMin();
	}
}


#endif /* DRIVER_SYSTICK_HPP_ */
