/********************************************************************************************************
 * @file     app_normal.c
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


#if(PWM_MODE==PWM_NORMAL)

/*********************************************************************************
    PWM0   :  PA2.  PC1.  PC2.	PD5
    PWM1   :  PA3.  PC3.
    PWM2   :  PA4.  PC4.
    PWM3   :  PB0.  PD2.
    PWM4   :  PB1.  PB4.
    PWM5   :  PB2.  PB5.
    PWM0_N :  PA0.  PB3.  PC4	PD5
    PWM1_N :  PC1.  PD3.
    PWM2_N :  PD4.
    PWM3_N :  PC5.
    PWM4_N :  PC0.  PC6.
    PWM5_N :  PC7.
 *********************************************************************************/

#define PWM_PIN		GPIO_PC1
#define AS_PWMx		AS_PWM0
#define PWM_ID		PWM0_ID

void user_init()
{
	sleep_ms(2000);

	pwm_set_clk(CLOCK_SYS_CLOCK_HZ, CLOCK_SYS_CLOCK_HZ);

	gpio_set_func(PWM_PIN, AS_PWMx);
	pwm_set_mode(PWM_ID, PWM_NORMAL_MODE);
	//pwm_set_cycle_and_duty(PWM_ID, 500 * CLOCK_SYS_CLOCK_1US, 250 * CLOCK_SYS_CLOCK_1US);
	//pwm_start(PWM_ID);
}
u16 pwm_cnt = 0;
void main_loop (void)
{
	//pwm_cnt++;
	//if(pwm_cnt>=500) pwm_cnt = 0;

	//pwm_set_cycle_and_duty(PWM_ID, 2 * CLOCK_SYS_CLOCK_1US,1*CLOCK_SYS_CLOCK_1US);
	pwm_set_cycle_and_duty(PWM_ID,2*CLOCK_SYS_CLOCK_1PER8US,CLOCK_SYS_CLOCK_1PER8US);

	pwm_start(PWM_ID);
	sleep_ms(100);
}

#endif

