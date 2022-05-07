/**

 * @file     main.c 
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

extern void user_init();
extern void main_loop (void);



unsigned int ir_dma_fifo_cnt=0;
/**
 * @brief		This function serves to handle the interrupt of MCU
 * @param[in] 	none
 * @return 		none
 */
_attribute_ram_code_ void irq_handler(void)
{
#if (PWM_MODE==PWM_IR_DMA_FIFO)
	if(reg_pwm_irq_sta & FLD_IRQ_PWM0_IR_DMA_FIFO_DONE){
		reg_pwm_irq_sta |= FLD_IRQ_PWM0_IR_DMA_FIFO_DONE;
		ir_dma_fifo_cnt++;
	}
#endif
}

/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
int main (void)
{
	cpu_wakeup_init();

	clock_init(SYS_CLK);

	rf_drv_init(RF_MODE_BLE_1M);

	gpio_init(1);

	user_init();

	while (1) {
		main_loop ();
	}
	return 0;
}



