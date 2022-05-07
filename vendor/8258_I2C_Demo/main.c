/********************************************************************************************************
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

int irq_cnt = 0;
int i2c_read_cnt = 0;
int i2c_write_cnt = 0;
/**
 * @brief		This function serves to handle the interrupt of MCU
 * @param[in] 	none
 * @return 		none
 */
_attribute_ram_code_ void irq_handler(void)
{
	irq_cnt ++;

	unsigned char  irq_status = reg_i2c_slave_irq_status;

	if(irq_status & FLD_HOST_CMD_IRQ)
	{
		reg_i2c_slave_irq_status = irq_status; //clear all irq status

		if(irq_status & FLD_HOST_READ_IRQ)
		{
			i2c_read_cnt ++;
			gpio_toggle(LED1);
		}
		else
		{
			i2c_write_cnt ++;
			gpio_toggle(LED2);
		}
	}
}

/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
u16 cnt;
u8 RD_i2c_tx_buff[2]={0x35,0x17};
u8 RD_i2c_tx_buff1[2]={0x5c,0x24};
int main (void)   //must on ramcode
{
	cpu_wakeup_init();

	clock_init(SYS_CLK);

	rf_drv_init(RF_MODE_BLE_1M);

	gpio_init(1);

	user_init();
	i2c_gpio_set(I2C_GPIO_GROUP_C2C3);
	i2c_master_init(0xe0,(unsigned char)(CLOCK_SYS_CLOCK_HZ/(4*100000)));
	while (1) {
		main_loop ();
		cnt++;
		if(cnt>=40000){

			i2c_write_series(0,0,(u8 *)RD_i2c_tx_buff, 2);
			sleep_us(180);
			i2c_write_series(0,0,(u8 *)RD_i2c_tx_buff1, 2);
			//i2c_read_series(OPT3001_RESULT_REGISTER,OPT3001_RESULT_REGISTER_LEN, (u8 *)i2c_rx_buff, RX_BUFF_DATA_LEN);
			cnt=0;
		}
	}
	return 0;
}

