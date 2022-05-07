/********************************************************************************************************
 * @file     app.c 
 *
 * @brief    This is the xxxx file for TLSR8258
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


#define OUTPUT_PIN   GPIO_PA0

unsigned char result=0;


void user_init()
{
	sleep_ms(2000);  //leave enough time for SWS_reset when power on
	gpio_set_func(OUTPUT_PIN,AS_GPIO);
	gpio_set_input_en(OUTPUT_PIN,0);
	gpio_set_output_en(OUTPUT_PIN,1);
	gpio_write(OUTPUT_PIN,1);

	lpc_power_on(); //power on
	lpc_set_input_chn(LPC_INPUT_PB2); //select input channel PB2
	lpc_set_input_ref(LPC_LOWPOWER,LPC_LOWPOWER_REF_810MV);//select mode and reference
	lpc_set_scaling_coeff(LPC_SCALING_PER50);//select scalign coefficient
}


void main_loop (void)
{

	result =  lpc_get_result();

	gpio_write(OUTPUT_PIN,result);


}

