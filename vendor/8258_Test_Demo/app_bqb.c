/********************************************************************************************************
 * @file     app.c
 *
 * @brief    This is the BQB file for TLSR8258
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
#include "BQB/bqb.h"
#if(TEST_DEMO==BQB_DEMO)

#define BQB_UART_BUAD	   115200

void user_init(void)
{
	//1.init the LED pin,for indication
	gpio_set_func(LED1 ,AS_GPIO);
	gpio_set_output_en(LED1, 1); //enable output
	gpio_set_input_en(LED1 ,0);	 //disable input
	gpio_write(LED1, 0);         //LED On

	uart_gpio_set(BQB_UART_TX_PORT, BQB_UART_RX_PORT);// uart tx/rx pin set
	uart_reset();  //will reset uart digital registers from 0x90 ~ 0x9f, so uart setting must set after this reset
	uart_init_baudrate(BQB_UART_BUAD,CLOCK_SYS_CLOCK_HZ,PARITY_NONE, STOP_BIT_ONE);
	uart_dma_enable(0, 0);
	irq_disable_type(FLD_IRQ_DMA_EN);
	dma_chn_irq_enable(FLD_DMA_CHN_UART_RX | FLD_DMA_CHN_UART_TX, 0);
	uart_irq_enable(0,0);   //uart RX irq enable
	uart_ndma_irq_triglevel(1,0);	//set the trig level. 1 indicate one byte will occur interrupt
	bqbtest_init();
	//SET PA PORT
#if USER_REDEFINE_PA
	gpio_set_func(BQB_PA_TX_PORT ,AS_GPIO);
	gpio_set_output_en(BQB_PA_TX_PORT, 1); //enable output
	gpio_set_input_en(BQB_PA_TX_PORT ,0);	 //disable input
	gpio_write(BQB_PA_TX_PORT, 0);         //LED On
	gpio_set_func(BQB_PA_RX_PORT ,AS_GPIO);
	gpio_set_output_en(BQB_PA_RX_PORT, 1); //enable output
	gpio_set_input_en(BQB_PA_RX_PORT ,0);	 //disable input
	gpio_write(BQB_PA_RX_PORT, 0);         //LED On
#else
	rf_rffe_set_pin(BQB_PA_TX_PORT,BQB_PA_RX_PORT);
#endif
}

unsigned char temp_index = 0;
void main_loop(void)
{
	bqb_serviceloop();
}
#endif
