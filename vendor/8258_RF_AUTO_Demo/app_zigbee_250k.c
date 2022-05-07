/********************************************************************************************************
 * @file     app.c
 *
 * @brief    This is the RF file for TLSR8258
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

#if(RF_MODE==RF_ZIGBEE_250K)

#define TX_FIRST				1
#define RX_FIRST				2
#define RF_STRX_MODE			TX_FIRST

#define STX_WAITTIME_US         4000
#define SRX_WAITTIME_US         100000
#define TX_DELAY_US             10

#define RF_FREQ					42
#define RF_POWER				RF_POWER_P10p46dBm


volatile unsigned int rx_cnt=0;
volatile unsigned int tx_cnt=0;
volatile unsigned int timeout_cnt=0;
volatile unsigned int tx_state=0;
volatile unsigned int rx_state=0;
volatile unsigned int timeout_state=0;
unsigned char  rx_packet[64]  __attribute__ ((aligned (4)));
unsigned char  Zigbee_tx_packet[48] __attribute__ ((aligned (4))) = {0x12,0x00,0x00,0x00,0x13,0x00,0x00,0x00,0x00,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};



void user_init()
{
	//1.init the LED pin,for indication
	gpio_set_func(LED1|LED2|LED3,AS_GPIO);
	gpio_set_output_en(LED1|LED2|LED3, 1); //enable output
	gpio_set_input_en(LED1|LED2|LED3 ,0);	 //disable input
	gpio_write(LED1|LED2|LED3, 0);         //LED Off

	rf_set_power_level_index (RF_POWER);
	rf_trx_state_set(RF_MODE_AUTO,RF_FREQ);
	rf_rx_buffer_set(rx_packet,64, 0);

#if(RF_STRX_MODE==TX_FIRST)
    irq_disable();
    irq_clr_src();
    irq_enable_type(FLD_IRQ_ZB_RT_EN); //enable RF irq
    rf_irq_disable(FLD_RF_IRQ_ALL);
    rf_irq_enable(FLD_RF_IRQ_TX|FLD_RF_IRQ_RX|FLD_RF_IRQ_RX_TIMEOUT);//stx2rx irq interrupt
    irq_enable(); //enable general irq
#elif(RF_STRX_MODE==RX_FIRST)
    irq_disable();
    irq_clr_src();
    irq_enable_type(FLD_IRQ_ZB_RT_EN); //enable RF irq
    rf_irq_disable(FLD_RF_IRQ_ALL);
    rf_irq_enable(FLD_RF_IRQ_TX|FLD_RF_IRQ_RX|FLD_RF_IRQ_FIRST_TIMEOUT);//srx2tx irq interrupt
    irq_enable(); //enable general irq
#endif
}

void main_loop (void)
{
#if(RF_STRX_MODE==TX_FIRST)
	reg_rf_rx_timeout = SRX_WAITTIME_US;
	while(1)
	{
		tx_state=0;
		rx_state=0;
		timeout_state=0;
		sleep_ms(100);
		rf_start_stx2rx (Zigbee_tx_packet, clock_time()+16*TX_DELAY_US);
		while(1)
		{
			if(tx_state==1)//tx
			{
				tx_state = 0;
				tx_cnt++;
			}
			else if(rx_state==1)//rx
			{
				if(RF_ZIGBEE_PACKET_CRC_OK(rx_packet)&&RF_ZIGBEE_PACKET_LENGTH_OK(rx_packet))
					rx_cnt++;
				break;

			}
			else if(timeout_state==1)//time out
			{
				timeout_cnt++;
				break;
			}
		}
	}


#elif(RF_STRX_MODE==RX_FIRST)

	while(1)
	{
		tx_state=0;
		rx_state=0;
		timeout_state=0;
		rf_start_srx2tx (Zigbee_tx_packet, clock_time()+16*TX_DELAY_US);
		while(1)
		{
			if(tx_state==1)//tx
			{
				tx_state = 0;
				tx_cnt++;
				break;
			}
			else if(rx_state==1)//rx (check crc & length)
			{
				rx_state=0;
				if(RF_ZIGBEE_PACKET_CRC_OK(rx_packet)&&RF_ZIGBEE_PACKET_LENGTH_OK(rx_packet))
					rx_cnt++;
			}
			else if(timeout_state==1)//time out
			{
				timeout_state = 0;
				timeout_cnt++;
				break;
			}
		}
	}
#endif
}

#endif


