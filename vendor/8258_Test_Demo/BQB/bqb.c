/********************************************************************************************************
 * @file     bqb.c
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

#include "bqb.h"

#if(TEST_DEMO==BQB_DEMO)


static unsigned short pkt_cnt =0,cmd_pkt,l, h;
static unsigned char chn, pkt_type,freq,uart_tx_index,uart_rx_index,para, ctrl;
static unsigned char bb_mode;
static unsigned int pkt_interval;
static unsigned int tick_rx = 0;
volatile unsigned int t0,tick_tx;
Test_Status_t test_state;

#if ANTI_NOISE_TEST
static signed char noise_value = -100;
#endif


unsigned char	bqbtest_buffer[256] __attribute__ ((aligned (4)));
u8 bqbtest_pkt [64] = {
	39, 0, 0, 0,
	0, 37,
	0, 1, 2, 3, 4, 5, 6, 7
};


static union pkt_length_u
{
	unsigned char len;
	struct len_t
	{
		unsigned char low:6;
		unsigned char upper:2;
	}l;
}pkt_length;

#if ANTI_NOISE_TEST
/**
 * @brief   This function serves to get noise value, and it must be used following the function "rf_start_srx"
 * @return  noise value, unit(dBm)
 */
signed char get_noise_value(void)
{
	while(rf_rssi_get_154() == -110);
	WaitUs(5);
	signed int sum_rf_rssi = 0;
	for(int i = 0; i < 10; i++)
	{
		sum_rf_rssi += rf_rssi_get_154();
		WaitUs(1);
	}

	return sum_rf_rssi / 10;
}
#endif
/**
 * @brief   This function serves to get ble channel index
 * @param   chn: input channel
 * @return  ble channel index
 */
unsigned char bqbtest_channel (unsigned char chn)
{
	if (chn == 0)
	{
		return 37;
	}
	else if (chn < 12)
	{
		return chn - 1;
	}
	else if (chn == 12)
	{
		return 38;
	}
	else if (chn < 39)
	{
		return chn - 2;
	}
	else
	{
		return 39;
	}
}

/**
 * @brief   This function serves to obtain the pkt interval in different payload lengths and different modes
 * @param   payload_len: payload length
 * @param   mode: tx mode
 * 				1:BLE1M
 * 				2:BLE2M
 * 				3:BLE125K
 * 				4:BLE500K
 * @return  the pkt interval
 */
unsigned int get_pkt_interval(unsigned char payload_len, unsigned char mode)
{
	unsigned int total_len,byte_time=8;
	unsigned char preamble_len;

	preamble_len = read_reg8(0x402) & 0x1f ;
	total_len = preamble_len + 4 + 2 + payload_len +3; // preamble + access_code + header + payload + crc

	if(mode==1)//1m
	{
		byte_time = 8;
	}
	else if(mode==2)//2m
	{
		byte_time = 4;
	}
	else if(mode==3)//s=8
	{
		byte_time = 64;
	}
	else if(mode==4) // s=2
	{
		byte_time = 16;
	}
	return (((byte_time * total_len + 249  + 624)/625)*625);

}


/**
 * @brief   This function serves to read the usrt data and execute BQB program
 * @param   Pointer to uart data
 * @return  1:  2 bytes data is received.
 * 			0:  no data is received.
 */
unsigned short uart_bqbtest_get(unsigned short* cmd)
{
	if (!tick_rx && ((reg_uart_buf_cnt&FLD_UART_RX_BUF_CNT) == 1))
	{
		tick_rx = reg_system_tick;
	}
	else if ((reg_uart_buf_cnt&FLD_UART_RX_BUF_CNT) == 2)
	{
		h = reg_uart_data_buf(uart_rx_index)&0xff;
		uart_rx_index++;
		uart_rx_index &= 0x03;
		l = reg_uart_data_buf(uart_rx_index)&0xff;
		uart_rx_index++;
		uart_rx_index &= 0x03;

		*cmd = (l | (h<<8));

		tick_rx = 0;
		return 1;
	}
	else if (tick_rx && clock_time_exceed(tick_rx, 5000))
	{
		tick_rx = 0;
		reg_uart_data_buf(uart_rx_index);
		uart_rx_index++;
		uart_rx_index &= 0x03;
	}
	else if((reg_uart_buf_cnt&FLD_UART_RX_BUF_CNT)>2)
	{
		unsigned char i;

		unsigned char l = REG_ADDR8(0x9c)&0x0f;
		for(i=0; i<l; i++)
		{
			reg_uart_data_buf(uart_rx_index);
			uart_rx_index++;
			uart_rx_index &= 0x03;
		}
	}
	return 0;
}

/**
 * @brief   This function serves to generate random packets that need to be sent in burst mode
 * @param   *p - the address of random packets.
 * @param   n - the number of random packets.
 * @return  none.
 */
static void rf_phy_test_prbs9 (unsigned char *p, int n)
{
	//PRBS9: (x >> 1) | (((x<<4) ^ (x<<8)) & 0x100)
	unsigned short x = 0x1ff;
	int i;
	int j;
	for ( i=0; i<n; i++)
	{
		unsigned char d = 0;
		for (j=0; j<8; j++)
		{
			if (x & 1)
			{
				d |= BIT(j);
			}
			x = (x >> 1) | (((x<<4) ^ (x<<8)) & 0x100);
		}
		*p++ = d;
	}
}


/**
 * @brief   This function serves to read the usrt data and execute BQB program
 * @param   none.
 * @return  none.
 */
void bqb_serviceloop (void)
{
	if (uart_bqbtest_get (&cmd_pkt))
	{
		unsigned short rsp=0;
		unsigned char cmd = cmd_pkt >> 14;
		unsigned char k;
//		gpio_write(LED1, !gpio_read(LED1));
		gpio_toggle(LED1);
		tick_tx =  reg_system_tick;
		switch(cmd)
		{
			case CMD_SETUP:
			{
#if USER_REDEFINE_PA
				gpio_write(BQB_PA_RX_PORT, 0);
				gpio_write(BQB_PA_TX_PORT, 0);
#endif
				ctrl = (cmd_pkt >> 8)&0x3f;
				para = (cmd_pkt >> 2)&0x3f;
				if(ctrl==0)
				{
					if(para==0)
					{
						pkt_length.l.upper =0;
						rsp = 0;
					}
					else
					{
						rsp = BIT(0);//Error
					}
					rf_drv_init(RF_MODE_BLE_1M_NO_PN);
					write_reg8(0x402,BQB_PREAMBLE_LEN+0x40); //add by junwei
				}
				else if(ctrl== 1)
				{
					if((para>=0) && (para<=3))
					{
						pkt_length.l.upper = para &0x03;
					}
					else
					{
						rsp = BIT(0); //Error
					}
				}
				else if(ctrl==2)
				{
					if(para==1)//BLE 1M
					{
						rf_drv_init(RF_MODE_BLE_1M_NO_PN);
//						rf_drv_init(RF_MODE_LR_S8_125K);
						rsp = 0;
					}
					else if(para==2)//BLE 2M
					{
						rf_drv_init(RF_MODE_BLE_2M);
						rsp = 0;
					}
					else if(para==3)//s=8
					{
						rf_drv_init(RF_MODE_LR_S8_125K);
						rsp = 0;
					}
					else if(para==4)//s=2
					{
						rf_drv_init(RF_MODE_LR_S2_500K);
						rsp = 0;
					}
					else
					{
						rsp = BIT(0);
					}
					rf_pn_disable();
					bb_mode = para;
				}
				else if(ctrl==3)
				{
					rsp = 0;
				}
				else if(ctrl==4)
				{
					rsp |= BIT(1) | BIT(2);
					rsp = (rsp <<1);
				}
				else if(ctrl==5)
				{
					if(para==0)
					{
						rsp = (251<<1)&0x7ffe;
					}
					else if(para==1)
					{
						rsp = (17040 << 1)&0x7ffe;
					}
					else if(para==2)
					{
						rsp = (251<<1)&0x7ffe;
					}
					else if(para==3)
					{
						rsp = (17040 << 1)&0x7ffe;
					}
					else
					{
						rsp = BIT(0);//status EVENT Error
					}
				}
				pkt_cnt = 0;
				test_state = SETUP_STATE;
				bqb_uart_ndma_send_byte((rsp>>8)&0x7f);
				bqb_uart_ndma_send_byte(rsp&0xff);
				break;
			}
			case CMD_RX_TEST:
			{
#if USER_REDEFINE_PA
				gpio_write(BQB_PA_RX_PORT, 1);
				gpio_write(BQB_PA_TX_PORT, 0);
#endif
				chn = (cmd_pkt >> 8) & 0x3f;	//frequency
				pkt_length.l.low  = (cmd_pkt >> 2) & 0x3f;
				pkt_type = cmd_pkt & 0x03;
				freq = bqbtest_channel(chn);//set channel

				rf_set_ble_channel(freq);
				rf_start_srx(reg_system_tick);

#if ANTI_NOISE_TEST
				noise_value = get_noise_value();
				if(noise_value >= (DB_NOISE_SIGNAL - RSSI_MAX_GAP))
				{
					test_state = END_STATE;
				}
				else
				{
					bqb_uart_ndma_send_byte((rsp>>8)&0xff);
					bqb_uart_ndma_send_byte(rsp&0xff);
					pkt_cnt = 0;
					test_state = RX_STATE;
				}
#else
				bqb_uart_ndma_send_byte((rsp>>8)&0xff);
				bqb_uart_ndma_send_byte(rsp&0xff);
				pkt_cnt = 0;
				test_state = RX_STATE;
#endif
				break;
			}
			case CMD_TX_TEST:
			{
#if USER_REDEFINE_PA
				gpio_write(BQB_PA_RX_PORT, 0);
				gpio_write(BQB_PA_TX_PORT, 1);
#endif
				chn = (cmd_pkt >> 8) & 0x3f;	//frequency
				pkt_length.l.low  = (cmd_pkt >> 2) & 0x3f;
				pkt_type = cmd_pkt & 0x03;

				if(pkt_length.len<=253)
				{
				bqbtest_pkt[0] = pkt_length.len + 2;
				bqbtest_pkt[1] = 0;
				}
				else
				{
					bqbtest_pkt[0] = pkt_length.len + 2-256;
					bqbtest_pkt[1] = 1;
				}

				bqbtest_pkt[5] = pkt_length.len ;
				if(pkt_type==0)
				{

					rf_phy_test_prbs9 (bqbtest_pkt + 6, pkt_length.len);
				}
				else if(pkt_type==1)
				{

					for(k=0;k<(pkt_length.len);k++)
					{
						bqbtest_pkt[k+6] = 0x0f;
					}

				}
				else if(pkt_type==2)
				{

					for(k=0;k<(pkt_length.len);k++)
						{
							bqbtest_pkt[k+6] = 0x55;
						}

				}
				else if(pkt_type==3)
				{
					pkt_type = 4;

					//todo if not LE Coded
					for(k=0;k<(pkt_length.len);k++)
					{
						bqbtest_pkt[k+6] = 0xff;
					}

				}
				else
				{
					rsp = BIT(0);
				}
				pkt_interval = get_pkt_interval(pkt_length.len, bb_mode);//TODO

				bqbtest_pkt[4] = pkt_type;
				freq = bqbtest_channel(chn);//set channel
				REG_ADDR8(0xf00) = 0x80;
				rf_set_ble_channel(freq);
				rf_set_power_level_index(BQB_TX_POWER);
				test_state = TX_STATE;

				bqb_uart_ndma_send_byte((rsp>>8)&0xff);
				bqb_uart_ndma_send_byte(rsp&0xff);

				pkt_cnt = 0;
				break;
			}
			case CMD_END:
			{
#if USER_REDEFINE_PA
				gpio_write(BQB_PA_RX_PORT, 0);
				gpio_write(BQB_PA_TX_PORT, 0);
#endif
				ctrl = (cmd_pkt >> 8)&0x3f;
				para = (cmd_pkt >> 2)&0x3f;

#if (!RETURN_TX_LEN_EN)
				if(test_state==TX_STATE)//The tx quantity does not need to be reported
					pkt_cnt = 0;
#endif
				test_state = END_STATE;
				if((ctrl==0) && (para==0))
				{
					pkt_length.len =0;
					REG_ADDR8(0xf00) = 0x80;

					bqb_uart_ndma_send_byte((BIT(7))|((pkt_cnt>>8)&0x7f));
					bqb_uart_ndma_send_byte(pkt_cnt&0xff);

				}

				break;
			}
			default:
				break;
		}
	}
	if(test_state==RX_STATE)
	{
		if (read_reg8(0xf20) & BIT(0))
		{
			BM_SET(REG_ADDR8(0xf20) , BIT(0));
			if((REG_ADDR8(0x44f)&0x0f)==0)
			{
				pkt_cnt++;
			}
			rf_start_srx(reg_system_tick);
#if ANTI_NOISE_TEST
			noise_value = get_noise_value();
			if(noise_value >= (DB_NOISE_SIGNAL - RSSI_MAX_GAP))
			{
				test_state = END_STATE;
			}
#endif
		}
	}
	else if(test_state==TX_STATE)
	{
		if (clock_time_exceed(tick_tx,pkt_interval) || (pkt_cnt == 0))//pkt_interval
		{
			tick_tx =  reg_system_tick;
			REG_ADDR8(0xf00) = 0x80;
			rf_start_stx(bqbtest_pkt,reg_system_tick);
			pkt_cnt++;
		}
	}


}


/**
 * @brief   This function serves to initialize  BQB
 * @param   none.
 * @return  none.
 */
void  bqbtest_init(void)
{
	unsigned char chnidx=0;
	t0 = reg_system_tick;
	rf_phy_test_prbs9 (bqbtest_pkt + 6, 37);
	rf_rx_buffer_set(bqbtest_buffer,256,0);
	write_reg8(0x800f03,0x1c); //disable first timeout
	rf_drv_init(RF_MODE_BLE_1M_NO_PN);
	write_reg8(0x800402, BQB_PREAMBLE_LEN);
	write_reg32(0x800408, 0x29417671);
	write_reg8(0x800405, REG_ADDR8(0x405)|0x80); //trig accesscode
	uart_tx_index=0;
	uart_rx_index=0;
	flash_read_page(0x77000,1,&chnidx);
	if(chnidx!=0xff)
	{
		chnidx&=0x3f;
		analog_write(0x8a,(analog_read(0x8a)&0xc0)|chnidx);//close internal cap
	}
}

/**
 * @brief     uart send data function with not DMA method.
 *            variable uart_TxIndex,it must cycle the four registers 0x90 0x91 0x92 0x93 for the design of SOC.
 *            so we need variable to remember the index.
 * @param[in] uartData - the data to be send.
 * @return    none
 */
void bqb_uart_ndma_send_byte(unsigned char uartData)
{
	int t;
//	static unsigned char uart_TxIndex = 0;

	t = 0;
	while( uart_tx_is_busy() && (t<0xfffff))
	{
		t++;
	}
	if(t >= 0xfffff)
		return;

	reg_uart_data_buf(uart_tx_index) = uartData;

	uart_tx_index++;
	uart_tx_index &= 0x03;// cycle the four register 0x90 0x91 0x92 0x93.
}



#endif
