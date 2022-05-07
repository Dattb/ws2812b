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

unsigned long firmwareVersion;

#define SPI_MASTER_DEVICE		1
#define SPI_SLAVE_DEVICE		2
#define SPI_DEVICE				1


#if (SPI_DEVICE==SPI_MASTER_DEVICE)

#define BUFF_DATA_LEN    		16
#define SPI_CS_PIN				GPIO_PD6

#define SPI_READ_CMD			0x80
#define SPI_WRITE_CMD			0x00
#define SLAVE_ADDR				0x41024
#define CMD_BUF_LEN				4
unsigned char cmd_buf[4];

volatile unsigned char spi_tx_buff[BUFF_DATA_LEN]={0xAA,0x10,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xBB};
volatile unsigned char spi_rx_buff[BUFF_DATA_LEN]={0x00};

void user_init()
{
	sleep_ms(2000);
	//1.init the LED pin,for indication
	gpio_set_func(LED1 ,AS_GPIO);
	gpio_set_output_en(LED1, 1); 		//enable output
	gpio_set_input_en(LED1 ,0);			//disable input
	gpio_write(LED1, 0);              	//LED On

	spi_master_init((unsigned char)(CLOCK_SYS_CLOCK_HZ/(2*500000)-1),SPI_MODE0);
#if SHARE_MODE
	spi_set_share_mode_en();   //Connect the MASTER's DI and the slave's DI if you want to use this mode
#endif
	spi_master_gpio_set(SPI_GPIO_GROUP_A2A3A4D6);
}
void main_loop (void)
{
	sleep_ms(1000);
	spi_tx_buff[2] ++;
	gpio_toggle(LED1);
	cmd_buf[0]= (SLAVE_ADDR>>16)&0xff;
	cmd_buf[1]= (SLAVE_ADDR>>8)&0xff;
	cmd_buf[2]= SLAVE_ADDR&0xff;
	cmd_buf[3]= SPI_WRITE_CMD;
	spi_write((unsigned char*)cmd_buf, CMD_BUF_LEN,(unsigned char*)spi_tx_buff, BUFF_DATA_LEN,SPI_CS_PIN);
	cmd_buf[3]= SPI_READ_CMD;
	spi_read((unsigned char*)cmd_buf, CMD_BUF_LEN,(unsigned char*)spi_rx_buff, BUFF_DATA_LEN,SPI_CS_PIN);
}

#elif(SPI_DEVICE== SPI_SLAVE_DEVICE)

void user_init()
{
	sleep_ms(2000);

	//1.init the LED pin,for indication
	gpio_set_func(LED1 ,AS_GPIO);
	gpio_set_output_en(LED1, 1); 		//enable output
	gpio_set_input_en(LED1 ,0);			//disable input
	gpio_write(LED1, 0);              	//LED On

	gpio_set_func(LED2 ,AS_GPIO);
	gpio_set_output_en(LED2, 1); 		//enable output
	gpio_set_input_en(LED2 ,0);			//disable input
	gpio_write(LED2, 0);              	//LED On

	spi_slave_init((unsigned char)(CLOCK_SYS_CLOCK_HZ/(2*500000)-1),SPI_MODE0);
#if SHARE_MODE
	spi_set_share_mode_en();  //Connect the MASTER's DI and the slave's DI if you want to use this mode
#endif
	spi_slave_gpio_set(SPI_GPIO_GROUP_A2A3A4D6);
	reg_irq_mask |= FLD_IRQ_MIX_CMD_EN;
	irq_enable();
}
void main_loop (void)
{
	sleep_ms(50);
}

#endif

