/********************************************************************************************************
 * @file     app.c 
 *
 * @brief    This is the source file for TLSR8258
 *
 * @author	 Driver Group
 * @date     May 8, 2018
 *
 * @par      Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *           The information contained herein is confidential property of Telink
 *           Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *           of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *           Co., Ltd. and the licensee or the terms described here-in. This heading
 *           MUST NOT be removed from this file.
 *
 *           Licensees are granted free, non-transferable use of the information in this
 *           file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 * @par      History:
 * 			 1.initial release(DEC. 26 2018)
 *
 * @version  A001
 *
 *******************************************************************************************************/
#include "app_config.h"

void user_init(void)
{
	//1.enable USB DP pull up 1.5k
	usb_set_pin_en();
	//2.enable USB manual interrupt(in auto interrupt mode,USB device would be USB printer device)
	usb_irq_init();
	//3.set ep5 BUSY(BUSY same as ACK) bit as 1 means output endpoint buffer can receive data from USB host
	usbhw_data_ep_ack(USB_EDP_CDC_OUT);
	//4.enable global interrupt
	irq_enable();
}



void main_loop (void)
{
	usb_handle_irq_process();

	if((usb_g_config != 0) && (usb_cdc_data_len!=0)){
		usb_cdc_tx_data_to_host(usb_cdc_data,usb_cdc_data_len);
		usb_cdc_data_len = 0;
	}
}



