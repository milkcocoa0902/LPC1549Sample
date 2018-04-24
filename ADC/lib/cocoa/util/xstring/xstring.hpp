/*
 * xstring.hpp
 *
 *  Created on: 2018/03/08
 *      Author: Keita
 */

#ifndef UTIL_XSTRING_XSTRING_HPP_
#define UTIL_XSTRING_XSTRING_HPP_

#include <string>
#include <stdint.h>

namespace Util{
	namespace XString{
		std::string toString(const char _data);
		std::string toString(int _data);
		std::string toString(float _data);

		template <class T>
		T toNumber(const std::string& _str);
	}
}


#endif /* UTIL_XSTRING_XSTRING_HPP_ */
