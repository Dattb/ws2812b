/********************************************************************************************************
 * @file     app_ir_fifo.c
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

#if(PWM_MODE==PWM_IR_FIFO)
/*********************************************************************************
    PWM0   :  PA2.  PC1.  PC2.	PD5
    PWM0_N :  PA0.  PB3.  PC4	PD5
 *********************************************************************************/

#define PWM_PIN					GPIO_PA2
#define AS_PWMx					AS_PWM0
#define PWM_ID					PWM0_ID
#define PWM_PULSE_NUM1			7
#define PWM_PULSE_NUM2			8
#define PWM_PULSE_NUM3			9


void user_init()
{
	sleep_ms(2000);

	pwm_set_clk(CLOCK_SYS_CLOCK_HZ, CLOCK_SYS_CLOCK_HZ);

	gpio_set_func(PWM_PIN, AS_PWMx);
	pwm_set_mode(PWM_ID, PWM_IR_FIFO_MODE);

	pwm_set_cycle_and_duty(PWM_ID,1000 * CLOCK_SYS_CLOCK_1US, 333 * CLOCK_SYS_CLOCK_1US);
	pwm_set_pwm0_shadow_cycle_and_duty(1000 * CLOCK_SYS_CLOCK_1US, 500 * CLOCK_SYS_CLOCK_1US);
	pwm_ir_fifo_set_data_entry(PWM_PULSE_NUM1,0,1);
	pwm_ir_fifo_set_data_entry(PWM_PULSE_NUM2,0,0);
	pwm_ir_fifo_set_data_entry(PWM_PULSE_NUM3,1,1);
	pwm_start(PWM_ID);
}



void main_loop (void)
{
	sleep_ms(50);
}

#endif

