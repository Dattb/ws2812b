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

volatile unsigned int t0;

void user_init()
{
	sleep_ms(2000);  //leave enough time for SWS_reset when power on

	//1.init the LED pin,for indication
	gpio_set_func(LED1 ,AS_GPIO);
	gpio_set_output_en(LED1, 1); 		//enable output
	gpio_set_input_en(LED1 ,0);			//disable input
	gpio_write(LED1, 0);              	//LED On

#if (TIMER_MODE==TIMER_SYS_CLOCK_MODE)
	timer2_set_mode(TIMER_MODE_SYSCLK,0,1000 * CLOCK_SYS_CLOCK_1MS);
	timer_start(TIMER2);
#elif(TIMER_MODE==TIMER_GPIO_TRIGGER_MODE)
	timer2_gpio_init(SW1, POL_FALLING);
	irq_enable();
	timer2_set_mode(TIMER_MODE_GPIO_TRIGGER,0,3);
	timer_start(TIMER2);
#elif(TIMER_MODE==TIMER_GPIO_WIDTH_MODE)
	timer2_gpio_init(SW1, POL_FALLING);
	irq_enable();
	timer2_set_mode(TIMER_MODE_GPIO_WIDTH,0,0);
	timer_start(TIMER2);
#elif(TIMER_MODE==TIMER_TICK_MODE)
	timer2_set_mode(TIMER_MODE_TICK,0,0);
	timer_start(TIMER2);
#elif(TIMER_MODE==TIMER_WATCHDOG_MODE)
	wd_set_interval_ms(1000,CLOCK_SYS_CLOCK_1MS);
	wd_start();
#endif

}

void main_loop (void)
{
#if(TIMER_MODE==TIMER_WATCHDOG_MODE)
	t0= clock_time();
	while(!clock_time_exceed(t0,990000));
	wd_clear();
	gpio_toggle(LED1);

#endif
}

