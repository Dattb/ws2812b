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

#define WAKEUP_PAD						GPIO_PB0
#define CURRENT_TEST	     			0

void uart_drv_init()
{
#if(MCU_CORE_TYPE == MCU_CORE_8258 || (MCU_CORE_TYPE == MCU_CORE_8278))
//note: dma addr must be set first before any other uart initialization! (confirmed by sihui)
//	u8 *uart_rx_addr = hci_rx_fifo.p + (hci_rx_fifo.wptr & (hci_rx_fifo.num-1)) * hci_rx_fifo.size;
//	uart_recbuff_init( uart_rx_addr, hci_rx_fifo.size, uart_hw_tx_buf);
//	uart_gpio_set(UART_TX_PIN, UART_RX_PIN);

	uart_reset();  //will reset uart digital registers from 0x90 ~ 0x9f, so uart setting must set after this reset

	//baud rate: 115200
	#if (CLOCK_SYS_CLOCK_HZ == 16000000)
		uart_init(9, 13, PARITY_NONE, STOP_BIT_ONE);
	#elif (CLOCK_SYS_CLOCK_HZ == 24000000)
		uart_init(12, 15, PARITY_NONE, STOP_BIT_ONE);
	#elif (CLOCK_SYS_CLOCK_HZ == 32000000)
		uart_init(8, 30, PARITY_NONE, STOP_BIT_ONE);
	#elif (CLOCK_SYS_CLOCK_HZ == 48000000)
		uart_init(15, 25, PARITY_NONE, STOP_BIT_ONE);
	#endif

	uart_dma_enable(0, 0); 	//uart data in hardware buffer moved by dma, so we need enable them first

	irq_set_mask(FLD_IRQ_DMA_EN);
	dma_chn_irq_enable(FLD_DMA_CHN_UART_RX | FLD_DMA_CHN_UART_TX, 1);   	//uart Rx/Tx dma irq enable
#else

	//todo:uart init here
	uart_io_init(UART_GPIO_SEL);
#if(CLOCK_SYS_CLOCK_HZ == 32000000)
	CLK32M_UART115200;
#elif(CLOCK_SYS_CLOCK_HZ == 16000000)
	CLK16M_UART115200;
#endif
	uart_BuffInit(hci_rx_fifo_b, hci_rx_fifo.size, uart_hw_tx_buf);
#endif
}

void user_init()
{
	/*
	 * cai nay de cau hinh khi GPIO treo VCC
	 */
//	cpu_set_gpio_wakeup(GPIO_PD4, 0, 1);
//	gpio_setup_up_down_resistor(GPIO_PD4, PM_PIN_PULLUP_1M);
//	cpu_sleep_wakeup(DEEPSLEEP_MODE , PM_WAKEUP_PAD, 100);
	//sleep_ms(2000);

	/*
	 * cai nay de cau hinh khi GPIO treo am
	 * truoc khi cau hinh ntn can sua dien tro treo VCC o chan vdk
	 */

//		cpu_set_gpio_wakeup(GPIO_PB4, 1, 1);
//		gpio_setup_up_down_resistor(GPIO_PB4, PM_PIN_PULLDOWN_100K);
		//sleep_ms(2000);

#if CURRENT_TEST
	gpio_shutdown(GPIO_ALL);
#else
	//1.init the LED pin,for indication
	gpio_set_func(LED1 ,AS_GPIO);
	gpio_set_output_en(LED1, 1); 		//enable output
	gpio_set_input_en(LED1 ,0);			//disable input
	gpio_write(LED1, 0);              	//LED On

	gpio_write(LED1,1);
	sleep_ms(100);
	gpio_write(LED1,0);
	sleep_ms(100);
#endif

#if(PM_MODE==IDLE_TIMER_WAKEUP)



#elif(PM_MODE==SUSPEND_PAD_WAKEUP)
	/* Caution: if wake-up source is only pad, 32K clock source MUST be 32K RC * */

	cpu_set_gpio_wakeup(WAKEUP_PAD, Level_High, 1);
	gpio_setup_up_down_resistor(WAKEUP_PAD, PM_PIN_PULLDOWN_100K);

#elif(PM_MODE==DEEP_PAD_WAKEUP)
	/* Caution: if wake-up source is only pad, 32K clock source MUST be 32K RC * */

	cpu_set_gpio_wakeup(WAKEUP_PAD, Level_High, 1);
	gpio_setup_up_down_resistor(WAKEUP_PAD, PM_PIN_PULLDOWN_100K);

	cpu_sleep_wakeup(DEEPSLEEP_MODE , PM_WAKEUP_PAD, 0);

#elif(PM_MODE==DEEP_32K_RC_WAKEUP||PM_MODE==DEEP_32K_XTAL_WAKEUP)

    cpu_sleep_wakeup(DEEPSLEEP_MODE , PM_WAKEUP_TIMER,(clock_time() + 1000*CLOCK_16M_SYS_TIMER_CLK_1MS));

#elif(PM_MODE==DEEP_RET_PAD_WAKEUP)
	/* Caution: if wake-up source is only pad, 32K clock source MUST be 32K RC * */

	cpu_set_gpio_wakeup(WAKEUP_PAD, Level_High, 1);
	gpio_setup_up_down_resistor(WAKEUP_PAD, PM_PIN_PULLDOWN_100K);

	cpu_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW32K , PM_WAKEUP_PAD, 0);

#elif(PM_MODE==DEEP_RET_32K_RC_WAKEUP||PM_MODE==DEEP_RET_32K_XTAL_WAKEUP)

	cpu_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW32K , PM_WAKEUP_TIMER,(clock_time() + 1000*CLOCK_16M_SYS_TIMER_CLK_1MS));

#endif

}


/////////////////////////////////////////////////////////////////////
// main loop flow
/////////////////////////////////////////////////////////////////////
void main_loop (void)
{

#if(PM_MODE==IDLE_TIMER_WAKEUP)

	cpu_stall_wakeup_by_timer0(1000*CLOCK_SYS_CLOCK_1MS);

#elif(PM_MODE==SUSPEND_PAD_WAKEUP)

	cpu_sleep_wakeup(SUSPEND_MODE, PM_WAKEUP_PAD, 0);

#elif(PM_MODE==SUSPEND_32K_RC_WAKEUP||PM_MODE==SUSPEND_32K_XTAL_WAKEUP)

	cpu_sleep_wakeup(SUSPEND_MODE, PM_WAKEUP_TIMER, clock_time() + 1000*CLOCK_16M_SYS_TIMER_CLK_1MS);

#endif

#if CURRENT_TEST
	sleep_ms(3000);
#else
	gpio_write(LED1, 1);
	sleep_ms(100);
	gpio_write(LED1, 0);
	sleep_ms(100);
#endif

}



