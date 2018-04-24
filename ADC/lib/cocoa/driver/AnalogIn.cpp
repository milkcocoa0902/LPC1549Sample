/*
 * AnalogIn.cpp
 *
 *  Created on: 2018/02/06
 *      Author: Keita
 */


#include "AnalogIn.hpp"

namespace Driver{
	namespace GPIO{

		AnalogPin::AnalogPin(const uint8_t _port, const uint8_t _pin, const uint8_t _module, const uint8_t _id):
				port(_port),
				pin(_pin),
				module(_module),
				id(_id){
			Chip_IOCON_PinMuxSet(LPC_IOCON,
					port,
					pin,
					IOCON_ADMODE_EN | IOCON_MODE_INACT);
			Chip_SWM_FixedPinEnable((CHIP_SWM_PIN_FIXED_T)(_module * 0x0C + _id), true);
		}

		uint8_t AnalogPin::port_()const{
			return port;
		}
		uint8_t AnalogPin::pin_()const{
			return pin;
		}
		uint8_t AnalogPin::module_()const{
			return module;
		}
		uint8_t AnalogPin::id_()const{
			return id;
		}


		std::array<bool, 2> Analog::isInitialized = {false, false};

		Analog::Analog(const uint8_t _port, const uint8_t _pin, const uint8_t _module, const uint8_t _id):
			pin(AnalogPin(_port, _pin, _module, _id)){

			if(!isInitialized[_module])
				Configuration(_module);

			SetInput();
		}

		Analog::~Analog(){
			switch (pin.module_()){
				case 0:{
					Chip_ADC_ClearSequencerBits(LPC_ADC0,
							ADC_SEQA_IDX,
							ADC_SEQ_CTRL_CHANSEL(pin.id_()));
					break;
				}
				case 1:{
					Chip_ADC_ClearSequencerBits(LPC_ADC1,
							ADC_SEQA_IDX,
							ADC_SEQ_CTRL_CHANSEL(pin.id_()));
					break;
				}
			}
		}

		void Analog::Configuration(uint32_t _ch){
			auto base = (_ch == 0 ? LPC_ADC0 : LPC_ADC1);

			/// supply clock
			Chip_ADC_Init(base, 0);
			Chip_ADC_SetClockRate(base, ADCSampleClock);

			/// Set Sequencer to SEQ_A
			Chip_ADC_SetSequencerBits(base,
					ADC_SEQA_IDX,
					ADC_SEQ_CTRL_BURST);

			/// ADC Calibration
			Chip_ADC_StartCalibration(base);
			while(!Chip_ADC_IsCalibrationDone(base));

			// Enable Sequencer
			Chip_ADC_EnableSequencer(base, ADC_SEQA_IDX);

			// Burst Sequencer
			Chip_ADC_StartBurstSequencer(base, ADC_SEQA_IDX);


			isInitialized[_ch] = true;
		}

		void Analog::SetInput(){
			switch (pin.module_()){
				case 0:{
					Chip_ADC_SetSequencerBits(LPC_ADC0,
							ADC_SEQA_IDX,
							ADC_SEQ_CTRL_CHANSEL(pin.id_()));
					break;
				}
				case 1:{
					Chip_ADC_SetSequencerBits(LPC_ADC1,
							ADC_SEQA_IDX,
							ADC_SEQ_CTRL_CHANSEL(pin.id_()));
					break;
				}
			}
		}

		uint32_t Analog::GetInputRaw(){
			uint32_t retValue;
			switch(pin.module_()){
				case 0:
					retValue = ADC_DR_RESULT(Chip_ADC_GetDataReg(LPC_ADC0, pin.id_()));
					break;
				case 1:
					retValue = ADC_DR_RESULT(Chip_ADC_GetDataReg(LPC_ADC1, pin.id_()));
					break;
				default:
					retValue = 0xFFFF;
			}

			return retValue;
		}
	}
}
