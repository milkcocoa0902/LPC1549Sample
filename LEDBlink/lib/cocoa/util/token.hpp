/*
 * token.hpp
 *
 *  Created on: 2017/03/21
 *      Author: Keita
 */

#ifndef UTIL_TOKEN_HPP_
#define UTIL_TOKEN_HPP_

namespace Util{
	extern char NewLine;
	static auto IsMark = [](char _c){
		return  (('!' <= _c && _c <= '/') ||
				(':' <= _c && _c <= '?') ||
				('[' <= _c && _c <= '`') ||
				('{' <= _c && _c <= '~'));
	};

	static auto IsEndLine = [](char _c){
		return (_c == NewLine);
	};

	static auto IsEnd = [](char _c)->bool{
		return (_c == '\0');
	};

	static auto IsPoint = [](char _c)->bool{
		return (_c == '.');
	};

	static auto IsSign = [](char _c){
		return ((_c == '+') || (_c == '-'));
	};

	static auto IsSmall = [](char _c){
		return (('a' <= _c) && (_c <= 'z'));
	};

	static auto IsLarge = [](char _c){
		return (('A' <= _c) && (_c <= 'Z'));
	};

	static auto IsAlphabet = [](char _c){
		return (IsSmall(_c) || IsLarge(_c));
	};

	static auto IsNumber = [](char _c)->bool{
		return (('0' <= _c) && (_c <= '9'));
	};

	static auto IsHex = [](char _c){
		return  (('0' <= _c) && (_c <= '9')) ||
				(('a' <= _c )&& (_c <= 'f')) ||
				(('A' <= _c) && (_c <= 'F'));
	};


}



#endif /* UTIL_TOKEN_HPP_ */
