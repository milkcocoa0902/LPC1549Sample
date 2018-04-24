/*
 * xstring.cpp
 *
 *  Created on: 2018/03/08
 *      Author: Keita
 */

#include  "xstring.hpp"
#include <cmath>

namespace Util{
	namespace XString{
		std::string toString(const char _data){
			std::string str = "";
			str += _data;
			return str + '\0';
		}

		std::string toString(int _data){
			std::string str = "";
			bool minus = (_data < 0 ? true : false);

			if(minus)
				_data = -_data;

			while(_data != 0){
				str.insert(str.begin(), (_data % 10) + '0');
				_data /= 10;
			}

			if(minus)
				str = '-' + str;

			return str + '\0';
		}

		std::string toString(float _data){
			std::string str = "";
			bool minus = (_data < 0 ? true : false);

			if(minus)
				_data = -_data;

			int decimal = (int)_data;
			_data -= decimal;

			while(decimal != 0){
				str.insert(str.begin(), (decimal % 10) + '0');
				decimal /= 10;
			}

			str += '.';
			for(int i = 0;i < 5;i++){
				_data *= 10;
				str += ((int)_data % 10) + '0';
			}

			if(minus)
				str = '-' + str;

			return str + '\0';
		}

		template <class T>
		T toNumber(const std::string& _str){
			T result = 0;
			bool minus = false;
			bool point = false;
			int cnt = 1;

			for(char c : _str){
				if(c == '-'){
					if(minus)
						return 0;
					minus = true;
				}else if(c == '.'){
					if(point)
						return 0;
					point = true;
				}else if(c >= '0' && c <= '9'){
					if(!point){
						result = (result * 10) + (c - '0');
					}else{
						result += pow(0.1, cnt++) * (c - '0');
					}
				}else{
					return 0;
				}
			}

			if(minus)
				result = -result;

			return result;
		}
		template int toNumber<int>(const std::string& _str);
		template float toNumber<float>(const std::string& _str);
	}
}
