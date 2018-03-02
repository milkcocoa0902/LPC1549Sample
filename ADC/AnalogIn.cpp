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


		bool Analog::isInitialized = false;

		Analog::Analog(const uint8_t _port, const uint8_t _pin, const uint8_t _module, const uint8_t _id):
			pin(AnalogPin(_port, _pin, _module, _id)){

			SetInput();

			if(isInitialized)
				return;

			/// @section ADC0
			/// supply clock
			Chip_ADC_Init(LPC_ADC0, 0);
			Chip_ADC_SetClockRate(LPC_ADC0, ADCSampleClock);

			/// Set Sequencer to SEQ_A
			Chip_ADC_SetSequencerBits(LPC_ADC0,
					ADC_SEQA_IDX,
					ADC_SEQ_CTRL_BURST);

			/// ADC Calibration
			Chip_ADC_StartCalibration(LPC_ADC0);
			while(!Chip_ADC_IsCalibrationDone(LPC_ADC0));

			// Start Sequencer
			Chip_ADC_EnableSequencer(LPC_ADC0, ADC_SEQA_IDX);



			/// @Section ADC1
			/// Supply clock
			Chip_ADC_Init(LPC_ADC1, 0);
			Chip_ADC_SetClockRate(LPC_ADC1, ADCSampleClock);

			/// Set Sequencer to SEQ_A
			Chip_ADC_SetSequencerBits(LPC_ADC1,
					ADC_SEQA_IDX,
					ADC_SEQ_CTRL_BURST);
			/// ADC Calibration
			Chip_ADC_StartCalibration(LPC_ADC1);
			while(!Chip_ADC_IsCalibrationDone(LPC_ADC1));


			// Start Sequencer
			Chip_ADC_EnableSequencer(LPC_ADC1, ADC_SEQA_IDX);

			SetInput();
			isInitialized = true;


			Chip_ADC_StartBurstSequencer(LPC_ADC0, ADC_SEQA_IDX);
			Chip_ADC_StartBurstSequencer(LPC_ADC1, ADC_SEQA_IDX);
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
