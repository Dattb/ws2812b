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

unsigned char  mouse[4];

void user_init(void)
{
	//1.enable USB DP pull up 1.5k
	usb_set_pin_en();
	//2.enable USB manual interrupt(in auto interrupt mode,USB device would be USB printer device)
	usb_irq_init();
	//3.enable global interrupt
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

	//initiate Button for Mouse input
	gpio_set_func(GPIO_PD1, AS_GPIO);
	gpio_set_input_en(GPIO_PD1,1);
	gpio_set_output_en(GPIO_PD1,0);
	gpio_setup_up_down_resistor(GPIO_PD1, PM_PIN_PULLUP_10K);

	gpio_set_func(GPIO_PD2, AS_GPIO);
	gpio_set_input_en(GPIO_PD2,1);
	gpio_set_output_en(GPIO_PD2,0);
	gpio_setup_up_down_resistor(GPIO_PD2, PM_PIN_PULLUP_10K);
}

/* enum to USB input device and simulate the left click and right click of mouse */
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
				printf("Key:Mouse Left Click ! \r\n");
				mouse[0] = BIT(0);// BIT(0) - left key; BIT(1) - right key; BIT(2) - middle key; BIT(3) - side key; BIT(4) - external key
				mouse[1] = 0;	  // Displacement relative to x coordinate
				mouse[2] = 0;	  // Displacement relative to y coordinate
				mouse[3] = 0;     // Displacement relative to the roller
				usb_mouse_hid_report(mouse);
			}
		}

		if(gpio_read(GPIO_PD2)==0)
		{
			sleep_us(10000);
			if(gpio_read(GPIO_PD2)==0)
			{
				while(gpio_read(GPIO_PD2)==0);
				gpio_write(RED_LED,OFF);
				printf("Key:release \r\n");
				mouse[0] = 0;
				mouse[1] = 0;
				mouse[2] = 0;
				mouse[3] = 0;
				usb_mouse_hid_report (mouse);
			}
		}
	}

}
