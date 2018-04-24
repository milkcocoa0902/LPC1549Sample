/*
 * I2C.hpp
 *
 *  Created on: 2018/04/22
 *      Author: Keita
 */

#ifndef LIB_COCOA_DRIVER_COMM_I2C_HPP_
#define LIB_COCOA_DRIVER_COMM_I2C_HPP_

#include "chip.hpp"
#include "../DigitalIO.hpp"

namespace Driver {

class I2C {
	GPIO::Digital sda;
	GPIO::Digital scl;
	uint32_t divider;
	uint32_t bitRate;
	static bool isInitialized;

public:
	/// @brief  Default Constructor
	I2C();

	/// @brief  Default Deconstructor
	virtual ~I2C();

	/// @brief  Read(1) and Write(1).
	/// @param  _targetAddr  Address of the target device.
	/// @param  _txBuf  Pointer to the buffer to send data.
	/// @param  _rxBuf  Pointer to the buffer to receive data.
	/// @return Communication result. false: failed, true: succeeded.
	bool ReadWrite(uint8_t _targetAddr, uint8_t* _txBuf, uint8_t* _rxBuf);


	/// @brief  Read(1) and Write(N).
	/// @param  _targetAddr  Address of the target device.
	/// @param  _txBuf  Pointer to the buffer to transmit data.
	/// @param  _txLen	Data length to transmit.
	/// @param  _rxBuf  Pointer to the buffer to receive data.
	/// @return Communication result. false: failed, true: succeeded.
	bool ReadWrite(uint8_t _targetAddr, uint8_t* _txBuf, uint32_t _txLen, uint8_t* _rxBuf);

	/// @brief  Read(N) and Write(1).
	/// @param  _targetAddr  Address of the target device.
	/// @param  _txBuf  Pointer to the buffer to transmit data.
	/// @param  _rxBuf  Pointer to the buffer to receive data.
	/// @param  _rxLen	Data length to receive data.
	/// @return Communication result. false: failed, true: succeeded.
	bool ReadWrite(uint8_t _targetAddr, uint8_t* _txBuf, uint8_t* _rxBuf, uint32_t _rxLen);

	/// @brief  Read(N) and Write(N).
	/// @param  _targetAddr  Address of the target device.
	/// @param  _txBuf  Pointer to the buffer to transmit data.
	/// @param  _txLen	Data length to transmit.
	/// @param  _rxBuf  Pointer to the buffer to receive data.
	/// @param  _rxLen	Data length to receive.
	/// @return Communication result. false: failed, true: succeeded.
	bool ReadWrite(uint8_t _targetAddr, uint8_t* _txBuf, uint32_t _txLen, uint8_t* _rxBuf, uint32_t _rxLen);

	/// @brief  Write(1) only.
	/// @param  _targetAddr  Address of the target device.
	/// @param  _txBuf  Pointer to the buffer to transmit data.
	/// @return Communication result. false: failed, true: succeeded.
	bool Write(uint8_t _targetAddr, uint8_t* _txBuf);

	/// @brief  Write(N) only.
	/// @param  _targetAddr  Address of the target device.
	/// @param  _txBuf  Pointer to the buffer to transmit data.
	/// @return Communication result. false: failed, true: succeeded.
	bool Write(uint8_t _targetAddr, uint8_t* _txBuf, uint32_t _txLen);

	/// @brief  Read(1) only.
	/// @param  _targetAddr  Address of the target device.
	/// @param  _rxBuf  Pointer to the buffer to receive data.
	/// @return Communication result. false: failed, true: succeeded.
	bool Read(uint8_t _targetAddr, uint8_t* _rxBuf);

	/// @brief  Read(N) only.
	/// @param  _targetAddr  Address of the target device.
	/// @param  _rxBuf  Pointer to the buffer to receive data.
	/// @param  _rxLen	Data length to receive.
	/// @return Communication result. false: failed, true: succeeded.
	bool Read(uint8_t _targetAddr, uint8_t* _rxBuf, uint32_t _rxLen);
};

} /* namespace Driver */

#endif /* LIB_COCOA_DRIVER_COMM_I2C_HPP_ */
