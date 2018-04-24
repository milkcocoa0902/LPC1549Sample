/*
 * SPI.cpp
 *
 *  Created on: 2018/04/04
 *      Author: Keita
 */

#include "SPI.hpp"

namespace Driver {
std::array<LPC_SPI_T* , 2> SPI::SpiBase = {LPC_SPI0, LPC_SPI1};
std::array<bool, 2> SPI::isInitialized = {false, false};
uint32_t SPI::SpiClock;
std::array<SPI_CFG_T, 2> SPI::spiConfig;
std::array<SPI_DELAY_CONFIG_T, 2> SPI::spiDelayConf;

SPI::SPI(uint8_t _ch,
		Driver::GPIO::Digital _mosi,
		Driver::GPIO::Digital _miso,
		Driver::GPIO::Digital _clk):
			ch(_ch),
			mosi(_mosi),
			miso(_miso),
			clk(_clk),
			bitWidth(8){

	// そのチャンネルがすでに初期化されていたらその設定を使用する
	if(isInitialized[ch])
		return;

	// SPIのチャンネルに対してデフォルトの設定を行う
	Chip_SPI_Init(SpiBase[ch]);

	SpiClock = 8000000;

	spiConfig[ch].ClkDiv = SystemCoreClock / SpiClock;
	spiConfig[ch].Mode = SPI_MODE_MASTER;
	spiConfig[ch].ClockMode = SPI_CLOCK_MODE0;
	spiConfig[ch].DataOrder = SPI_DATA_MSB_FIRST;
	spiConfig[ch].SSELPol = SPI_CFG_SPOL0_LO |
			SPI_CFG_SPOL1_LO |
			SPI_CFG_SPOL2_LO |
			SPI_CFG_SPOL3_LO;
	Chip_SPI_SetConfig(SpiBase[ch], &spiConfig[ch]);


	// SPIのフレーム待機時間の設定
	spiDelayConf[ch].PreDelay = 1;
	spiDelayConf[ch].PostDelay = 1;
	spiDelayConf[ch].FrameDelay = 1;
	spiDelayConf[ch].TransferDelay = 1;
	Chip_SPI_DelayConfig(SpiBase[ch], &spiDelayConf[ch]);

	// 各種ピンのSWM設定
	if(ch == 0){
		mosi(SWM_SPI0_MOSI_IO);
		miso(Driver::GPIO::Direction::In)(SWM_SPI0_MISO_IO);
		clk(SWM_SPI0_SCK_IO);
	}

	if(ch == 1){
		mosi(SWM_SPI1_MOSI_IO);
		miso(Driver::GPIO::Direction::In)(SWM_SPI1_MISO_IO);
		clk(SWM_SPI1_SCK_IO);
	}

	isInitialized[ch] = true;

	// SPIを有効化する
	Chip_SPI_Enable(SpiBase[ch]);
}

SPI::SPI(uint8_t _ch):
		ch(_ch),
		mosi({}),
		miso({}),
		clk({}),
		bitWidth(8){
	SPI(ch, mosi, miso, clk);
}

SPI::~SPI() {
	// TODO Auto-generated destructor stub
}

void SPI::SetFreq(uint32_t _freq){
	SpiClock = _freq;
	spiConfig[ch].ClkDiv = SystemCoreClock / SpiClock;
	Chip_SPI_SetConfig(SpiBase[ch], &spiConfig[ch]);
}

void SPI::SetClockMode(uint32_t _mode){
	spiConfig[ch].ClockMode = (SPI_CLOCK_MODE_T)(_mode << 4);
	Chip_SPI_SetConfig(SpiBase[ch], &spiConfig[ch]);
}

void SPI::SetDataOrder(bool _msb){
	spiConfig[ch].DataOrder = (SPI_DATA_ORDER_T)(_msb << 3);
	Chip_SPI_SetConfig(SpiBase[ch], &spiConfig[ch]);
}

void SPI::SetBitWidth(uint32_t _width){
	bitWidth = _width;
}

void SPI::SetCS(Driver::GPIO::Digital _ncs, uint32_t _ch, bool _pole){
	auto add = 0;
	if(ch == 1){
		if(_ch == 0)
			add = SWM_SPI1_SSELSN_0_IO - SWM_SPI0_SSELSN_0_IO;
		if(_ch == 1)
			add = SWM_SPI1_SSELSN_1_IO - SWM_SPI0_SSELSN_1_IO;
	}
	_ncs((CHIP_SWM_PIN_MOVABLE_T)(SWM_SPI0_SSELSN_0_IO + _ch + add));

	auto poles = spiConfig[ch].SSELPol;
	if(!_pole)
		poles &= ~(1 << _ch);
	if(_pole)
		poles |= (1 << _ch);
	spiConfig[ch].SSELPol = poles;

	Chip_SPI_SetConfig(SpiBase[ch], &spiConfig[ch]);
}

bool SPI::ReadWrite(uint16_t* _txBuf, uint16_t* _rxBuf, uint32_t _ncs){
	return ReadWrite(_txBuf, _rxBuf, _ncs, 1);
}

bool SPI::ReadWrite(uint16_t* _txBuf, uint16_t* _rxBuf, uint32_t _ncs, uint32_t _len){
	SPI_DATA_SETUP_T SpiData;

	SpiData.pTx = _txBuf;
	SpiData.pRx = _rxBuf;
	SpiData.DataSize = bitWidth;
	SpiData.Length = _len;
	SpiData.ssel = ((0b1111 ^ (1 << _ncs)) << 16);
	SpiData.TxCnt = 0;
	SpiData.RxCnt = 0;

	auto retVal = Chip_SPI_RWFrames_Blocking(SpiBase[ch], &SpiData);
	return retVal != 0;
}

bool SPI::Write(uint16_t* _txBuf, uint32_t _ncs){
	return Write(_txBuf, _ncs, 1);
}

bool SPI::Write(uint16_t* _txBuf, uint32_t _ncs, uint32_t _len){
	SPI_DATA_SETUP_T SpiData;

	SpiData.pTx = _txBuf;
	SpiData.DataSize = bitWidth;
	SpiData.Length = _len;
	SpiData.ssel = ((0b1111 ^ (1 << _ncs)) << 16);
	SpiData.TxCnt = 0;
	SpiData.RxCnt = 0;

	auto retVal = Chip_SPI_WriteFrames_Blocking(SpiBase[ch], &SpiData);
	return retVal != 0;
}

bool SPI::Read(uint16_t* _rxBuf, uint32_t _ncs){
	return Read(_rxBuf, _ncs, 1);
}

bool SPI::Read(uint16_t* _rxBuf, uint32_t _ncs, uint32_t _len){
	SPI_DATA_SETUP_T SpiData;

	SpiData.pRx = _rxBuf;
	SpiData.DataSize = bitWidth;
	SpiData.Length = _len;
	SpiData.ssel = ((0b1111 ^ (1 << _ncs)) << 16);
	SpiData.TxCnt = 0;
	SpiData.RxCnt = 0;

	auto retVal = Chip_SPI_ReadFrames_Blocking(SpiBase[ch], &SpiData);
	return retVal != 0;
}

} /* namespace Driver */
