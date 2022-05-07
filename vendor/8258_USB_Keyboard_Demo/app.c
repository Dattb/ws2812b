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

unsigned char  kb_data[8];
// BYTE0: special key(Ctrl/shift ...);
// BYTE1: reserved;
// BYTE2~BYTE7: normal key

void user_init()
{
	//1.enable USB DP pull up 1.5k
	usb_set_pin_en();
	//2.enable USB manual interrupt(in auto interrupt mode,USB device would be USB printer device)
	usb_irq_init();
	//3.set ep5 BUSY(BUSY same as ACK) bit as 1 means output endpoint buffer can receive data from USB host
	usbhw_data_ep_ack(USB_EDP_KEYBOARD_OUT);
	//4.enable global interrupt
	irq_enable();

	//initiate LED for indication
	gpio_set_func(RED_LED, AS_GPIO);
	gpio_set_input_en(RED_LED,0);
	gpio_set_output_en(RED_LED,1);
	gpio_write(RED_LED,ON);
	sleep_us(100000);
	gpio_write(RED_LED,OFF);

	gpio_set_func(YELLOW_LED, AS_GPIO);
	gpio_set_input_en(YELLOW_LED,0);
	gpio_set_output_en(YELLOW_LED,1);
	gpio_write(YELLOW_LED,ON);
	sleep_us(100000);
	gpio_write(YELLOW_LED,OFF);

	gpio_set_func(GREEN_LED, AS_GPIO);
	gpio_set_input_en(GREEN_LED,0);
	gpio_set_output_en(GREEN_LED,1);
	gpio_write(GREEN_LED,ON);
	sleep_us(100000);
	gpio_write(GREEN_LED,OFF);

	//initiate Button for keyboard input
	gpio_set_func(GPIO_PD1, AS_GPIO);
	gpio_set_input_en(GPIO_PD1,1);
	gpio_set_output_en(GPIO_PD1,0);
	gpio_setup_up_down_resistor(GPIO_PD1, PM_PIN_PULLUP_10K);

	gpio_set_func(GPIO_PD2, AS_GPIO);
	gpio_set_input_en(GPIO_PD2,1);
	gpio_set_output_en(GPIO_PD2,0);
	gpio_setup_up_down_resistor(GPIO_PD2, PM_PIN_PULLUP_10K);
}


void main_loop (void)
{

	usb_handle_irq_process();

	if(usb_g_config != 0 ){
		if(gpio_read(GPIO_PD1)==0)
		{
			sleep_us(10000);
			if(gpio_read(GPIO_PD1)==0)
			{
				while(gpio_read(GPIO_PD1)==0);
				gpio_write(RED_LED,ON);
				printf("Key:LCtrl + 1 + 2 !\n");
				// special key
				kb_data[0] = 1;		// 1: ctrl; 2: shift
				// reserved
				kb_data[1] = 0;
				// normal key: data[2]~data[7]
				kb_data[2] = 0x59;	// number key: 1
				kb_data[3] = 0x5a;  // number key: 2
				kb_data[4] = 0;
				kb_data[5] = 0;
				kb_data[6] = 0;
				kb_data[7] = 0;
				usb_keyboard_hid_report(kb_data);
			}
		}

		if(gpio_read(GPIO_PD2)==0)
		{
			sleep_us(10000);
			if(gpio_read(GPIO_PD2)==0)
			{
				while(gpio_read(GPIO_PD2)==0);
				gpio_write(RED_LED,OFF);
				printf("Key Released!\n");
				for(int i=0;i<8;i++)
				{
					kb_data[i]=0;
				}
				usb_keyboard_hid_report(kb_data);
			}
		}
	}
}

