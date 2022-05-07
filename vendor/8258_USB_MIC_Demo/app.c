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

#define	MIC_BUFFER_SIZE			4096
volatile signed short MicBuf[MIC_BUFFER_SIZE>>1];

void user_init(void)
{
	//1.enable USB DP pull up 1.5k
	usb_set_pin_en();
	//2.enable USB manual interrupt(in auto interrupt mode,USB device would be USB printer device)
	usb_irq_init();
	//3.enable global interrupt
	irq_enable();

	reg_usb_ep6_buf_addr =0x80;
	reg_usb_ep7_buf_addr =0xc0;
	reg_usb_ep_max_size  =256>>2;

	audio_config_mic_buf((unsigned short*)MicBuf,MIC_BUFFER_SIZE);

#if (AUDIO_MODE==AUDIO_AMIC_TO_USB)
	gpio_set_func(GPIO_PC4, AS_GPIO);
	gpio_set_output_en(GPIO_PC4, 1); 		//enable output
	gpio_set_input_en(GPIO_PC4 ,0);			//disable input
	gpio_write(GPIO_PC4, 1);              	//BIAS OUTPUT 1

	audio_amic_init(AUDIO_16K);
#elif(AUDIO_MODE==AUDIO_DMIC_TO_USB)
	gpio_set_func(GPIO_PA0, AS_DMIC);
	gpio_set_func(GPIO_PA1, AS_DMIC);
	gpio_set_input_en(GPIO_PA0, 1);

	audio_dmic_init(AUDIO_16K);

#elif AUDIO_MODE == AUDIO_CODEC_TO_USB
	audio_set_codec(I2C_GPIO_GROUP_A3A4, CODEC_MODE_LINE_TO_HEADPHONE_LINEOUT_I2S);

	audio_i2s_init();
#endif
	audio_set_usb_output();
}



void main_loop (void)
{
	usb_handle_irq_process();

	if(usb_audio_mic_cnt)
	{
#if(AUDIO_MODE==AUDIO_AMIC_TO_USB)
		audio_tx_data_to_usb(AMIC, AUDIO_16K);
#elif(AUDIO_MODE==AUDIO_DMIC_TO_USB)
		audio_tx_data_to_usb(DMIC, AUDIO_16K);
#elif(AUDIO_MODE==AUDIO_CODEC_TO_USB)
		audio_tx_data_to_usb(I2S_IN, AUDIO_16K);
#endif
		usb_audio_mic_cnt=0;
	}

}



