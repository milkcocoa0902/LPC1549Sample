/*
 * I2C.cpp
 *
 *  Created on: 2018/04/22
 *      Author: Keita
 */

#include "I2C.hpp"
#include "i2cm_15xx.hpp"

namespace Driver {
bool I2C::isInitialized = false;

I2C::I2C() :
	sda({0, 23}),
	scl({0, 22}),
	divider(24),
	bitRate(400000){

	if(isInitialized)
		return;

	// TODO Auto-generated constructor stub
	scl(GPIO::Option::StdI2C)(GPIO::Direction::In)(SWM_FIXED_I2C0_SCL);
	sda(GPIO::Option::StdI2C)(GPIO::Direction::In)(SWM_FIXED_I2C0_SDA);

	Chip_I2C_Init(LPC_I2C);
	Chip_I2C_SetClockDiv(LPC_I2C, divider);
	Chip_I2CM_SetBusSpeed(LPC_I2C, bitRate);
	Chip_I2CM_Enable(LPC_I2C);
}

I2C::~I2C() {
	// TODO Auto-generated destructor stub
	Chip_I2CM_Disable(LPC_I2C);
	Chip_I2C_DeInit(LPC_I2C);
}


bool I2C::ReadWrite(uint8_t _targetAddr, uint8_t* _txBuf, uint8_t* _rxBuf){
	return ReadWrite(_targetAddr, _txBuf, 1, _rxBuf, 1);
}

bool I2C::ReadWrite(uint8_t _targetAddr, uint8_t* _txBuf, uint32_t _txLen, uint8_t* _rxBuf){
	return ReadWrite(_targetAddr, _txBuf, _txLen, _rxBuf, 1);
}

bool I2C::ReadWrite(uint8_t _targetAddr, uint8_t* _txBuf, uint8_t* _rxBuf, uint32_t _rxLen){
	return ReadWrite(_targetAddr, _txBuf, 1, _rxBuf, _rxLen);
}

bool I2C::ReadWrite(uint8_t _targetAddr, uint8_t* _txBuf, uint32_t _txLen, uint8_t* _rxBuf, uint32_t _rxLen){
	I2CM_XFER_T i2cmXefer;

	i2cmXefer.slaveAddr = _targetAddr;
	i2cmXefer.status = 0;
	i2cmXefer.txSz = _txLen;
	i2cmXefer.rxSz = _rxLen;
	i2cmXefer.txBuff = _txBuf;
	i2cmXefer.rxBuff = _rxBuf;

	Chip_I2CM_XferBlocking(LPC_I2C, &i2cmXefer);

	return i2cmXefer.status == I2CM_STATUS_OK;
}

bool I2C::Write(uint8_t _targetAddr, uint8_t* _txBuf){
	return Write(_targetAddr, _txBuf, 1);
}

bool I2C::Write(uint8_t _targetAddr, uint8_t* _txBuf, uint32_t _txLen){
	I2CM_XFER_T i2cmXefer;

	i2cmXefer.slaveAddr = _targetAddr;
	i2cmXefer.status = 0;
	i2cmXefer.txSz = _txLen;
	i2cmXefer.rxSz = 0;
	i2cmXefer.txBuff = _txBuf;

	Chip_I2CM_XferBlocking(LPC_I2C, &i2cmXefer);

	return i2cmXefer.status == I2CM_STATUS_OK;
}

bool I2C::Read(uint8_t _targetAddr, uint8_t* _rxBuf){
	return Read(_targetAddr, _rxBuf, 1);
}

bool I2C::Read(uint8_t _targetAddr, uint8_t* _rxBuf, uint32_t _rxLen){
	I2CM_XFER_T i2cmXefer;

	i2cmXefer.slaveAddr = _targetAddr;
	i2cmXefer.status = 0;
	i2cmXefer.txSz = 0;
	i2cmXefer.rxSz = _rxLen;
	i2cmXefer.rxBuff = _rxBuf;

	Chip_I2CM_XferBlocking(LPC_I2C, &i2cmXefer);

	return i2cmXefer.status == I2CM_STATUS_OK;
}

} /* namespace Driver */
