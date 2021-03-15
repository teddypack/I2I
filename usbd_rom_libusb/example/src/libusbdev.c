/*
 * @brief USB Band Width test and WCID examples for USB ROM API.
 *
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2013
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "board.h"
#include <stdio.h>
#include <string.h>
#include "app_usbd_cfg.h"
#include "libusbdev.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/**
 * Structure containing Virtual Comm port control data
 */
typedef struct _LUSB_CTRL_ {
	USBD_HANDLE_T hUsb;		/*!< handle to USBD stack */
	uint8_t *pRxBuf;		/*!< Current Rx buffer pointer */
	uint32_t rxLen;			/*!< Size of the Rx buffer queued */
	uint32_t rxRemaining;	/*!< Remaining space in Rx buffer */
	uint8_t *pTxBuf;		/*!< Current Tx buffer pointer */
	uint32_t txLen;			/*!< Size of the Tx buffer queued */
	uint32_t txRemaining;	/*!< Remaining bytes to be sent */
	uint32_t newStatus;		/*!< New interrupt status */
	uint32_t curStatus;		/*!< Current interrupt status */
} LUSB_CTRL_T;

LUSB_CTRL_T g_lusb;

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/
const USBD_API_T *g_pUsbApi;

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Handle USB RESET event */
ErrorCode_t lusb_ResetEvent(USBD_HANDLE_T hUsb)
{
	/* reset the control structure */
	memset(&g_lusb, 0, sizeof(LUSB_CTRL_T));
	g_lusb.hUsb = hUsb;

	return LPC_OK;
}

/* USB bulk EP_IN endpoint handler */
ErrorCode_t lusb_BulkIN_Hdlr(USBD_HANDLE_T hUsb, void *data, uint32_t event)
{
	LUSB_CTRL_T *pUSB = (LUSB_CTRL_T *) data;
	uint32_t buf_len;

	if (event == USB_EVT_IN) {
		if (pUSB->txLen <= USB_FS_MAX_BULK_PACKET) {
			pUSB->pTxBuf = 0;
			pUSB->txLen = 0;
		}
		else {
			pUSB->txLen -= USB_FS_MAX_BULK_PACKET;
			buf_len = pUSB->txLen;
			/* check if we have more data to send */
			if (pUSB->txLen > USB_FS_MAX_BULK_PACKET) {
				buf_len = USB_FS_MAX_BULK_PACKET;
			}
			USBD_API->hw->WriteEP(pUSB->hUsb, LUSB_IN_EP, pUSB->pTxBuf, buf_len);
			pUSB->pTxBuf += buf_len;
		}
	}
	return LPC_OK;
}

/* USB bulk EP_IN endpoint handler */
ErrorCode_t lusb_IntrIN_Hdlr(USBD_HANDLE_T hUsb, void *data, uint32_t event)
{
	LUSB_CTRL_T *pUSB = (LUSB_CTRL_T *) data;

	if (event == USB_EVT_IN) {
		/* check if we have new status to send */
		if (pUSB->newStatus) {
			/* swap and send status */
			pUSB->curStatus = pUSB->newStatus;
			pUSB->newStatus = 0;
			USBD_API->hw->WriteEP(pUSB->hUsb, LUSB_INT_EP, (uint8_t *) &pUSB->curStatus, sizeof(uint32_t));
		}
		else {
			/* nothing to send */
			pUSB->curStatus = 0;
		}
	}
	return LPC_OK;
}

/* USB bulk EP_OUT endpoint handler */
ErrorCode_t lusb_BulkOUT_Hdlr(USBD_HANDLE_T hUsb, void *data, uint32_t event)
{
	LUSB_CTRL_T *pUSB = (LUSB_CTRL_T *) data;
	uint32_t len = 0;

	/* We received a transfer from the USB host. */
	if (event == USB_EVT_OUT) {
		len = USBD_API->hw->ReadEP(hUsb, LUSB_OUT_EP, pUSB->pRxBuf);

		pUSB->rxRemaining -= len;
		pUSB->pRxBuf += len;
		pUSB->rxLen += len;

		if ((len < USB_FS_MAX_BULK_PACKET) || (pUSB->rxRemaining < USB_FS_MAX_BULK_PACKET)) {
			/* Indicate transfer complete by setting pRxBuf to 0. we received a
			   short packet or the pRxBuf is full. */
			pUSB->pRxBuf = 0;
		}
	}

	return LPC_OK;
}

/* Handler for WCID USB device requests. */
ErrorCode_t WCID_hdlr(USBD_HANDLE_T hUsb, void *data, uint32_t event)
{
	USB_CORE_CTRL_T *pCtrl = (USB_CORE_CTRL_T *) hUsb;
	ErrorCode_t ret = ERR_USBD_UNHANDLED;

	if (event == USB_EVT_SETUP) {
		switch (pCtrl->SetupPacket.bmRequestType.BM.Type) {
		case REQUEST_STANDARD:
			if ((pCtrl->SetupPacket.bmRequestType.BM.Recipient == REQUEST_TO_DEVICE) &&
				(pCtrl->SetupPacket.bRequest == USB_REQUEST_GET_DESCRIPTOR) &&
				(pCtrl->SetupPacket.wValue.WB.H == USB_STRING_DESCRIPTOR_TYPE) &&
				(pCtrl->SetupPacket.wValue.WB.L == 0x00EE)) {
				pCtrl->EP0Data.pData = (uint8_t *) WCID_String_Descriptor;
				pCtrl->EP0Data.Count = pCtrl->SetupPacket.wLength;
				USBD_API->core->DataInStage(pCtrl);
				ret = LPC_OK;
			}
			break;

		case REQUEST_VENDOR:
			switch (pCtrl->SetupPacket.bRequest) {
			case 0x0E:		/* libusbK benchmark test */
				pCtrl->EP0Buf[0] = 0x02;
				pCtrl->EP0Data.pData = (uint8_t *) &pCtrl->EP0Buf[0];
				pCtrl->EP0Data.Count = pCtrl->SetupPacket.wLength;
				USBD_API->core->DataInStage(pCtrl);
				ret = LPC_OK;
				break;

			case 0x11:		/* libusbK benchmark test */
				pCtrl->EP0Buf[0] = 'A';
				pCtrl->EP0Buf[1] = 'B';
				pCtrl->EP0Buf[2] = 'C';
				pCtrl->EP0Data.pData = (uint8_t *) &pCtrl->EP0Buf[0];
				pCtrl->EP0Data.Count = pCtrl->SetupPacket.wLength;
				USBD_API->core->DataInStage(pCtrl);
				ret = LPC_OK;
				break;

			case 0x10:		/* libusbK benchmark test */
				pCtrl->EP0Data.pData = pCtrl->EP0Buf;	/* data to be received */
				ret = LPC_OK;
				break;

			case LUSB_PID:
				switch (pCtrl->SetupPacket.bmRequestType.BM.Recipient) {
				case REQUEST_TO_DEVICE:
					if (pCtrl->SetupPacket.wIndex.W == 0x0004) {
						pCtrl->EP0Data.pData = (uint8_t *) WCID_CompatID_Descriptor;
						pCtrl->EP0Data.Count = pCtrl->SetupPacket.wLength;
						USBD_API->core->DataInStage(pCtrl);
						ret = LPC_OK;
					}
					break;
				}
				break;
			}
		}
	}
	else if ((event == USB_EVT_OUT) && (pCtrl->SetupPacket.bmRequestType.BM.Type == REQUEST_VENDOR)) {
		if (pCtrl->SetupPacket.bRequest == 0x10) {
			USBD_API->core->StatusInStage(pCtrl);
			ret = LPC_OK;
		}
	}

	return ret;
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Handle interrupt from USB */
void USB_IRQHandler(void)
{
	uint32_t *addr = (uint32_t *) LPC_USB->EPLISTSTART;

	/*	WORKAROUND for artf32289 ROM driver BUG:
	    As part of USB specification the device should respond
	    with STALL condition for any unsupported setup packet. The host will send
	    new setup packet/request on seeing STALL condition for EP0 instead of sending
	    a clear STALL request. Current driver in ROM doesn't clear the STALL
	    condition on new setup packet which should be fixed.
	 */
	if ( LPC_USB->DEVCMDSTAT & _BIT(8) ) {	/* if setup packet is received */
		addr[0] &= ~(_BIT(29));	/* clear EP0_OUT stall */
		addr[2] &= ~(_BIT(29));	/* clear EP0_IN stall */
	}
	USBD_API->hw->ISR(g_lusb.hUsb);
}

/* Initialize USB interface. */
int libusbdev_init(uint32_t memBase, uint32_t memSize)
{
	USBD_API_INIT_PARAM_T usb_param;
	USB_CORE_DESCS_T desc;
	ErrorCode_t ret = LPC_OK;

	/* enable clocks and USB PHY/pads */
	Chip_USB_Init();

	/* Init USB API structure */
	g_pUsbApi = (const USBD_API_T *) LPC_ROM_API->usbdApiBase;

	/* initialize call back structures */
	memset((void *) &usb_param, 0, sizeof(USBD_API_INIT_PARAM_T));
	usb_param.usb_reg_base = LPC_USB0_BASE;
	/*	WORKAROUND for artf44835 ROM driver BUG:
	    Code clearing STALL bits in endpoint reset routine corrupts memory area
	    next to the endpoint control data. For example When EP0, EP1_IN, EP1_OUT,
	    EP2_IN are used we need to specify 3 here. But as a workaround for this
	    issue specify 4. So that extra EPs control structure acts as padding buffer
	    to avoid data corruption. Corruption of padding memory doesn’t affect the
	    stack/program behaviour.
	 */
	usb_param.max_num_ep = 3 + 1;
	usb_param.mem_base = memBase;
	usb_param.mem_size = memSize;
	usb_param.USB_Reset_Event = lusb_ResetEvent;

	/* Set the USB descriptors */
	desc.device_desc = (uint8_t *) USB_DeviceDescriptor;
	desc.string_desc = (uint8_t *) USB_StringDescriptor;
	/* Note, to pass USBCV test full-speed only devices should have both
	 * descriptor arrays point to same location and device_qualifier set
	 * to 0.
	 */
	desc.high_speed_desc = (uint8_t *) USB_FsConfigDescriptor;
	desc.full_speed_desc = (uint8_t *) USB_FsConfigDescriptor;
	desc.device_qualifier = 0;

	/* USB Initialization */
	ret = USBD_API->hw->Init(&g_lusb.hUsb, &desc, &usb_param);
	if (ret == LPC_OK) {

		/*	WORKAROUND for artf32219 ROM driver BUG:
		    The mem_base parameter part of USB_param structure returned
		    by Init() routine is not accurate causing memory allocation issues for
		    further components.
		 */
		usb_param.mem_base = USB_STACK_MEM_BASE + (USB_STACK_MEM_SIZE - usb_param.mem_size);

		/* register WCID handler */
		ret = USBD_API->core->RegisterClassHandler(g_lusb.hUsb, WCID_hdlr, &g_lusb);
		if (ret == LPC_OK) {
			/* register EP2 interrupt handler */
			ret = USBD_API->core->RegisterEpHandler(g_lusb.hUsb, 2, lusb_BulkOUT_Hdlr, &g_lusb);
			if (ret == LPC_OK) {

				ret = USBD_API->core->RegisterEpHandler(g_lusb.hUsb, 3, lusb_BulkIN_Hdlr, &g_lusb);
				if (ret == LPC_OK) {
					ret = USBD_API->core->RegisterEpHandler(g_lusb.hUsb, 5, lusb_IntrIN_Hdlr, &g_lusb);
					if (ret == LPC_OK) {
						/*  enable USB interrrupts */
						NVIC_EnableIRQ(USB0_IRQn);
						/* now connect */
						USBD_API->hw->Connect(g_lusb.hUsb, 1);
					}
				}
			}
		}
	}

	return ret;
}

/* Check if libusbdev is connected USB host application. */
bool libusbdev_Connected(void)
{
	return USB_IsConfigured(g_lusb.hUsb);
}

/* Queue the read buffer to USB DMA */
ErrorCode_t libusbdev_QueueReadReq(uint8_t *pBuf, uint32_t buf_len)
{
	LUSB_CTRL_T *pUSB = (LUSB_CTRL_T *) &g_lusb;
	ErrorCode_t ret = ERR_FAILED;

	/* Check if a read request is pending */
	if (pUSB->pRxBuf == 0) {
		/* Queue the read request */
		pUSB->pRxBuf = pBuf;
		pUSB->rxRemaining = buf_len;
		pUSB->rxLen = 0;
		ret = LPC_OK;
	}

	return ret;
}

/* Check if queued read buffer got any data */
int32_t libusbdev_QueueReadDone(void)
{
	LUSB_CTRL_T *pUSB = (LUSB_CTRL_T *) &g_lusb;

	/* A read request is pending */
	if (pUSB->pRxBuf) {
		return -1;
	}
	/* if data received return the length */
	return pUSB->rxLen;
}

/* A blocking read call */
int32_t libusbdev_Read(uint8_t *pBuf, uint32_t buf_len)
{
	int32_t ret = -1;

	/* Queue read request  */
	if (libusbdev_QueueReadReq(pBuf, buf_len) == LPC_OK) {
		/* wait for Rx to complete */
		while ( (ret = libusbdev_QueueReadDone()) == -1) {
			/* Sleep until next IRQ happens */
			__WFI();
		}
	}

	return ret;
}

/* Queue the given buffer for transmision to USB host application. */
ErrorCode_t libusbdev_QueueSendReq(uint8_t *pBuf, uint32_t buf_len)
{
	LUSB_CTRL_T *pUSB = (LUSB_CTRL_T *) &g_lusb;
	ErrorCode_t ret = ERR_FAILED;

	/* Check if a read request is pending */
	if (pUSB->pTxBuf == 0) {
		/* Queue the Tx request */
		pUSB->txLen = buf_len;
		/* We can send maximum USB_FS_MAX_BULK_PACKET at a time */
		if (buf_len > USB_FS_MAX_BULK_PACKET) {
			buf_len = USB_FS_MAX_BULK_PACKET;
		}
		/* write the packet to EP buffer */
		USBD_API->hw->WriteEP(pUSB->hUsb, LUSB_IN_EP, pBuf, buf_len);
		pUSB->pTxBuf = pBuf + buf_len;

		ret = LPC_OK;
	}

	return ret;
}

/* Check if queued send is done. */
int32_t libusbdev_QueueSendDone(void)
{
	LUSB_CTRL_T *pUSB = (LUSB_CTRL_T *) &g_lusb;

	/* return remaining length */
	return pUSB->txLen;
}

/* Send the given buffer to USB host application */
ErrorCode_t libusbdev_Send(uint8_t *pBuf, uint32_t buf_len)
{
	ErrorCode_t ret = libusbdev_QueueSendReq(pBuf, buf_len);

	/* Queue read request  */
	if (ret == LPC_OK) {
		/* wait for Rx to complete */
		while ( libusbdev_QueueSendDone() != 0) {
			/* Sleep until next IRQ happens */
			__WFI();
		}
	}

	return ret;
}

/* Send interrupt signal to USB host application. */
ErrorCode_t libusbdev_SendInterrupt(uint32_t status)
{
	LUSB_CTRL_T *pUSB = (LUSB_CTRL_T *) &g_lusb;

	/* enter critical section */
	NVIC_DisableIRQ(USB0_IRQn);
	/* update new status */
	pUSB->newStatus = status;
	/* exit critical section */
	NVIC_EnableIRQ(USB0_IRQn);

	/* check if we are in middle of sending current status */
	if ( pUSB->curStatus == 0) {
		/* If not update current status and send */
		pUSB->curStatus = pUSB->newStatus;
		pUSB->newStatus = 0;
		USBD_API->hw->WriteEP(pUSB->hUsb, LUSB_INT_EP, (uint8_t *) &pUSB->curStatus, sizeof(uint32_t));
	}

	return LPC_OK;
}
