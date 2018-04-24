/*
 * fixMath.cpp
 *
 *  Created on: 2017/03/21
 *      Author: Keita
 */

#include "fixMath.hpp"

namespace Util{
	namespace Math{

		constexpr int fact(int _n) {
			// ループには再帰を使用する
			return _n == 0 ? 1 : _n * fact(_n - 1);
		}

		constexpr fix32 pow(fix32 _x,uint _n){
			fix32 temp = fix32::One;
			for (uint i = 0;i < _n;i++){
				temp *= _x;
			}
			return _x;
		}

		fix32 exp(fix32 x){
			fix32 y;
			return 0;//stub
		}

		fix32 sin(fix32 x) {
			return 0;//stub
		}

		fix32 cos(fix32 x) {
			return 0;//stub
		}

	}
}


