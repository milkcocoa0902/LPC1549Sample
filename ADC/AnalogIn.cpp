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
					IOCON_ADMODE_EN | IOCON_MODE_PULLDOWN);
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
			Chip_ADC_SetClockRate(LPC_ADC0, SystemCoreClock / ADCSampleClock);

			/// Set Sequencer to SEQ_A
			Chip_ADC_SetupSequencer(LPC_ADC0,
					ADC_SEQA_IDX,
					ADC_SEQ_CTRL_CHANSEL(pin.id) | ADC_SEQ_CTRL_MODE_EOS);

			/// ADC Calibration
			Chip_ADC_StartCalibration(LPC_ADC0);
			while(!Chip_ADC_IsCalibrationDone(LPC_ADC0));

			/// ADC Interrupt Settings
			Chip_ADC_ClearFlags(LPC_ADC0, Chip_ADC_GetFlags(LPC_ADC0));
			Chip_ADC_EnableInt(LPC_ADC0, ADC_INTEN_SEQA_ENABLE);
			NVIC_EnableIRQ(ADC0_SEQA_IRQn);

			// Start Sequencer
			Chip_ADC_EnableSequencer(LPC_ADC0, ADC_SEQA_IDX);



			/// @Section ADC1
			/// Supply clock
			Chip_ADC_Init(LPC_ADC1, 0);
			Chip_ADC_SetClockRate(LPC_ADC1, SystemCoreClock / ADCSampleClock);

			/// Set Sequencer to SEQ_A
			Chip_ADC_SetupSequencer(LPC_ADC1,
					ADC_SEQA_IDX,
					ADC_SEQ_CTRL_CHANSEL(pin.id) | ADC_SEQ_CTRL_MODE_EOS);

			/// ADC Calibration
			Chip_ADC_StartCalibration(LPC_ADC1);
			while(!Chip_ADC_IsCalibrationDone(LPC_ADC1));

			/// ADC Interrupt Settings
			Chip_ADC_ClearFlags(LPC_ADC1, Chip_ADC_GetFlags(LPC_ADC1));
			Chip_ADC_EnableInt(LPC_ADC1, ADC_INTEN_SEQA_ENABLE);
			NVIC_EnableIRQ(ADC1_SEQA_IRQn);

			// Start Sequencer
			Chip_ADC_EnableSequencer(LPC_ADC1, ADC_SEQA_IDX);

			isInitialized = true;
		}

		Analog::~Analog(){
			switch (pin.module_()){
				case 0:{
					auto CTRL = LPC_ADC0->SEQ_CTRL[ADC_SEQA_IDX];
					CTRL &=  ~ADC_SEQ_CTRL_CHANSEL(pin.id_());
					Chip_ADC_SetupSequencer(LPC_ADC0, ADC_SEQA_IDX, CTRL);
					break;
				}
				case 1:{
					auto CTRL = LPC_ADC1->SEQ_CTRL[ADC_SEQA_IDX];
					CTRL &=  ~ADC_SEQ_CTRL_CHANSEL(pin.id_());
					Chip_ADC_SetupSequencer(LPC_ADC1, ADC_SEQA_IDX, CTRL);
					break;
				}
			}
		}

		void Analog::Trigger(){
			Chip_ADC_StartSequencer(LPC_ADC0, ADC_SEQA_IDX);
			Chip_ADC_StartSequencer(LPC_ADC1, ADC_SEQA_IDX);
		}


		void Analog::SetInput(){
			switch (pin.module_()){
				case 0:{
					auto CTRL = LPC_ADC0->SEQ_CTRL[ADC_SEQA_IDX];
					CTRL |=  ADC_SEQ_CTRL_CHANSEL(pin.id_());
					Chip_ADC_SetupSequencer(LPC_ADC0, ADC_SEQA_IDX, CTRL);
				}
				case 1:{
					auto CTRL = LPC_ADC1->SEQ_CTRL[ADC_SEQA_IDX];
					CTRL |=  ADC_SEQ_CTRL_CHANSEL(pin.id_());
					Chip_ADC_SetupSequencer(LPC_ADC1, ADC_SEQA_IDX, CTRL);
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

		extern "C" void ADC0A_IRQHandler(void) {
			Chip_ADC_ClearFlags(LPC_ADC0, Chip_ADC_GetFlags(LPC_ADC0));
		}

		extern "C" void ADC1A_IRQHandler(void) {
			Chip_ADC_ClearFlags(LPC_ADC1, Chip_ADC_GetFlags(LPC_ADC1));
		}
	}
}
