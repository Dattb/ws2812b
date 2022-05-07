/********************************************************************************************************
 * @file     main.c 
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

extern void user_init();
extern void main_loop (void);

int spi_read_cnt = 0;
int spi_write_cnt = 0;
int SlaveRxLength = 0;
int irq_cnt = 0;
int spi_irq_cnt=0;
#define SPI_BUFFER_CNT 32
#define DataHead  0xAA
#define DataEnd   0xBB


/*The 'buff' is defined on the  "spi_slave_buff" section. The address of this section  varies with code size in the boot.link file.
  This address is also the starting address that the master can read and write, that is, "SLAVE_ADDR" in the demo.
  How to determine the specific address for "SLAVE_ADDR" in app.c file?
  step1:Compile the project in spi slave mode.
  step2:Find the ".mycode" section in the generated list file and the address corresponding to "buff" is "SLAVE_ADDR".

  The following is part of the contents of the list file.The SLAVE_ADDR is "0x841024".
  ****************************************
  Disassembly of section .my_code:
  00841024 <buff>:
  ****************************************


 */

volatile unsigned char  buff[SPI_BUFFER_CNT] __attribute__((section(".spi_slave_buff")));
unsigned char SlaveRxBuf[SPI_BUFFER_CNT];


/*SPI interrupt can be only used for Slave mode. When SPI Master writes data to Slave or reads data from Slave, SPI interrupt can be generated.
  Note that SPI interrupt flag is unable to distinguish whether this interrupt belongs to TX or RX.
  Master SPI Can send data according the next data transmission protocol to help Slave SPI judge whether this interrupt belongs to TX or RX.
  Data transmission protocol of Master SPI is as follows:
  DataHead DataLen xx xx xx  DataEnd
  eg. unsigned char TxBuffer[5]={0xAA,0x05,0x01,0x02,0xBB};
  DataHead = 0xAA ; DataLen =0x05; DataEnd= 0xBB;

 *The process of SPI interrupt in Slave Mode is as follows .
 *Step 1: Judge whether SPI interrupt generates.
 *Step 2: Judge DataHead is correct.
 *Step 3: Judge whether DataEnd is correct.
 *When all the above conditions are met,this process is that Master SPI writes data to the Slave.
 *When SPI interrupt generates and Step 2 and Step 3 can not meet the conditions, this process is that Master SPI reads data from Slave.

 */

void spi_slave_irq_handler(void)
{
	if(buff[0]==DataHead){//Judge whether DataHead is correct.
		SlaveRxLength= buff[1];
		if( buff[SlaveRxLength-1]==DataEnd ){//Judge whether DataEnd is correct.
		   spi_write_cnt++;  //
		   for(int i = 0;i<SlaveRxLength;i++){
			   SlaveRxBuf[i]=buff[i];//Get data that Master SPI writes to Slave.
			   buff[i]= 0;
		   }
		 }
	}
	else{ spi_read_cnt++;}
}
/**
 * @brief		This function serves to handle the interrupt of MCU
 * @param[in] 	none
 * @return 		none
 */
_attribute_ram_code_ void irq_handler(void)
{
	irq_cnt ++;

	unsigned char  irq_status = reg_spi_slave_irq_status;
	//SPI Interrupt means that every WRITE or READ will generate one interrupt(capture CS signal)
	if(irq_status & FLD_SLAVE_SPI_IRQ)
	{
		reg_spi_slave_irq_status = irq_status;
		spi_irq_cnt ++;
		spi_slave_irq_handler();
	}
}
/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
int main (void)   //must on ramcode
{
	cpu_wakeup_init();

	rf_drv_init(RF_MODE_BLE_1M);

	gpio_init(1);

	clock_init(SYS_CLK);

	user_init();

	irq_enable();

	while (1) {
		main_loop ();
	}
	return 0;
}

