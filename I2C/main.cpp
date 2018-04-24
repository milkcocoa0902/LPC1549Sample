/*
 * mainn.cpp
 *
 *  Created on: 2018/04/24
 *      Author: Keita
 */

#include "chip.hpp"
#include "driver/comm/I2C.hpp"
#include "driver/comm/uart.hpp"
#include "driver/systick.hpp"

const uint32_t OscRateIn = 12000000;
const uint32_t RTCOscRateIn = 32768;

int main(){
	// クロックソースを水晶として起動
	Chip_SetupXtalClocking();
	SystemCoreClockUpdate();

	// Delay関数用にSystickTimerを初期化
	Driver::Tick::Init();

	// I2C用のインスタンス
	Driver::I2C i2c;

	// 結果をPCに送信するためのポート
	Driver::Serial serial({0, 15}, {0, 14}, 0);

	static uint8_t slaveAddr = 0x6a;		// L3GD20のアドレス
	static uint8_t whoAmI = 0x0F;			// 通信確認用のレジスタ
	static uint8_t ctrlReg1 = 0x20;		// コントロールレジスタ
	static uint8_t enable = 0x0F;			// 0x0Fを書き込むと動作開始
	static uint8_t zLow = 0x2C;			// Z軸角速度の下位8bit
	static uint8_t zHigh = 0x2D;			// Z軸角速度の上位8bit
	static float resolution = 0.00875;	// ±250[dps]での分解能
	uint8_t result = 0;								// 通信確認結果
	int16_t yaw;									// Z軸角速度

	static uint8_t txBuf[] = {ctrlReg1, enable};

	i2c.ReadWrite(slaveAddr, &whoAmI, &result);
	if(result == 0xD4){
		// 通信に成功した場合、センサーを稼働させる
		i2c.Write(slaveAddr, txBuf, 2);
	}

	// Write直後にRead動作をしてはいけない
	Driver::Tick::DelayMs(5);

	while(true){
		// Z軸の角速度を読み込む
		i2c.ReadWrite(slaveAddr, &zLow, &result);
		yaw = result;
		i2c.ReadWrite(slaveAddr, &zHigh, &result);
		yaw |= (result << 8);

		// 分解能を掛けて実際の値に変換
		serial << (yaw * resolution);
		serial << "\r";
		Driver::Tick::DelayMs(250);
	}
}

