/*
 * uart.hpp
 *
 *  Created on: 2017/03/21
 *      Author: Keita
 */

#ifndef DRIVER_COMM_UART_HPP_
#define DRIVER_COMM_UART_HPP_

#include <string>
#include <stdint.h>
#include <cstring>
#include <array>
#include "../DigitalIO.hpp"
#include "../../util/type.hpp"
#include "../../util/xstring/xstring.hpp"

namespace Driver{
	class Serial{
	private:
		static constexpr uint32_t UartChNum = 3;
		static constexpr size_t TxSize = 128, RxSize = 128; //バッファーのサイズ
		static RINGBUFF_T TxBuf[UartChNum], RxBuf[UartChNum];
		static char TxRaw[UartChNum][TxSize], RxRaw[UartChNum][RxSize];
		static LPC_USART_T* UartBase[UartChNum];
		static std::array<Util::CallBack, UartChNum> Callback;
		Driver::GPIO::Digital tx, rx;
		uint32_t id;
		uint32_t baud;


	public:
		Serial() = default;
		Serial(const std::pair<uint8_t, uint8_t> _tx, const std::pair<uint8_t, uint8_t> _rx, const uint32_t _id, const uint32_t _baud = 115200);
		Serial(const Serial&) = default;
		virtual ~Serial();

		void Write(const char _c);
		void Write(const uint8_t* _data, size_t _sz);
		void Write(const char* _str);
		void Write(const std::vector<uint8_t>& _data);
		void WriteLine();
		void WriteLine(const std::string& line);
		void WriteLine(const char* line);
		void WriteLine(const std::vector<uint8_t>& bytes);
		void Write(const std::string& _text);
		void Write(const int _data);
		void Write(const float _data);
		char ReadByte();
		std::string Read(const size_t _sz);
		std::string Read();
		std::string ReadLine();
		bool IsEmpty();
		bool IsFull();
		void Claer();
		bool IsBusy();
		bool IsExist(const char _c);
		void setBaud(const uint32_t _baud);
		void SetCallback(const Util::CallBackRef _callback);
		void SetCallback(const Util::CallBackRRef _callback);
		static void IRQHandler(uint32_t _id){
			auto isReceive = (Chip_UART_GetStatus(Serial::UartBase[_id]) & UART_STAT_RXRDY != 0);

			Chip_UART_IRQRBHandler(Serial::UartBase[_id], &Serial::RxBuf[_id], &Serial::TxBuf[_id]);

			if((Callback[_id] != nullptr) &&  isReceive){
				auto& func = Callback[_id];
				func();
			}
		}

		void operator<<(const std::string& _str){
			Write(_str);
		}

		void operator<<(char _c){
			Write(_c);
		}

		void operator<<(const std::vector<uint8_t>& _data){
			Write(_data);
		}

		void operator<<(const char* _str){
			Write(_str);
		}

		void operator<<(const int _d){
			Write(Util::XString::toString(_d));
		}

		void operator<<(const float _f){
			Write(Util::XString::toString(_f));
		}

		void operator<<(Serial& _serial){
			Write(_serial.Read());
		}

		void operator>>(char& _c){
			_c = ReadByte();
		}

		void operator>>(std::string& str){
			str = ReadLine();
		}

		void operator>>(int& _d){
			_d = Util::XString::toNumber<int>(ReadLine());
		}

		void operator>>(long& _ld){
			_ld = Util::XString::toNumber<long>(ReadLine());
		}

		void operator>>(float& _f){
			_f = Util::XString::toNumber<float>(ReadLine());
		}

		void operator>>(Serial& _serial){
			_serial.Write(Read());
		}
	};
}


#endif /* DRIVER_COMM_UART_HPP_ */
