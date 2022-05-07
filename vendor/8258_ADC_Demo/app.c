/********************************************************************************************************
 * @file     app.c
 *
 * @brief    This is  the ADC of application for TLSR8258 
 *
 * @author   Driver Group
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

volatile unsigned short base_val;
volatile unsigned short vbat_val;
volatile unsigned int rns_val;
void user_init()
{

#if(ADC_MODE==ADC_RNG_MODE)
	 random_generator_init();
#else

	adc_init();

	#if(ADC_MODE==ADC_BASE_MODE)

		adc_base_init(GPIO_PB6);
		adc_set_ain_pre_scaler(ADC_PRESCALER_1);//ADC pre_scaling default value is ADC_PRESCALER_1F8, it can change after adc_base_init().
	#elif (ADC_MODE==ADC_VBAT_MODE)
		adc_vbat_init(GPIO_PB0);
	#endif
		adc_power_on_sar_adc(1);		//After setting the ADC parameters, turn on the ADC power supply control bit

#endif
}


void main_loop (void)
{
#if(ADC_MODE==ADC_RNG_MODE)
	rns_val = rand();
#else
	#if(ADC_MODE==ADC_BASE_MODE)
		base_val = adc_sample_and_get_result();
	#elif (ADC_MODE==ADC_VBAT_MODE)
		vbat_val = adc_sample_and_get_result();
	#endif
#endif
}


