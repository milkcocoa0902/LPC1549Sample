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
		void Init();
		void DelayUS(const uint64_t);
		void DelayMs(const uint64_t);
		void Delay(const uint64_t);
	}
}


#endif /* DRIVER_SYSTICK_HPP_ */
