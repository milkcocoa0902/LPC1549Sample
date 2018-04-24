#include <chip.hpp>
#include "usb.hpp"
#include "../DigitalIO.hpp"
#include <usbd/usbd_cdc.h>
#include <usbd/usbd.h>

namespace Driver{
	USBD_HANDLE_T USB::UsbHandle;
	const USBD_API_T* USB::UsbApi;
	bool USB::isInitialized = false;
	ErrorCode_t USB::initResult = ERR_FAILED;

	USB::VCOM_DATA USB::vComData;
	RINGBUFF_T USB::RxBuf;
	uint8_t USB::RxBufRaw[vComRxBufSize];

	USB::USB(){
		if(isInitialized){
			if(initResult == LPC_OK){
				Connect();
				return;
			}
		}

		USBD_API_INIT_PARAM_T UsbParam;
		USB_CORE_DESCS_T UsbDesc;

		Chip_USB_Init();

		UsbApi = (const USBD_API_T*)LPC_ROM_API->pUSBD;

		memset((void*)&UsbParam, 0, sizeof(USBD_API_INIT_PARAM_T));
		UsbParam.usb_reg_base = LPC_USB0_BASE;
		UsbParam.max_num_ep = 3 + 1;
		UsbParam.mem_base = USB_STACK_MEM_BASE;
		UsbParam.mem_size = USB_STACK_MEM_SIZE;

		UsbDesc.device_desc = (uint8_t*)&UsbDeviceDescriptor[0];
		UsbDesc.string_desc = (uint8_t*)&UsbDeviceDescriptor[0];
		UsbDesc.high_speed_desc = (uint8_t*)&UsbFsConfigDescriptor[0];
		UsbDesc.full_speed_desc = (uint8_t*)&UsbFsConfigDescriptor[0];
		UsbDesc.device_qualifier = 0;

		initResult = USBD_API->hw->Init(&UsbHandle, &UsbDesc, &UsbParam);
		if(initResult == LPC_OK){
			initResult = ComInit(UsbHandle, &UsbDesc, &UsbParam);
			if(initResult == LPC_OK){
				isInitialized = true;
				Connect();
			}
		}
		RingBuffer_Init(&RxBuf, RxBufRaw, sizeof(RxBufRaw[0]), vComRxBufSize);
	}

	void USB::Connect(){
		if(initResult == LPC_OK){
			NVIC_EnableIRQ(USB_IRQn);
			USBD_API->hw->Connect(UsbHandle, 1);
		}
	}

	void USB::Disconnect(){
		NVIC_DisableIRQ(USB_IRQn);
		USBD_API->hw->Connect(UsbHandle, 0);
	}


	ErrorCode_t USB::ComInit(const USBD_HANDLE_T _handle, USB_CORE_DESCS_T* _desc, USBD_API_INIT_PARAM_T* _param){
		USBD_CDC_INIT_PARAM_T CdcParam;
		auto ret = LPC_OK;
		uint32_t EpIndex;

		vComData.hUsb = _handle;
		memset((void*)&CdcParam, 0, sizeof(USBD_CDC_INIT_PARAM_T));
		CdcParam.mem_base = _param->mem_base;
		CdcParam.mem_size = _param->mem_size;
		CdcParam.cif_intf_desc = (uint8_t*)FindDesctiptor(_desc->high_speed_desc, CDC_COMMUNICATION_INTERFACE_CLASS);
		CdcParam.dif_intf_desc = (uint8_t*)FindDesctiptor(_desc->high_speed_desc, CDC_DATA_INTERFACE_CLASS);
		CdcParam.SetLineCode = SetLineCode;

		ret = USBD_API->cdc->init(_handle, &CdcParam, &vComData.hCdc);

		if(ret == LPC_OK){
			vComData.rx_buff = (uint8_t*)CdcParam.mem_base;
			CdcParam.mem_base += vComRxBufSize;
			CdcParam.mem_size -= vComRxBufSize;

			EpIndex = (((USB_CDC_IN_EP & 0x0F) << 1) + 1);
			ret = USBD_API->core->RegisterEpHandler(_handle, EpIndex, vComBulkInHandler, &vComData);

			if(ret == LPC_OK){
				EpIndex = ((USB_CDC_OUT_EP & 0x0F) << 1);
				ret = USBD_API->core->RegisterEpHandler(_handle, EpIndex, vComBulkOutHandler, &vComData);
			}

			_param->mem_size = CdcParam.mem_size;
			_param->mem_base = CdcParam.mem_base;
		}
		return ret;
	}

	ErrorCode_t USB::SetLineCode(USBD_HANDLE_T _handle, CDC_LINE_CODING* _code){
		VCOM_DATA* vCom = &vComData;
		vCom->tx_flags = VCOM_TX_CONNECTED;
		return LPC_OK;
	}

	ErrorCode_t USB::vComBulkOutHandler(USBD_HANDLE_T _handle, void* _data, uint32_t _event){
		VCOM_DATA* vCom = (VCOM_DATA*)_data;

		switch(_event){
		case USB_EVT_OUT:
			vCom->rx_count = USBD_API->hw->ReadEP(_handle, USB_CDC_OUT_EP, vCom->rx_buff);
			if(vCom->rx_flags & VCOM_RX_BUF_QUEUED){
				vCom->rx_flags &= ~VCOM_RX_BUF_QUEUED;
				if(vCom->rx_count != 0){
					vCom->rx_flags |= VCOM_RX_BUF_FULL;
				}
			}else if(vCom->rx_flags & VCOM_RX_DB_QUEUED){
				vCom->rx_flags &= ~VCOM_RX_DB_QUEUED;
				vCom->rx_flags |= VCOM_RX_DONE;
			}
			break;

		case USB_EVT_OUT_NAK:
			if((vCom->rx_flags & (VCOM_RX_BUF_FULL | VCOM_RX_BUF_QUEUED)) == 0){
				USBD_API->hw->ReadReqEP(_handle, USB_CDC_OUT_EP, vCom->rx_buff, vComRxBufSize);
				vCom->rx_flags |= VCOM_RX_BUF_QUEUED;
			}
			break;

		default:
			break;
		}

		return LPC_OK;
	}

	ErrorCode_t USB::vComBulkInHandler(USBD_HANDLE_T _handle, void* _data, uint32_t _event){
		VCOM_DATA* pVcom = (VCOM_DATA*) _data;

		if (_event == USB_EVT_IN) {
			pVcom->tx_flags &= ~VCOM_TX_BUSY;
		}
		return LPC_OK;
	}

	USB_INTERFACE_DESCRIPTOR* USB::FindDesctiptor(const uint8_t* _Desc, uint32_t _intfClass){
		USB_COMMON_DESCRIPTOR* pD;
		USB_INTERFACE_DESCRIPTOR* _IntfDesc = 0;
		uint32_t next_desc_adr;

		pD = (USB_COMMON_DESCRIPTOR *) _Desc;
		next_desc_adr = (uint32_t) _Desc;

		while (pD->bLength) {
			/* is it interface descriptor */
			if (pD->bDescriptorType == USB_INTERFACE_DESCRIPTOR_TYPE) {

				_IntfDesc = (USB_INTERFACE_DESCRIPTOR *) pD;
				/* did we find the right interface descriptor */
				if (_IntfDesc->bInterfaceClass == _intfClass) {
					break;
				}
			}
			_IntfDesc = 0;
			next_desc_adr = (uint32_t) pD + pD->bLength;
			pD = (USB_COMMON_DESCRIPTOR *) next_desc_adr;
		}

		return _IntfDesc;
	}

	uint32_t USB::vComReadBuf(uint8_t* _buf, uint32_t _len){
		auto vCom = &vComData;
		uint32_t cnt = 0;

		// Read data from receive buffer if data is presented
		if(vCom->rx_count){
			cnt = (vCom->rx_count < _len) ? vCom->rx_count : _len;
			memcpy(_buf, vCom->rx_buff, cnt);
			vCom->rx_rd_count += cnt;

			// Enter Critical Section
			NVIC_DisableIRQ(USB0_IRQn);
			if(vCom->rx_rd_count >= vCom->rx_count){
				vCom->rx_flags &= ~VCOM_RX_BUF_FULL;
				vCom->rx_rd_count = vCom->rx_count = 0;
			}
			// Exit Critical Section
			NVIC_EnableIRQ(USB0_IRQn);
		}

		return cnt;
	}

	uint32_t USB::vComWrite(uint8_t* _buf, uint32_t _len){
		auto vCom = &vComData;
		auto ret = 0;

		if((vCom->tx_flags & VCOM_TX_CONNECTED) && ((vCom->tx_flags & VCOM_TX_BUSY) == 0)){
			vCom->tx_flags |= VCOM_TX_BUSY;

			NVIC_DisableIRQ(USB0_IRQn);
			ret = USBD_API->hw->WriteEP(vCom->hUsb, USB_CDC_IN_EP, _buf, _len);
			NVIC_EnableIRQ(USB0_IRQn);
		}

		return ret;
	}

	void USB::CopyEp2Queue(){
		auto tmpSize = vComRxBufSizeTmp;
		uint8_t buf[tmpSize];
		uint32_t len;

		while((len = vComReadBuf(buf, tmpSize)) > 0){
			RingBuffer_InsertMult(&RxBuf, buf, len);
		}
	}

	void USB::Write(const std::string& _str){
		vComWrite((uint8_t*)_str.data(), _str.length());
	}

	void USB::WriteLine(const std::string& _str){
		Write(_str + '\r');
	}

	uint8_t USB::ReadByte(){
		uint8_t ret = 0;
		if(!isEmpty())
			RingBuffer_Pop(&RxBuf, (void*)&ret);
		return ret;
	}

	std::string USB::Read(){
		std::string str = "";
		char c;

		while(!isEmpty()){
			RingBuffer_Pop(&RxBuf, (void*)&c);
			str += c;
		}

		return str;
	}

	std::string USB::ReadLine(){
		if(isLine()) {
			std::string str = "";
			char c;
			c = ReadByte();

			while(c != '\r'){
				str += c;
				c = ReadByte();
			}

			return str + '\0';
		}

		return "";
	}

	bool USB::isEmpty(){
		return RingBuffer_IsEmpty(&RxBuf);
	}

	bool USB::isFull(){
		return RingBuffer_IsFull(&RxBuf);
	}

	bool USB::isExist(const char _c){
		char* data = (char*)RxBufRaw;

		for(unsigned int i = RxBuf.tail;i != RxBuf.head;i = ((i + 1) % RxBuf.count)){
			if(data[i] == _c)
				return true;
		}

		return false;
	}

	bool USB::isLine(){
		return isExist('\r');
	}



	extern "C" void USB_IRQHandler(){
		USB::UsbApi_()->hw->ISR(USB::UsbHandle_());
		USB::CopyEp2Queue();
	}
}
