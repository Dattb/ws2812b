/********************************************************************************************************
 * @file     main.c
 *
 * @brief    This is main for TLSR8258 Audio
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

extern void user_init();
extern void main_loop (void);
extern const short *buffer ;
extern volatile unsigned int remaining;
extern unsigned short half_buff_length;

/**
 * @brief		This function serves to handle the interrupt of MCU
 * @param[in] 	none
 * @return 		none
 */
_attribute_ram_code_ void irq_handler(void)
{
	/**
	 * @brief  This function servers to  write data to fifo by timer  interrupt.
	 * step 0:set timer interval,it  equals approximately the time of sdm play half depth of fifo.
	 * step 1: circulate write the half number of dfifo0 depth ,if fifo will full,write zero.
	 * step 2: if sample data finish,stop timer*/
#if (AUDIO_MODE == AUDIO_BUF_TO_SDM_BY_TIMER_IRQ)
	if(reg_tmr_sta & FLD_TMR_STA_TMR1)
		{
			reg_tmr_sta |= FLD_TMR_STA_TMR1; //clear irq status
			if (remaining)
			{
				unsigned int block_len_temp=((reg_dfifo0_wptr+1+half_buff_length)==reg_dfifo0_rptr)? 0:(half_buff_length>>1);
	        	unsigned int  block_len = (remaining <block_len_temp) ? remaining :block_len_temp;
	            audio_rx_data_from_sample_buff(buffer, block_len);
				buffer += block_len;
				remaining -= block_len;
			}
			else
			{
				timer_stop(TIMER1);
			}
		}

#endif
}

/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
int main (void) {

	cpu_wakeup_init();

	rf_drv_init(RF_MODE_BLE_1M);

	gpio_init(1);

	clock_init(SYS_CLK);

	user_init();

	while (1) {
		main_loop ();
	}
	return 0;
}





