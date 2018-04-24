/*
 * USB.hpp
 *
 *  Created on: 2018/03/21
 *      Author: Keita
 */

#ifndef DRIVER_COMM_USB_HPP_
#define DRIVER_COMM_USB_HPP_

#include <string>
#include <cstring>
#include <usbd/usbd_rom_api.h>
#include "usbd/app_usbd_cfg.hpp"
#include "ring_buffer.hpp"

#define VCOM_TX_CONNECTED   _BIT(8)		/* connection state is for both RX/Tx */
#define VCOM_TX_BUSY        _BIT(0)
#define VCOM_RX_DONE        _BIT(0)
#define VCOM_RX_BUF_FULL    _BIT(1)
#define VCOM_RX_BUF_QUEUED  _BIT(2)
#define VCOM_RX_DB_QUEUED   _BIT(3)

namespace Driver{
	class USB{

		static constexpr uint32_t vComRxBufSize = 1024;
		static constexpr uint32_t vComRxBufSizeTmp = 256;
		static const USBD_API_T* UsbApi;
		static USBD_HANDLE_T UsbHandle;

		struct VCOM_DATA {
			USBD_HANDLE_T hUsb;
			USBD_HANDLE_T hCdc;
			uint8_t *rx_buff;
			uint16_t rx_rd_count;
			uint16_t rx_count;
			uint16_t tx_buff_count;		//
			uint8_t *tx_buff;
			volatile uint16_t tx_flags;
			volatile uint16_t rx_flags;
		};
		static VCOM_DATA vComData;
		static RINGBUFF_T RxBuf;
		static uint8_t RxBufRaw[vComRxBufSize];
		static bool isInitialized;
		static ErrorCode_t initResult;

	public:
		USB();

	private:
		USB_INTERFACE_DESCRIPTOR *FindDesctiptor(const uint8_t *pDesc, uint32_t intfClass);
		ErrorCode_t ComInit(const USBD_HANDLE_T, USB_CORE_DESCS_T*, USBD_API_INIT_PARAM_T*);
		static ErrorCode_t SetLineCode(USBD_HANDLE_T, CDC_LINE_CODING*);
		static ErrorCode_t vComBulkInHandler(USBD_HANDLE_T, void*, uint32_t);
		static ErrorCode_t vComBulkOutHandler(USBD_HANDLE_T, void*, uint32_t);
		static uint32_t vComReadBuf(uint8_t*, uint32_t);
		static uint32_t vComWrite(uint8_t*, uint32_t);

	public:
		void Connect();
		void Disconnect();
		static void CopyEp2Queue();
		void Write(const std::string&);
		void WriteLine(const std::string&);
		uint8_t ReadByte();
		std::string Read();
		std::string ReadLine();
		bool isEmpty();
		bool isFull();
		bool isExist(const char);
		bool isLine();

		static const USBD_API_T* UsbApi_(){
			return UsbApi;
		}

		static const USBD_HANDLE_T UsbHandle_(){
			return UsbHandle;
		}
	};
}


#endif /* DRIVER_COMM_USB_HPP_ */
