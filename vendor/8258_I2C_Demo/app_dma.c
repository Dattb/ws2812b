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

#if(I2C_MODE == I2C_DMA_MODE)

#define  	I2C_MASTER_DEVICE        	1   //i2c master demo
#define     I2C_SLAVE_DEVICE			2   //i2c master demo

#define     I2C_DEVICE					1

#define     BUFF_DATA_LEN				16
#define     SLAVE_DEVICE_ADDR			0x48000
#define     SLAVE_DEVICE_ADDR_LEN		3
#define     I2C_CLK_SPEED				200000
volatile unsigned char i2c_tx_buff[BUFF_DATA_LEN] = {0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};
volatile unsigned char i2c_rx_buff[BUFF_DATA_LEN] = {0};

void user_init()
{
	sleep_ms(2000);
	//1.init the LED pin,for indication
	gpio_set_func(LED1 ,AS_GPIO);
	gpio_set_output_en(LED1, 1); 		//enable output
	gpio_set_input_en(LED1 ,0);			//disable input
	gpio_write(LED1, 0);              	//LED Off

	gpio_set_func(LED2 ,AS_GPIO);
	gpio_set_output_en(LED2, 1); 		//enable output
	gpio_set_input_en(LED2 ,0);			//disable input
	gpio_write(LED2, 1);              	//LED On

#if(I2C_DEVICE == I2C_MASTER_DEVICE)

	//i2c_gpio_set(I2C_GPIO_GROUP_C0C1);  	// SDA/CK : C0/C1
	//i2c_master_init(0x5C, (unsigned char)(CLOCK_SYS_CLOCK_HZ/(4*I2C_CLK_SPEED)) ); // 200KHz

#elif(I2C_DEVICE == I2C_SLAVE_DEVICE)

	i2c_gpio_set(I2C_GPIO_GROUP_C0C1);  	//SDA/CK : C0/C1
	i2c_slave_init(0x5C, I2C_SLAVE_DMA,0);
	reg_irq_mask |= FLD_IRQ_MIX_CMD_EN; //enable I2C interrupt
	irq_enable();
#endif

}

void main_loop (void)
{
#if(I2C_DEVICE == I2C_MASTER_DEVICE)
//	sleep_ms(500);
//	i2c_tx_buff[0]++;
//	i2c_write_series(SLAVE_DEVICE_ADDR, SLAVE_DEVICE_ADDR_LEN, (unsigned char *)i2c_tx_buff, BUFF_DATA_LEN);
//	sleep_ms(500);
//	i2c_read_series(SLAVE_DEVICE_ADDR,  SLAVE_DEVICE_ADDR_LEN, (unsigned char *)i2c_rx_buff, BUFF_DATA_LEN);
//	gpio_toggle(LED1);
#endif

}

#endif
