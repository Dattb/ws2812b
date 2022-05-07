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

#if (UART_MODE==UART_NDMA)


#define rec_buff_Len    16
#define trans_buff_Len  16

volatile unsigned char uart_rx_flag;
volatile unsigned int  uart_ndmairq_cnt;
volatile unsigned char uart_ndmairq_index;
volatile unsigned char uart_cts_count=0;
__attribute__((aligned(4))) unsigned char rec_buff[rec_buff_Len]={0};
__attribute__((aligned(4))) unsigned char trans_buff[trans_buff_Len] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
																		0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x00};
void user_init()
{
	sleep_ms(2000);  //leave enough time for SWS_reset when power on

	//note: dma addr must be set first before any other uart initialization! (confirmed by sihui)
	uart_recbuff_init( (unsigned short *)&rec_buff, sizeof(rec_buff));

	uart_gpio_set(UART_TX_PA2, UART_RX_PA0);// uart tx/rx pin set

	uart_reset();  //will reset uart digital registers from 0x90 ~ 0x9f, so uart setting must set after this reset

	//baud rate: 9600
	uart_init_baudrate(115200,CLOCK_SYS_CLOCK_HZ,PARITY_NONE, STOP_BIT_ONE);

#if( FLOW_CTR == NORMAL)

	uart_dma_enable(0, 0);

	irq_disable_type(FLD_IRQ_DMA_EN);

	dma_chn_irq_enable(FLD_DMA_CHN_UART_RX | FLD_DMA_CHN_UART_TX, 0);

	uart_irq_enable(1,0);   //uart RX irq enable

	uart_ndma_irq_triglevel(1,0);	//set the trig level. 1 indicate one byte will occur interrupt

#elif( FLOW_CTR ==  USE_CTS )
	//CTS pin.It can be A3/B2/C4/D1.
	uart_set_cts(1, STOP_VOLT, UART_CTS_PA3);

	uart_dma_enable(0, 0);				       //Disable DMA

	irq_disable_type(FLD_IRQ_DMA_EN);

	dma_chn_irq_enable(FLD_DMA_CHN_UART_RX | FLD_DMA_CHN_UART_TX, 0);	//Disable DMA irq

	uart_irq_enable(0,0);   //uart RX irq disable



#elif( FLOW_CTR ==   USE_RTS )
	// RTS pin : A4  B3 B6 C0
	uart_set_rts(1, RTS_MODE, RTS_THRESH, RTS_INVERT,UART_RTS_PA4);
	uart_dma_enable(0, 0);

	irq_disable_type(FLD_IRQ_DMA_EN);
	dma_chn_irq_enable(FLD_DMA_CHN_UART_RX | FLD_DMA_CHN_UART_TX, 0);

	uart_irq_enable(1,0);   //uart RX irq enable

	uart_ndma_irq_triglevel(RTS_THRESH,0);


#endif
}

/////////////////////////////////////////////////////////////////////
// main loop flow
/////////////////////////////////////////////////////////////////////
void main_loop (void)
{
	sleep_ms(1000);
#if( FLOW_CTR == NORMAL)

	for(unsigned char i=0;i<trans_buff_Len;i++){
		uart_ndma_send_byte(trans_buff[i]);
	}
	if(uart_rx_flag>0){
		uart_ndmairq_cnt=0; //Clear uart_ndmairq_cnt
		uart_rx_flag=0;
		for(unsigned char i=0;i<trans_buff_Len;i++){
			uart_ndma_send_byte(rec_buff[i]);
		}
	}
#elif( FLOW_CTR ==  USE_CTS )
	uart_ndma_send_byte(trans_buff[uart_cts_count]);
	uart_cts_count++;
	if(uart_cts_count == 16)
	{
		uart_cts_count=0;
	}

#elif( FLOW_CTR ==  USE_RTS )

	while(1);
#endif


}





#endif















