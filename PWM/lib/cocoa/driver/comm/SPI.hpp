/*
 * SPI.hpp
 *
 *  Created on: 2018/04/04
 *      Author: Keita
 */

#ifndef DRIVER_COMM_SPI_HPP_
#define DRIVER_COMM_SPI_HPP_

#include "chip.hpp"
#include "../DigitalIO.hpp"
#include <vector>
#include <array>
#include <utility>
#include <stdint.h>

namespace Driver {

class SPI {
private:
	uint8_t ch;
	Driver::GPIO::Digital mosi;
	Driver::GPIO::Digital miso;
	Driver::GPIO::Digital clk;
	std::array<Driver::GPIO::Digital, 4>  ncs;
	uint32_t bitWidth;
	static std::array<bool, 2> isInitialized;
	static std::array<LPC_SPI_T*, 2> SpiBase;
	static uint32_t SpiClock;
	static std::array<SPI_CFG_T, 2> spiConfig;
	static std::array<SPI_DELAY_CONFIG_T, 2> spiDelayConf;

public:
	/// @brief Default Constructor
	/// @param _ch    SPI channel (0 or 1)
	/// @param _mosi  SPI MOSI, Master-In-Slave-Out.
	/// @param _miso  SPI MISO, Master-Out-Slave-In.
	/// @param _clk   SPI clock
	///
	/// ex.)
	/// Driver::SPI spi(0, {0, 13}, {0, 12}, {0, 11});
	SPI(uint8_t _ch,
			Driver::GPIO::Digital _mosi,
			Driver::GPIO::Digital _miso,
			Driver::GPIO::Digital _clk);

	/// @brief Constructor that only set channel for use. Use this, only primary constructor has already used.
	/// @param _ch  SPI channel (0 or 1)
	SPI(uint8_t _ch);

	virtual ~SPI();

	/// @brief  Set SPI speed
	/// @param  _freq  SPI frequency in [Hz]
	/// @return none
	void SetFreq(uint32_t _freq);

	/// @brief  Set clock polarity
	/// @param  mode  Select clock mode from the table shown below.
	/// @return none
	///
	///         Clock mode table
	/// |------|------|--------|---------|
	/// | mode | idle | change | capture |
	/// |------|------|--------|---------|
	/// |   0  | Low  | falling| rising  |
	/// |------|------|--------|---------|
	/// |   1  | Low  | rising | falling |
	/// |------|------|--------|---------|
	/// |   2  | High | rising | falling |
	/// |------|------|--------|---------|
	/// |   3  | High | falling| rising  |
	/// |------|------|--------|---------|
	void SetClockMode(uint32_t mode);

	/// @brief  Set data transmit/receive data order
	/// @param  _msb  false: MSB first, true: LSB first
	/// @return none
	void SetDataOrder(bool _msb);

	/// @brief  Set SPI bit width in a frame.
	/// @param  _width   Bit width in a frame. Same as the clock pulse count.
	/// @return none
	void SetBitWidth(uint32_t _width);

	/// @brief  Assign CS pin with polarity;
	/// @param  _ncs  CS pin as type of Driver::GPIO::Digital
	/// @param  _ch   The number that the cs pin should assign
	/// @param  _pole CS pin polarity. false: Active-Low, true: Active-High.
	void SetCS(Driver::GPIO::Digital _ncs, uint32_t _ch, bool _pole);

	/// @brief  Read and Write data between the selected device.
	/// @param  _txBuf  Pointer to transmit data buffer.
	/// @param  _rxBuf  Pointer to receive data buffer.
	/// @param  _ncs    The number to use CS pin to select device.
	/// @param  _len    Data length both of transmit and receive.
	/// @return Communication result. false: failed, true: succeeded.
	bool ReadWrite(uint16_t* _txBuf, uint16_t* _rxBuf, uint32_t _ncs);

	/// @brief  Read and Write data between the selected device.
	/// @param  _txBuf  Pointer to transmit data buffer.
	/// @param  _rxBuf  Pointer to receive data buffer.
	/// @param  _ncs    The number to use CS pin to select device.
	/// @return Communication result. false: failed, true: succeeded.
	bool ReadWrite(uint16_t* _txBuf, uint16_t* _rxBuf, uint32_t _ncs, uint32_t _len);

	/// @brief  Write data to the selected device. When the response is exist, deleted it.
	/// @param  _txBuf  Pointer to transmit data buffer.
	/// @param  _ncs    The number to use CS pin to select device.
	/// @return Communication result. false: failed, true: succeeded.
	bool Write(uint16_t* _txBuf, uint32_t _ncs);

	/// @brief  Write data to the selected device. When the response is exist, deleted it.
	/// @param  _txBuf  Pointer to transmit data buffer.
	/// @param  _ncs    The number to use CS pin to select device.
	/// @param  _len    Data length both of transmit and receive.
	/// @return Communication result. false: failed, true: succeeded.
	bool Write(uint16_t* _txBuf, uint32_t _ncs, uint32_t _len);

	/// @brief  Read data from the selected device.
	/// @param  _rxBuf  Pointer to receive data buffer.
	/// @param  _ncs    The number to use CS pin to select device.
	/// @return Communication result. false: failed, true: succeeded.
	bool Read(uint16_t* _rxBuf, uint32_t _ncs);

	/// @brief  Read data from the selected device.
	/// @param  _rxBuf  Pointer to receive data buffer.
	/// @param  _ncs    The number to use CS pin to select device.
	/// @param  _len    Data length both of transmit and receive.
	/// @return Communication result. false: failed, true: succeeded.
	bool Read(uint16_t* _rxBuf, uint32_t _ncs, uint32_t _len);
};

} /* namespace Driver */

#endif /* DRIVER_COMM_SPI_HPP_ */
