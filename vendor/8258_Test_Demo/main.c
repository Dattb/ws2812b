/********************************************************************************************************
 * @file     main.c
 *
 * @brief    This is the EMI file for TLSR8258
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

#if(TEST_DEMO==EMI_DEMO)
extern void led_serviceloop();
extern void key_scan();
extern void key_serviceloop(void);
#endif

/**
 * @brief		This function serves to handle the interrupt of MCU
 * @param[in] 	none
 * @return 		none
 */
_attribute_ram_code_ void irq_handler(void)
{

#if(TEST_DEMO==EMI_DEMO)
	if(reg_tmr_sta & FLD_TMR_STA_TMR0)
	{
		reg_tmr_sta = FLD_TMR_STA_TMR0; //clear irq status
		key_scan();
	}
#endif
}

/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
int main (void) {

	blc_pm_select_internal_32k_crystal();

	int deepRetWakeUp = pm_is_MCU_deepRetentionWakeup();  //MCU deep retention wakeUp

	cpu_wakeup_init();

	rf_drv_init(RF_MODE_BLE_1M_NO_PN);

	gpio_init(!deepRetWakeUp);

	clock_init(SYS_CLK);

	user_init();

	while (1)
	{
		main_loop ();
	}
	return 0;
}



