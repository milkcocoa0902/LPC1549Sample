/*
 * type.hpp
 *
 *  Created on: 2017/03/21
 *      Author: Keita
 */

#ifndef UTIL_TYPE_HPP_
#define UTIL_TYPE_HPP_


#include <functional>
#include <stdint.h>
#include <string>
#include <vector>


//よく使うテンプレートクラスを実体化する。
extern template class std::vector<std::string>;
extern template class std::vector<uint8_t>;

extern template class std::function<void(void)>;

namespace Util{

	//コールバック関数の型
	using CallBack = std::function<void(void)>;
	using CallBackRef = const std::function<void(void)>&;
	using CallBackRRef = std::function<void(void)>&&;

	using TextVector = std::vector<std::string>;
	using TextIterator = std::vector<std::string>::iterator;
	using ByteVector = std::vector<uint8_t>;

	using uint = unsigned int;

}


#endif /* UTIL_TYPE_HPP_ */
