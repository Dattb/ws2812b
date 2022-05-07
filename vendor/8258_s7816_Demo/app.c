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


volatile __attribute__((aligned(4))) unsigned char trans_buff[16] = {0x05,0x00,0x00,0x00,0xFF,0x10,0x12,0xFD,0x00};
volatile __attribute__((aligned(4))) unsigned char recvBuff[48] = {0x00};
#define S7816_MODE_RX			0
#define S7816_MODE_TX			1
#define S7816_TRX_MODE			1


void user_init()
{
	sleep_ms(2000);  //leave enough time for SWS_reset when power on
	/************  1.initiate receive buff and 7816 clk  *******/
	s7816_set_clk(0x20);
	s7816_set_rx_buf((unsigned short *)&recvBuff,sizeof(recvBuff));
	/************  2.set pin for 7816  *******/
	s7816_set_pin(S7816_TRX_D7);
	/************  3.set baudrate for 7816  *******/
	uart_reset();
	uart_init(249, 9,PARITY_EVEN,STOP_BIT_ONE);//9600 in 24M system clk
	/************  4.set dma interrupt for 7816  *******/
	uart_dma_enable(1, 1); 	//uart data in hardware buffer moved by dma, so we need enable them first
	irq_enable_type(FLD_IRQ_DMA_EN);
	dma_chn_irq_enable(FLD_DMA_CHN_UART_RX | FLD_DMA_CHN_UART_TX, 1);   	//uart Rx/Tx dma irq enable
	uart_irq_enable(0, 0);  	//uart Rx/Tx irq no need, disable the
	/************  4.set half duplex mode for 7816  *******/
#if(S7816_TRX_MODE==S7816_MODE_RX)
	s7816_set_half_duplex(S7816_RX);
#elif(S7816_TRX_MODE==S7816_MODE_TX)
	s7816_set_half_duplex(S7816_TX);
#endif
}

void main_loop (void)
{
#if(S7816_TRX_MODE==S7816_MODE_RX)

#elif(S7816_TRX_MODE==S7816_MODE_TX)
	s7816_dma_send((unsigned char*)trans_buff);
	sleep_ms(2000);
#endif

}



