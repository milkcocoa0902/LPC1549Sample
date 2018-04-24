/*
 * fixMath.hpp
 *
 *  Created on: 2017/03/21
 *      Author: Keita
 */

#ifndef UTIL_MATH_FIXMATH_HPP_
#define UTIL_MATH_FIXMATH_HPP_

#include "fix.hpp"
#include "../type.hpp"

namespace Util{
	namespace Math{
		template<class T> T abs(const T& _a) {
			if (_a >= T(0)) {
				return _a;
			} else {
				return -_a;
			}
		}


		//負数なら1
		template<class T> bool sign(const T& _c) {
			return _c < T(0);
		}

		constexpr int fact(int x);//階乗

		constexpr fix32 pow(fix32 x,uint n);//x^nを求める

		fix32 exp(fix32);//自然対数の底
		fix32 sin(fix32);
		fix32 cos(fix32);

	}
}



#endif /* UTIL_MATH_FIXMATH_HPP_ */
