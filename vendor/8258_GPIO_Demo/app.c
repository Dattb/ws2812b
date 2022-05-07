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

void user_init()
{
	sleep_ms(2000);
	//1.init the LED pin,for indication
	gpio_set_func(LED1 ,AS_GPIO);
	gpio_set_output_en(LED1, 1); 		//enable output
	gpio_set_input_en(LED1 ,0);			//disable input
	gpio_write(LED1, 0);              	//LED On

	//2.init the SW1 pin,for trigger interrupt
#if (GPIO_MODE == GPIO_IRQ )
	gpio_set_func(SW1 ,AS_GPIO);
	gpio_set_output_en(SW1, 0); 			//enable output
	gpio_set_input_en(SW1 ,1);				//disable input
	gpio_setup_up_down_resistor(SW1, PM_PIN_PULLUP_10K);
	gpio_set_interrupt(SW1, POL_FALLING);
#elif(GPIO_MODE == GPIO_IRQ_RSIC0)
	gpio_set_func(SW2 ,AS_GPIO);
	gpio_set_output_en(SW2, 0); 			//enable output
	gpio_set_input_en(SW2 ,1);				//disable input
	gpio_setup_up_down_resistor(SW2, PM_PIN_PULLUP_10K);
	gpio_set_interrupt_risc0(SW2, POL_FALLING);

#elif(GPIO_MODE == GPIO_IRQ_RSIC1)
	gpio_set_func(SW2 ,AS_GPIO);
	gpio_set_output_en(SW2, 0); 			//enable output
	gpio_set_input_en(SW2 ,1);				//disable input
	gpio_setup_up_down_resistor(SW2, PM_PIN_PULLUP_10K);
	gpio_set_interrupt_risc1(SW2, POL_FALLING);

#elif(GPIO_MODE == GPIO_TOGGLE)
	gpio_write(LED1, !gpio_read(LED1)); 
	gpio_toggle(LED1);

#elif(GPIO_MODE == GPIO_HIGH_RESISTOR)
	gpio_shutdown(GPIO_ALL);				//set all gpio as high resistor except sws and mspi
	
#endif
}


/////////////////////////////////////////////////////////////////////
// main loop flow
/////////////////////////////////////////////////////////////////////
void main_loop (void)
{



}




