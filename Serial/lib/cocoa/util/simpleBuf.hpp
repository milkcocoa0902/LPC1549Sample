/*
 * simpleBuf.hpp
 *
 *  Created on: 2017/03/21
 *      Author: Keita
 */

#ifndef UTIL_SIMPLEBUF_HPP_
#define UTIL_SIMPLEBUF_HPP_

#include <stddef.h>
#include <stdint.h>
#include <string.h>

namespace Util{

template <size_t _size>struct SimpleBuffer{
public:
	static constexpr size_t size = _size;

private:
	uint8_t buf[_size];
	size_t depth;
public:
	SimpleBuffer(){
		depth = 0;
	}
	SimpleBuffer(const SimpleBuffer&) = delete;

	void Write(uint8_t _Sc){
		buf[depth++] = _c;
	}

	void Write(const uint8_t* _inst, size_t _sz){
		memcpy(&buf[depth],_inst,_sz);
		depth += _sz;
	}

	const uint8_t* GetData(){
		return buf;
	}

	const size_t GetDepth(){
		return depth;
	}

	void Reset(){
		depth=0;
	}
};

}



#endif /* UTIL_SIMPLEBUF_HPP_ */
