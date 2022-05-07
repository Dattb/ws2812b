/********************************************************************************************************
 * @file     app.c
 *
 * @brief    This is the Audio of application for TLSR8258.
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
#include "audio_data.h"
#define AUDIO_RATE_VAL			AUDIO_16K

#define	MIC_BUFFER_SIZE			4096
volatile signed short MicBuf[MIC_BUFFER_SIZE>>1];
volatile signed short MicBuf1[MIC_BUFFER_SIZE>>1];
volatile signed short MicBuf_f[MIC_BUFFER_SIZE>>1];
volatile signed short MicBuf2[MIC_BUFFER_SIZE>>1];

#define FLASH_READ_ADDR			0x040000
#define FLASH_WRITE_ADDR		0x020000
#define FLASH_BUFF_LEN			256
#define RECORD_BTN				GPIO_PD0
#define PLAYER_BTN				GPIO_PD1

#define HALF_PTR_SIZE			0x800     // 16k is ok without flash write operation
#define QUARTER_PTR_SIZE		0x400     // 16k is ok without flash write operation, 8k is ok with flash write operation
#define ONE_EIGHTH_PTR_SIZE		0x200     // 16k is ok without flash write operation, 8k is ok with flash write operation
#define ONE_SIXTEENTH_PTR_SIZE	0x100     // 16k is ok without flash write operation

#define BUFF_SIZE				QUARTER_PTR_SIZE
#define BUFF_LEN				(0x1000/BUFF_SIZE)

volatile unsigned short rptr;
volatile unsigned char cnt;
volatile unsigned int n;
volatile unsigned int m;

//* Points within our sound sample.
 const short *buffer = NULL;
//* Number of octets left in the sample buffer.
 volatile unsigned int remaining = 0;
//*Number of half fifo length .
unsigned short half_buff_length=0;

void user_init()
{
	gpio_set_func(GPIO_PA5, AS_USB);
	gpio_set_func(GPIO_PA6, AS_USB);

	usb_dp_pullup_en (1);  //open USB enum
	gpio_set_func(GPIO_PA5, AS_USB);
	gpio_set_func(GPIO_PA6, AS_USB);
	gpio_set_func(GPIO_PD4|GPIO_PD5 ,AS_GPIO);
	gpio_set_output_en(GPIO_PD4|GPIO_PD5, 1); 		//enable output
	gpio_set_input_en(GPIO_PD4|GPIO_PD5 ,0);			//disable input
	gpio_write(GPIO_PD4|GPIO_PD5, 0);
#if (AUDIO_MODE ==AUDIO_AMIC_TO_SDM)
	//PC4 for BIAS setting
	gpio_set_func(GPIO_PC4, AS_GPIO);
	gpio_set_output_en(GPIO_PC4, 1); 		//enable output
	gpio_set_input_en(GPIO_PC4 ,0);			//disable input
	gpio_write(GPIO_PC4, 1);              	//BIAS OUTPUT 1

	audio_config_mic_buf((unsigned short*)MicBuf,MIC_BUFFER_SIZE);
	audio_amic_init(AUDIO_RATE_VAL);
	audio_set_sdm_output(AMIC,AUDIO_RATE_VAL,1);

#elif (AUDIO_MODE ==AUDIO_DMIC_TO_SDM)
	gpio_set_func(GPIO_PA0, AS_DMIC);
	gpio_set_func(GPIO_PA1, AS_DMIC);

	gpio_set_input_en(GPIO_PA0, 1);

	audio_config_mic_buf((unsigned short*)MicBuf,MIC_BUFFER_SIZE);
	audio_dmic_init(AUDIO_RATE_VAL);
	audio_set_sdm_output(DMIC,AUDIO_RATE_VAL,1);

#elif AUDIO_MODE == AUDIO_AMIC_TO_CODEC
	audio_set_codec(I2C_GPIO_GROUP_A3A4, CODEC_MODE_I2S_TO_HEADPHONE_LINEOUT,CLOCK_SYS_CLOCK_HZ);
	audio_config_mic_buf((unsigned short*)MicBuf,MIC_BUFFER_SIZE);
	audio_amic_init(AUDIO_RATE_VAL);
	audio_set_i2s_output(AMIC,AUDIO_RATE_VAL);

#elif(AUDIO_MODE == AUDIO_BUF_TO_SDM)
	/* set fifo0 as output */
	dfifo_set_dfifo0((unsigned short*)MicBuf,MIC_BUFFER_SIZE);
	audio_buff_init(AUDIO_RATE_VAL);
	audio_set_sdm_output(BUF_IN,AUDIO_RATE_VAL,1);
	/*set store space as 16k*/
	write_reg8(0x40004,32);



#elif(AUDIO_MODE == AUDIO_AMIC_TO_BUF)
	gpio_set_func(GPIO_PC4, AS_GPIO);
	gpio_set_output_en(GPIO_PC4, 1); 		//enable output
	gpio_set_input_en(GPIO_PC4 ,0);			//disable input
	gpio_write(GPIO_PC4, 1);              	//BIAS OUTPUT 1

	/* set fifo1 as input */
	dfifo_set_dfifo1((unsigned short*)MicBuf2,MIC_BUFFER_SIZE);
	audio_amic_init(AUDIO_RATE_VAL);
	/*set store space as 16k*/
	write_reg8(0x40004,32);
#elif(AUDIO_MODE == AUDIO_AMIC_TO_BUF_TO_SDM)
	gpio_set_func(RECORD_BTN ,AS_GPIO);
	gpio_set_output_en(RECORD_BTN, 0); 			//enable output
	gpio_set_input_en(RECORD_BTN ,1);				//disable input
	gpio_setup_up_down_resistor(RECORD_BTN, PM_PIN_PULLUP_10K);

	gpio_set_func(PLAYER_BTN ,AS_GPIO);
	gpio_set_output_en(PLAYER_BTN, 0); 			//enable output
	gpio_set_input_en(PLAYER_BTN ,1);				//disable input
	gpio_setup_up_down_resistor(PLAYER_BTN, PM_PIN_PULLUP_10K);

	gpio_set_func(GPIO_PC4, AS_GPIO);
	gpio_set_output_en(GPIO_PC4, 1); 		//enable output
	gpio_set_input_en(GPIO_PC4 ,0);			//disable input
	gpio_write(GPIO_PC4, 1);              	//BIAS OUTPUT 1

	/* set fifo1 as input */
	dfifo_set_dfifo1((unsigned short*)MicBuf2,MIC_BUFFER_SIZE);


	/* set fifo0 as output */
	dfifo_set_dfifo0((unsigned short*)MicBuf,MIC_BUFFER_SIZE);
#elif (AUDIO_MODE ==AUDIO_AMIC_TO_SDM_BIDIR_SIGFIFO)
	//PC4 for BIAS setting
	gpio_set_func(GPIO_PC4, AS_GPIO);
	gpio_set_output_en(GPIO_PC4, 1); 		//enable output
	gpio_set_input_en(GPIO_PC4 ,0);			//disable input
	gpio_write(GPIO_PC4, 1);              	//BIAS OUTPUT 1

	audio_config_mic_buf((unsigned short*)MicBuf,MIC_BUFFER_SIZE);
	audio_amic_init(AUDIO_RATE_VAL);
	audio_set_sdm_output(BUF_IN,AUDIO_RATE_VAL,1);
	reg_dfifo_mode &= (~FLD_AUD_DFIFO0_OUT);
	write_reg8(0x40004,32);

#elif(AUDIO_MODE == AUDIO_AMIC_TO_SDM_BIDIR_BIDFIFO)

	gpio_set_func(GPIO_PC4, AS_GPIO);
	gpio_set_output_en(GPIO_PC4, 1); 		//enable output
	gpio_set_input_en(GPIO_PC4 ,0);			//disable input
	gpio_write(GPIO_PC4, 1);              	//BIAS OUTPUT 1

	/* set fifo1 as input */
	dfifo_set_dfifo1((unsigned short*)MicBuf2,MIC_BUFFER_SIZE);
	audio_amic_init(AUDIO_RATE_VAL);

	/* set fifo0 as output */
	dfifo_set_dfifo0((unsigned short*)MicBuf,MIC_BUFFER_SIZE);
	audio_set_sdm_output(BUF_IN,AUDIO_RATE_VAL,1);
	reg_dfifo_mode &= (~FLD_AUD_DFIFO0_OUT);
	write_reg8(0x40004,32);

#elif AUDIO_MODE == AUDIO_CODEC_TO_CODEC

	audio_set_codec(I2C_GPIO_GROUP_A3A4, CODEC_MODE_LINE_IN_TO_LINEOUT_I2S,CLOCK_SYS_CLOCK_HZ);
	audio_i2s_init();
	dfifo_set_dfifo0((unsigned short*)MicBuf,MIC_BUFFER_SIZE);
	dfifo_set_dfifo1((unsigned short*)MicBuf1,MIC_BUFFER_SIZE);
	audio_set_i2s_output(I2S_IN,AUDIO_48K);



#elif (AUDIO_MODE == AUDIO_BUF_TO_SDM_BY_TIMER_IRQ)
    /*
     *set timer interval,it equals approximately the time of sdm play half depth of fifo.
     *for example :AUDIO_RATE_VAL=16K, fifo depth=4096 bytes,  the time of sdm play half depth of fifo(2048 bytes) is 64 ms(2048*(1/(16*2))).
     */
	timer1_set_mode(TIMER_MODE_SYSCLK,0,((1000000/(16))/(0x1000/MIC_BUFFER_SIZE)) * CLOCK_SYS_CLOCK_1US);//set timer62.5ms<64ms
	/* set fifo0 as output */
	dfifo_set_dfifo0((unsigned short*)MicBuf,MIC_BUFFER_SIZE);
	audio_buff_init(AUDIO_RATE_VAL);
	audio_set_sdm_output(BUF_IN,AUDIO_RATE_VAL,1);
	irq_enable();//enable global interrupt

	half_buff_length=MIC_BUFFER_SIZE>>1;
	buffer = bump;
	remaining = sizeof(bump) / sizeof(bump[0]);
	unsigned int block_len = (remaining <(MIC_BUFFER_SIZE>>2)) ? remaining :(MIC_BUFFER_SIZE>>2);
	timer_start(TIMER1);//trigger timer.
	audio_rx_data_from_sample_buff(buffer, block_len);////first write half number of dfifo0 depth( recommend set 4K),
	buffer += block_len;
	remaining -= block_len;

#endif
}



void main_loop (void)
{

#if(AUDIO_MODE == AUDIO_BUF_TO_SDM)

	m=0;
	while(m<read_reg8(0x40004))
	{
		/* load 4k audio data from flash to buffer*/
		for(unsigned int i=0;i<(MIC_BUFFER_SIZE/FLASH_BUFF_LEN);i++)
		{
			flash_read_page(FLASH_READ_ADDR+ m*0x1000 +(FLASH_BUFF_LEN*i),FLASH_BUFF_LEN,(unsigned char *)(MicBuf_f+((FLASH_BUFF_LEN*i)/2)));
		}
		audio_rx_data_from_buff((signed char*)MicBuf_f,MIC_BUFFER_SIZE);
		while(reg_dfifo0_wptr < reg_dfifo0_rptr);
		m++;
	}

#elif(AUDIO_MODE == AUDIO_AMIC_TO_BUF)
	n=0;
	while(n<read_reg8(0x40004))
	{
		while(reg_dfifo1_wptr < (0x800+rptr));
		rptr=0x800;
		flash_erase_sector(FLASH_WRITE_ADDR + n*0x1000);
		for(unsigned int i=0;i<((MIC_BUFFER_SIZE/2)/FLASH_BUFF_LEN);i++)
		{
			flash_write_page(FLASH_WRITE_ADDR + n*0x1000 +(FLASH_BUFF_LEN*i),FLASH_BUFF_LEN,(unsigned char *)(MicBuf2+((FLASH_BUFF_LEN*i)/2)));
		}
		while(reg_dfifo1_wptr > rptr);
		rptr=0x00;
		for(unsigned int j=0;j<((MIC_BUFFER_SIZE/2)/FLASH_BUFF_LEN);j++)
		{
			flash_write_page(FLASH_WRITE_ADDR + n*0x1000 +FLASH_BUFF_LEN*8+(FLASH_BUFF_LEN*j),FLASH_BUFF_LEN,(unsigned char *)(MicBuf2+FLASH_BUFF_LEN*4+((FLASH_BUFF_LEN*j)/2)));
		}
		n++;
	}


#elif(AUDIO_MODE == AUDIO_AMIC_TO_BUF_TO_SDM)
	if(gpio_read(RECORD_BTN)==0)
	{
		sleep_ms(100);
		n=0;
		audio_amic_init(AUDIO_RATE_VAL);

		while(gpio_read(RECORD_BTN)==0)
		{
			while(reg_dfifo1_wptr < (0x200+rptr));
			rptr=0x200;
			flash_erase_sector(FLASH_WRITE_ADDR+n*0x1000);
			for(unsigned int i=0;i<((MIC_BUFFER_SIZE/2)/FLASH_BUFF_LEN);i++)
			{
				flash_write_page(FLASH_WRITE_ADDR + n*0x1000 + (FLASH_BUFF_LEN*i),FLASH_BUFF_LEN,(unsigned char *)(MicBuf2+((FLASH_BUFF_LEN*i)/2)));
			}
			while(reg_dfifo1_wptr > rptr);
			rptr=0x00;
			for(unsigned int j=0;j<((MIC_BUFFER_SIZE/2)/FLASH_BUFF_LEN);j++)
			{
				flash_write_page(FLASH_WRITE_ADDR + n*0x1000 + FLASH_BUFF_LEN*8+(FLASH_BUFF_LEN*j),FLASH_BUFF_LEN,(unsigned char *)(MicBuf2+FLASH_BUFF_LEN*4+((FLASH_BUFF_LEN*j)/2)));
			}
			n++;
			if(n>32)break;
		}

	}

	if(gpio_read(PLAYER_BTN)==0)
	{
		sleep_ms(100);
		m=0;

		audio_buff_init(AUDIO_RATE_VAL);
		audio_set_sdm_output(AMIC,AUDIO_RATE_VAL,1);

		while((n>=m)&&(gpio_read(PLAYER_BTN)==0))
		{
			for(unsigned int i=0;i<(MIC_BUFFER_SIZE/FLASH_BUFF_LEN);i++)
			{
				flash_read_page(FLASH_READ_ADDR + m*0x1000 +(FLASH_BUFF_LEN*i),FLASH_BUFF_LEN,(unsigned char *)(MicBuf_f+((FLASH_BUFF_LEN*i)/2)));
			}
			audio_rx_data_from_buff((signed char*)MicBuf_f,MIC_BUFFER_SIZE);
			while(reg_dfifo0_wptr < reg_dfifo0_rptr);
			m++;
		}
	}
#elif(AUDIO_MODE == AUDIO_AMIC_TO_SDM_BIDIR_SIGFIFO)
	/* 1.record amic data to buffer0 */
	n=0;
	while(n<read_reg8(0x40004))
	{
		while(reg_dfifo0_wptr < (0x400+rptr));
		rptr=0x400;
		flash_erase_sector(FLASH_WRITE_ADDR + n*0x1000);
		for(unsigned int i=0;i<((MIC_BUFFER_SIZE/4)/FLASH_BUFF_LEN);i++)
		{
			flash_write_page(FLASH_WRITE_ADDR + n*0x1000 +(FLASH_BUFF_LEN*i),FLASH_BUFF_LEN,(unsigned char *)(MicBuf+((FLASH_BUFF_LEN*i)/2)));
			flash_read_page(FLASH_READ_ADDR+ n*0x1000 +(FLASH_BUFF_LEN*i),FLASH_BUFF_LEN,(unsigned char *)(MicBuf+((FLASH_BUFF_LEN*i)/2)));
		}
		reg_dfifo_mode |= FLD_AUD_DFIFO0_OUT;
		while(reg_dfifo0_wptr < (0x400+rptr));
		rptr=0x800;
		for(unsigned int j=0;j<((MIC_BUFFER_SIZE/4)/FLASH_BUFF_LEN);j++)
		{
			flash_write_page(FLASH_WRITE_ADDR + n*0x1000 +FLASH_BUFF_LEN*4 +(FLASH_BUFF_LEN*j),FLASH_BUFF_LEN,(unsigned char *)(MicBuf+FLASH_BUFF_LEN*2 + ((FLASH_BUFF_LEN*j)/2)));
			flash_read_page(FLASH_READ_ADDR+ n*0x1000 +FLASH_BUFF_LEN*4 +(FLASH_BUFF_LEN*j),FLASH_BUFF_LEN,(unsigned char *)(MicBuf+FLASH_BUFF_LEN*2 + ((FLASH_BUFF_LEN*j)/2)));
		}
		while(reg_dfifo0_wptr < (0x400+rptr));
		rptr=0xc00;
		for(unsigned int j=0;j<((MIC_BUFFER_SIZE/4)/FLASH_BUFF_LEN);j++)
		{
			flash_write_page(FLASH_WRITE_ADDR + n*0x1000 +FLASH_BUFF_LEN*8 +(FLASH_BUFF_LEN*j),FLASH_BUFF_LEN,(unsigned char *)(MicBuf+FLASH_BUFF_LEN*4 + ((FLASH_BUFF_LEN*j)/2)));
			flash_read_page(FLASH_READ_ADDR+ n*0x1000 +FLASH_BUFF_LEN*8 +(FLASH_BUFF_LEN*j),FLASH_BUFF_LEN,(unsigned char *)(MicBuf+FLASH_BUFF_LEN*4 + ((FLASH_BUFF_LEN*j)/2)));
		}
		while(reg_dfifo0_wptr > rptr);
		rptr=0x00;
		for(unsigned int j=0;j<((MIC_BUFFER_SIZE/4)/FLASH_BUFF_LEN);j++)
		{
			flash_write_page(FLASH_WRITE_ADDR + n*0x1000 +FLASH_BUFF_LEN*12 +(FLASH_BUFF_LEN*j),FLASH_BUFF_LEN,(unsigned char *)(MicBuf+FLASH_BUFF_LEN*6 + ((FLASH_BUFF_LEN*j)/2)));
			flash_read_page(FLASH_READ_ADDR+ n*0x1000 +FLASH_BUFF_LEN*12 +(FLASH_BUFF_LEN*j),FLASH_BUFF_LEN,(unsigned char *)(MicBuf+FLASH_BUFF_LEN*6 + ((FLASH_BUFF_LEN*j)/2)));
		}
		n++;
	}

#elif(AUDIO_MODE == AUDIO_AMIC_TO_SDM_BIDIR_BIDFIFO)
	n=0;
	while(n<read_reg8(0x40004))
	{
		for(unsigned int j=0;j<BUFF_LEN;j++)
		{
			if(j==(BUFF_LEN-1))
			{
				while(reg_dfifo1_wptr > rptr);
				rptr=0;
			}
			else
			{
				while(reg_dfifo1_wptr < (BUFF_SIZE+rptr));
				rptr=BUFF_SIZE*(j+1);
			}

			if(j==0)flash_erase_sector(FLASH_WRITE_ADDR + n*0x1000);
			for(unsigned int i=0;i<((MIC_BUFFER_SIZE/BUFF_LEN)/FLASH_BUFF_LEN);i++)
			{
				/* operation of flash  will lead to delay of read ptr and let sdm play with missing data, you should control it by some ways*/
#if(AUDIO_RATE_VAL==AUDIO_8K)
				/* user can write data to flash to delay rptr */
				flash_write_page(FLASH_WRITE_ADDR + n*0x1000 + BUFF_SIZE*j + (FLASH_BUFF_LEN*i),FLASH_BUFF_LEN,(unsigned char *)(MicBuf2+(BUFF_SIZE*j/2)+((FLASH_BUFF_LEN*i)/2)));
#elif(AUDIO_RATE_VAL==AUDIO_16K)
                /* user should avoid writing data to flash to delay rptr*/
#endif
				flash_read_page(FLASH_READ_ADDR+ n*0x1000 + BUFF_SIZE*j + (FLASH_BUFF_LEN*i),FLASH_BUFF_LEN,(unsigned char *)(MicBuf+(BUFF_SIZE*j/2)+((FLASH_BUFF_LEN*i)/2)));
			}
			if(j==15)
			{
				reg_dfifo0_wptr=0;
			}
			else
			{
				reg_dfifo0_wptr = BUFF_SIZE*(j+1);
			}
			if(j==0) reg_dfifo_mode |= FLD_AUD_DFIFO0_OUT;
		}
		n++;
	}

#endif
}


