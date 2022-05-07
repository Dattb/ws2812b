/********************************************************************************************************
 * @file     bqb.h
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

#ifndef BQB_H_
#define BQB_H_

#include "../app_config.h"

#if(TEST_DEMO==BQB_DEMO)

#define RETURN_TX_LEN_EN    0          			//1: return tx length, 0:not return tx length
/* set uart port */
#define BQB_UART_TX_PORT   	UART_TX_PD7
#define BQB_UART_RX_PORT   	UART_RX_PD6//UART_RX_PA0  UART_RX_PB0 UART_RX_PB7  UART_RX_PC3  UART_RX_PC5 UART_RX_PD6
/* set the length of preamble */
#define BQB_PREAMBLE_LEN   	6
/* set power of Tx */
#define BQB_TX_POWER		RF_POWER_P7p02dBm

#define AUTO 0

/**
 *  @brief  command type for BQB Test
 */
#define	CMD_SETUP			0
#define	CMD_RX_TEST			1
#define	CMD_TX_TEST			2
#define	CMD_END				3

/**
 *  @brief  Event type for BQB Test
 */
#define STATUS_EVENT_MASK	(0)
#define REPORT_EVENT_MASK	(BIT(15))

#define LED1     			GPIO_PD2

/**
 * @brief  Anti-noise test related
 */
#define ANTI_NOISE_TEST		1
#define RSSI_MAX_GAP		0
#define DB_NOISE_SIGNAL 	-70

/**
 * @brief User redefine PA
 */
#define USER_REDEFINE_PA	0
#if USER_REDEFINE_PA
#define BQB_PA_TX_PORT     GPIO_PD4
#define BQB_PA_RX_PORT     GPIO_PD3
#else
#define BQB_PA_TX_PORT     RFFE_TX_PD1
#define BQB_PA_RX_PORT     RFFE_RX_PD0
#endif


/**
 *  @brief  command status for BQB Test
 */
typedef enum {
	SETUP_STATE=0x10,
	RX_STATE,
	TX_STATE,
	END_STATE
}Test_Status_t;


/**
 * @brief   This function serves to initialize  BQB
 * @param   none.
 * @return  none.
 */
void  bqbtest_init(void);



/**
 * @brief   This function serves to read the usrt data and execute BQB program
 * @param   none.
 * @return  none.
 */
void bqb_serviceloop (void);


/**
 * @brief     uart send data function with not DMA method.
 *            variable uart_TxIndex,it must cycle the four registers 0x90 0x91 0x92 0x93 for the design of SOC.
 *            so we need variable to remember the index.
 * @param[in] uartData - the data to be send.
 * @return    none
 */
void bqb_uart_ndma_send_byte(unsigned char uartData);
#endif

#endif /* BQB_H_ */
