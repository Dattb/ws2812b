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

#define AUDIO_RATE_VAL			AUDIO_16K

void user_init(void)
{
#if(AUDIO_MODE == AUDIO_USB_TO_SDM)
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

	audio_usb_init(AUDIO_RATE_VAL);

	audio_set_sdm_output(USB_IN,AUDIO_RATE_VAL,1);
	audio_set_usb_output();


#endif

}


void main_loop (void)
{
#if (AUDIO_MODE == AUDIO_USB_TO_SDM)
	usb_handle_irq_process();

	if(usb_audio_speaker_cnt)
	{
		audio_rx_data_from_usb();
		usb_audio_speaker_cnt=0;
	}

#endif
}



