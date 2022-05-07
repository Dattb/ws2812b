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
 *
 *******************************************************************************************************/
#include "app_config.h"


#define RF_AOD_EN		        1

#define TX     	                1
#define RX		                2
#define RF_TX_RX_MODE      		RX



#define RF_triangle_1357     	1
#define RF_triangle_2468     	2
#define RF_eight_all_open		3
#define RX_RF_ARRAY    			RF_eight_all_open


#define LED2     				GPIO_PA3
#define LED3     				GPIO_PA4
#define LED4     				GPIO_PB0
#define LED5     				GPIO_PB1
#define SW2     				GPIO_PC1
#define SW3     				GPIO_PC0

#define V1						GPIO_PC6
#define V2						GPIO_PC5
#define V3						GPIO_PC7

#define RF_FREQ					RF_REAL_FREQ-2400					// TX/RX frequency
#define RF_POWER				RF_POWER_P10p46dBm  // TX Power
#define ACCESS_CODE				0xbbcaf1b1	        // Connect access code
#define TX_INTERVAL				400000  			// TX interval,16000000 means 1 second

volatile unsigned int rx_cnt=0;
volatile unsigned int tx_cnt=0;
unsigned int tick_now;
int angle_value;
unsigned char raw_data[90];
extern unsigned int rsquare;

unsigned char  rx_packet[128]  __attribute__ ((aligned (4)));
// AOD Tx data
unsigned char  ble_tx_packet_AOD[48] __attribute__ ((aligned (4))) = {
0x15,0x00,0x00,0x00,0xed,0x12,0x94,0x30,0x31,0x32,0x33,0x34,
0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,0x40,0x41	 };
// AOA Tx data
unsigned char  ble_tx_packet_AOA[48] __attribute__ ((aligned (4))) = {
0x15,0x00,0x00,0x00,0xed,0x12,0x14,0x30,0x31,0x32,0x33,0x34,
0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,0x40,0x41	 };

void init_test_gpio(void)
{

	gpio_set_func(GPIO_PA5, AS_USB);
	gpio_set_func(GPIO_PA6, AS_USB);
	usb_dp_pullup_en (1);

	//1.init the LED pin,for indication
	gpio_set_func(LED2 ,AS_GPIO);
	gpio_set_output_en(LED2, 1); //enable output
	gpio_set_input_en(LED2 ,0);//disable input
	gpio_write(LED2, 0);              //LED On

	gpio_set_func(LED3 ,AS_GPIO);
	gpio_set_output_en(LED3, 1); //enable output
	gpio_set_input_en(LED3 ,0);//disable input
	gpio_write(LED3,0);              //LED On

	gpio_set_func(LED4 ,AS_GPIO);
	gpio_set_output_en(LED4, 1); //enable output
	gpio_set_input_en(LED4 ,0);//disable input
	gpio_write(LED4,0);              //LED On

	gpio_set_func(LED5 ,AS_GPIO);
	gpio_set_output_en(LED5, 1); //enable output
	gpio_set_input_en(LED5 ,0);//disable input
	gpio_write(LED5,0);              //LED On

	gpio_set_func(SW2 ,AS_GPIO);
	gpio_set_output_en(SW2, 0); //enable output
	gpio_set_input_en(SW2 ,1);//disable input            //LED On
	gpio_setup_up_down_resistor(SW2,PM_PIN_PULLUP_10K);

	gpio_set_func(SW3 ,AS_GPIO);
	gpio_set_output_en(SW3, 0); //enable output
	gpio_set_input_en(SW3 ,1);//disable input
	gpio_write(SW3,0);              //LED On
	gpio_setup_up_down_resistor(SW2,PM_PIN_PULLUP_10K);
}

void triangle_rf1357(void)
{
	/*
	 * RF1357,V3 = 0
	 */
	sub_wr(0x596, 0, 6, 5); //disable act as gpio
	sub_wr(0x5ad, 2, 3, 2); //c[5], antsel_0
	sub_wr(0x5ad, 1, 5, 4); //c[6], antsel_1
	sub_wr(0x438, 3, 6, 4); //ant_num logical num

	gpio_set_func(V3 ,AS_GPIO);
	gpio_set_output_en(V3, 1); //enable output
	gpio_set_input_en(V3 ,0);//disable input
	gpio_write(V3, 0);              //V3 = 0
}

void triangle_rf2468(void)
{
	/*
	 * RF1357,V3 = 1
	 */
	sub_wr(0x596, 0, 6, 5); //disable act as gpio
	sub_wr(0x5ad, 2, 3, 2); //c[5], antsel_0
	sub_wr(0x5ad, 1, 5, 4); //c[6], antsel_1
	sub_wr(0x438, 3, 6, 4); //ant_num logical num

	gpio_set_func(V3 ,AS_GPIO);
	gpio_set_output_en(V3, 1); //enable output
	gpio_set_input_en(V3 ,0);//disable input
	gpio_write(V3, 1);              //V3 = 1
}

void triangle_all_open(void)
{
	/*
	 * RF ALL OPEN
	 */
	sub_wr(0x596, 0, 7, 5); //disable act as gpio
	sub_wr(0x5ad, 2, 3, 2); //c[5], antsel_o
	sub_wr(0x5ad, 1, 5, 4); //c[6], antsel_1
	sub_wr(0x5ad, 1, 7, 6); //c[7], antsel_2
	sub_wr(0x438, 7, 6, 4); //ant_num logical num

}

void user_init()
{
	//1. init gpio and RF mode
	init_test_gpio();
	rf_drv_init(RF_MODE_BLE_1M_NO_PN);

	//2. select RF antenna
#if(RX_RF_ARRAY==RF_triangle_1357)
	triangle_rf1357();
#elif(RX_RF_ARRAY==RF_triangle_2468)
	triangle_rf2468();
#elif(RX_RF_ARRAY==RF_eight_all_open)
	triangle_all_open();
	init_lookup_table();
#endif

	//3. select TX/RX
#if(RF_TX_RX_MODE==TX)
	sub_wr(0x438, 1, 2, 2); 				// supp_tx_dat_en
#elif(RF_TX_RX_MODE==RX)
	sub_wr(0x438, 1, 0, 0);					// supp_rx_dat_en
#endif

	//4. other setting
	WRITE_REG8(0x43b,0x29);					// adjust sample point location .
	rf_set_power_level_index (RF_POWER);	// set power level
	rf_set_tx_rx_off();						// close tx/rx
	rf_set_channel(RF_FREQ,0);				// set channel
	rf_access_code_comm(ACCESS_CODE);			// set access code
}

void main_loop (void)
{
#if(RF_TX_RX_MODE==TX)
	rf_ble_tx_on();
	while(1)
	{
		while((unsigned int)(clock_time()-tick_now) < TX_INTERVAL);
		tick_now = clock_time();
		gpio_toggle(LED2);
		tx_cnt++;
		#if RF_AOD_EN
			ble_tx_packet_AOD[7]  =tx_cnt;
			rf_tx_pkt (ble_tx_packet_AOD);
		#else
			ble_tx_packet_AOA[7]  =tx_cnt;
			rf_tx_pkt (ble_tx_packet_AOA);
		#endif
		while(!rf_tx_finish());			// waiting for tx finish
		rf_tx_finish_clear_flag();					// clear tx finish flag
	}

#elif(RF_TX_RX_MODE==RX)

	rf_rx_buffer_set((unsigned char*)rx_packet,128, 0);	// init rx buffer
	rf_set_rxmode ();									// open rx

	while(1)
	{
		if(rf_is_rx_finish())
		{
			#if RF_AOD_EN
			if((rx_packet[0] == 0x7A) && ((REG_ADDR8(0x44f)&0x0f)==0)&& (rx_packet[4]==0xed)&&(rx_packet[6]==0x94))
			#else
			if((rx_packet[0] == 0x7A) && ((REG_ADDR8(0x44f)&0x0f)==0)&& (rx_packet[4]==0xed)&&(rx_packet[6]==0x14))
			#endif
			{
				#if(RX_RF_ARRAY==RF_eight_all_open)
					gpio_toggle(LED2);
					angle_value = get_input_angle_for_polygon(&rx_packet[0]);
					get_raw_data(&raw_data[0],&rx_packet[0],90);
					printf("angle = %d\n",angle_value);
					sleep_us(150);
				#else
					gpio_toggle(LED2);
					angle_value = get_input_angle(&rx_packet[0]);	//get input angle
					get_raw_data(&raw_data[0],&rx_packet[0],90);	//get raw data
					temp_value = angle_value;
					if(temp_value>128)
					{
						printf("angle = -%d\n",(256-(temp_value)));
					}
					else
					{
						printf("angle = %d\n",angle_value);
					}
				#endif

				rx_cnt++;
			}
			rf_rx_finish_clear_flag();
			rf_set_tx_rx_off_auto_mode();
		}
	}


#endif
}



