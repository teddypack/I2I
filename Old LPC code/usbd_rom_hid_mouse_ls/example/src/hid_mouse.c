/*
 * @brief This file contains USB HID Mouse example using USB ROM Drivers.
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
#include <stdint.h>
#include <string.h>
#include "usbd_rom_api.h"
#include "hid_mouse.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/**
 * @brief Structure to hold mouse data
 */
typedef struct {
	USBD_HANDLE_T hUsb;	/*!< Handle to USB stack. */
	uint8_t report[MOUSE_REPORT_SIZE];	/*!< Last report data  */
	uint8_t tx_busy;	/*!< Flag indicating whether a report is pending in endpoint queue. */
} Mouse_Ctrl_T;

/** Singleton instance of mouse control */
static Mouse_Ctrl_T g_mouse;
/* Part of WORKAROUND for artf49378 */
static USB_EP_HANDLER_T g_defaultHidHdlr;

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

extern const uint8_t Mouse_ReportDescriptor[];
extern const uint16_t Mouse_ReportDescSize;

/*****************************************************************************
 * Private functions
 ****************************************************************************/

static void setXYMouseReport(uint8_t *rep, int8_t xVal, int8_t yVal)
{
	rep[1] = (uint8_t) xVal;
	rep[2] = (uint8_t) yVal;
}

static void setLeftButtonMouseReport(uint8_t *rep, uint8_t state)
{
	if (state) {
		rep[0] |= 0x01;
	}
	else {
		rep[0] &= ~0x01;
	}
}

/* Routine to update mouse state report */
static void Mouse_UpdateReport(void)
{
	uint8_t joystick_status = Joystick_GetStatus();
	CLEAR_HID_MOUSE_REPORT(&g_mouse.report[0]);

	switch (joystick_status) {
	case JOY_PRESS:
		setLeftButtonMouseReport(g_mouse.report, 1);
		break;

	case JOY_LEFT:
		setXYMouseReport(g_mouse.report, -4, 0);
		break;

	case JOY_RIGHT:
		setXYMouseReport(g_mouse.report, 4, 0);
		break;

	case JOY_UP:
		setXYMouseReport(g_mouse.report, 0, -4);
		break;

	case JOY_DOWN:
		setXYMouseReport(g_mouse.report, 0, 4);
		break;
	}
}

/* HID Get Report Request Callback. Called automatically on HID Get Report Request */
static ErrorCode_t Mouse_GetReport(USBD_HANDLE_T hHid, USB_SETUP_PACKET *pSetup, uint8_t * *pBuffer, uint16_t *plength)
{
	/* ReportID = SetupPacket.wValue.WB.L; */
	switch (pSetup->wValue.WB.H) {
	case HID_REPORT_INPUT:
		Mouse_UpdateReport();
		*pBuffer = &g_mouse.report[0];
		*plength = MOUSE_REPORT_SIZE;
		break;

	case HID_REPORT_OUTPUT:				/* Not Supported */
	case HID_REPORT_FEATURE:			/* Not Supported */
		return ERR_USBD_STALL;
	}
	return LPC_OK;
}

/* HID Set Report Request Callback. Called automatically on HID Set Report Request */
static ErrorCode_t Mouse_SetReport(USBD_HANDLE_T hHid, USB_SETUP_PACKET *pSetup, uint8_t * *pBuffer, uint16_t length)
{
	/* we will reuse standard EP0Buf */
	if (length == 0) {
		return LPC_OK;
	}
	/* ReportID = SetupPacket.wValue.WB.L; */
	switch (pSetup->wValue.WB.H) {
	case HID_REPORT_INPUT:				/* Not Supported */
	case HID_REPORT_OUTPUT:				/* Not Supported */
	case HID_REPORT_FEATURE:			/* Not Supported */
		return ERR_USBD_STALL;
	}
	return LPC_OK;
}

/* HID interrupt IN endpoint handler */
static ErrorCode_t Mouse_EpIN_Hdlr(USBD_HANDLE_T hUsb, void *data, uint32_t event)
{
	switch (event) {
	case USB_EVT_IN:
		/* USB_EVT_IN occurs when HW completes sending IN packet. So clear the
		    busy flag for main loop to queue next packet.
		 */
		g_mouse.tx_busy = 0;
		break;
	}
	return LPC_OK;
}

/* Part of WORKAROUND for artf49378 ROM driver BUG */
static ErrorCode_t HID_LowSpeedPatch(USBD_HANDLE_T hUsb, void *data, uint32_t event)
{
	USB_CORE_CTRL_T *pCtrl = (USB_CORE_CTRL_T *) hUsb;
	USB_HID_CTRL_T *pHidCtrl = (USB_HID_CTRL_T *) data;
	ErrorCode_t ret = ERR_USBD_UNHANDLED;
	uint16_t cnt = 0, len = 0;

	switch (event) {
	case USB_EVT_SETUP:
		if (pCtrl->SetupPacket.bmRequestType.BM.Type == REQUEST_STANDARD) {

			switch (pCtrl->SetupPacket.bRequest) {
			case USB_REQUEST_GET_DESCRIPTOR:
				/* handle HID descriptors first */
				switch (pCtrl->SetupPacket.wValue.WB.H) {
				case HID_HID_DESCRIPTOR_TYPE:
					pCtrl->EP0Data.pData = pHidCtrl->hid_desc;
					len = ((USB_COMMON_DESCRIPTOR *) pHidCtrl->hid_desc)->bLength;
					ret = LPC_OK;
					break;

				case HID_REPORT_DESCRIPTOR_TYPE:
					ret = pHidCtrl->HID_GetReportDesc(pHidCtrl, &pCtrl->SetupPacket,
													  &pCtrl->EP0Data.pData, &len);
					break;

				case HID_PHYSICAL_DESCRIPTOR_TYPE:
					if (pHidCtrl->HID_GetPhysDesc == 0) {
						/* HID Physical Descriptor is not supported */
						ret = (ERR_USBD_STALL);
					}
					else {
						ret = pHidCtrl->HID_GetPhysDesc(pHidCtrl, &pCtrl->SetupPacket,
														&pCtrl->EP0Data.pData, &len);
					}
					break;

				default:
					/* handle standard descriptor requests */
					ret = pCtrl->USB_ReqGetDescriptor(pCtrl);
					break;
				}
				break;

			case USB_REQUEST_GET_CONFIGURATION:
				ret = pCtrl->USB_ReqGetConfiguration(pCtrl);
				break;

			case USB_REQUEST_GET_INTERFACE:
				ret = pCtrl->USB_ReqGetInterface(pCtrl);
				break;

			default:
				break;
			}
		}
		else if ((pCtrl->SetupPacket.bmRequestType.BM.Type == REQUEST_CLASS) &&
				 (pCtrl->SetupPacket.bmRequestType.BM.Recipient == REQUEST_TO_INTERFACE) &&
				 (pCtrl->SetupPacket.bRequest == HID_REQUEST_GET_REPORT) ) {
			/* point to data to be sent */
			pCtrl->EP0Data.pData = pCtrl->EP0Buf;
			/* allow user to copy data to EP0Buf or change the pointer to his own buffer */
			ret = pHidCtrl->HID_GetReport(pHidCtrl, &pCtrl->SetupPacket,
										  &pCtrl->EP0Data.pData, &pCtrl->EP0Data.Count);
		}
		break;

	case USB_EVT_IN:
		if (pCtrl->SetupPacket.bmRequestType.BM.Dir == REQUEST_DEVICE_TO_HOST) {
			ret = LPC_OK;
		}
		break;
	}

	if (ret == LPC_OK) {
		if ((len != 0) && (pCtrl->EP0Data.Count > len)) {
			pCtrl->EP0Data.Count = len;
		}
		cnt = (pCtrl->EP0Data.Count > USB_MAX_PACKET0) ? USB_MAX_PACKET0 : pCtrl->EP0Data.Count;
		cnt = USBD_API->hw->WriteEP(pCtrl, 0x80, pCtrl->EP0Data.pData, cnt);
		pCtrl->EP0Data.pData += cnt;
		pCtrl->EP0Data.Count -= cnt;
	}
	else if (ret == ERR_USBD_UNHANDLED) {
		ret = g_defaultHidHdlr(hUsb, data, event);
	}

	return ret;
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* HID mouse interface init routine */
ErrorCode_t Mouse_Init(USBD_HANDLE_T hUsb,
					   USB_INTERFACE_DESCRIPTOR *pIntfDesc,
					   uint32_t *mem_base,
					   uint32_t *mem_size)
{
	USBD_HID_INIT_PARAM_T hid_param;
	USB_HID_REPORT_T reports_data[1];
	ErrorCode_t ret = LPC_OK;
	USB_CORE_CTRL_T *pCtrl = (USB_CORE_CTRL_T *) hUsb;

	/* Do a quick check of if the interface descriptor passed is the right one. */
	if ((pIntfDesc == 0) || (pIntfDesc->bInterfaceClass != USB_DEVICE_CLASS_HUMAN_INTERFACE)) {
		return ERR_FAILED;
	}

	/* init joystick control */
	Board_Joystick_Init();

	/* Init HID params */
	memset((void *) &hid_param, 0, sizeof(USBD_HID_INIT_PARAM_T));
	hid_param.max_reports = 1;
	hid_param.mem_base = *mem_base;
	hid_param.mem_size = *mem_size;
	hid_param.intf_desc = (uint8_t *) pIntfDesc;
	/* user defined functions */
	hid_param.HID_GetReport = Mouse_GetReport;
	hid_param.HID_SetReport = Mouse_SetReport;
	hid_param.HID_EpIn_Hdlr  = Mouse_EpIN_Hdlr;
	/* Init reports_data */
	reports_data[0].len = Mouse_ReportDescSize;
	reports_data[0].idle_time = 0;
	reports_data[0].desc = (uint8_t *) &Mouse_ReportDescriptor[0];
	hid_param.report_data  = reports_data;

	ret = USBD_API->hid->init(hUsb, &hid_param);

	if (ret == LPC_OK) {
		/*	WORKAROUND for artf49378 ROM driver BUG:
		   Per USB specification maximum packet size for low-speed device should be 8 bytes.
		   But the current ROM stack sends packets larger than 8 bytes violating the spec. This
		   patch will split the packet into multiple 8 byte packets. The patch overwrites scenarios
		   where larger packets could be sent on EP0_IN endpoint. Since HID is the only class most
		   widely used in low-speed mode the patch is provided as an overwrite to HID handler.
		 */
		g_defaultHidHdlr = pCtrl->ep0_hdlr_cb[pCtrl->num_ep0_hdlrs - 1];
		/* store the default CDC handler and replace it with ours */
		pCtrl->ep0_hdlr_cb[pCtrl->num_ep0_hdlrs - 1] = HID_LowSpeedPatch;

		/* update memory variables */
		*mem_base = hid_param.mem_base;
		*mem_size = hid_param.mem_size;
		/* store stack handle for later use. */
		g_mouse.hUsb = hUsb;
	}

	return ret;
}

/* Mouse tasks */
void Mouse_Tasks(void)
{
	/* check device is configured before sending report. */
	if ( USB_IsConfigured(g_mouse.hUsb)) {
		if (g_mouse.tx_busy == 0) {
			/* update report based on board state */
			Mouse_UpdateReport();
			/* send report data */
			g_mouse.tx_busy = 1;
			USBD_API->hw->WriteEP(g_mouse.hUsb, HID_EP_IN, &g_mouse.report[0], MOUSE_REPORT_SIZE);
		}
	}
	else {
		/* reset busy flag if we get disconnected. */
		g_mouse.tx_busy = 0;
	}

}
