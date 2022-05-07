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


#define FLASH_ADDR				0x020000
#define FLASH_BUFF_LEN			256

volatile unsigned char Flash_Read_Buff[FLASH_BUFF_LEN]={0};
volatile unsigned char Flash_Write_Buff[FLASH_BUFF_LEN]=
{		0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,
		0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,
		0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,
		0xb0,0xb1,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,
		0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,
		0xd0,0xd1,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,
		0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,
		0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,


		0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,
		0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,
		0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,
		0xb0,0xb1,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,
		0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,
		0xd0,0xd1,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,
		0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,
		0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,

		0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,
		0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,
		0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,
		0xb0,0xb1,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,
		0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,
		0xd0,0xd1,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,
		0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,
		0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,

		0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,
		0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,
		0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,
		0xb0,0xb1,0xb2,0xb3,0xb4,0xb5,0xb6,0xb7,
		0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,
		0xd0,0xd1,0xd2,0xd3,0xd4,0xd5,0xd6,0xd7,
		0xe0,0xe1,0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,
		0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,
};

volatile unsigned char status;
unsigned char mid_buf[4]={0};
unsigned char uid_buf[16]={0};
unsigned char buf1[300]={0};
unsigned char buf2[2]={0};


void user_init()
{
	//1.init the LED pin,for indication
	gpio_set_func(LED1 ,AS_GPIO);
	gpio_set_output_en(LED1, 1); //enable output
	gpio_set_input_en(LED1 ,0);//disable input
	gpio_write(LED1, 0);     //LED On


#if(FLASH_MODE==FLASH_PAGE_READ)

	flash_read_page(FLASH_ADDR,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);

#elif(FLASH_MODE==FLASH_PAGE_WRITE)

	flash_write_page(FLASH_ADDR,FLASH_BUFF_LEN,(unsigned char *)Flash_Write_Buff);
	flash_read_page(FLASH_ADDR,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);

#elif(FLASH_MODE==FLASH_SECTOR_ERASE)
	flash_write_page(FLASH_ADDR,FLASH_BUFF_LEN,(unsigned char *)Flash_Write_Buff);
	flash_write_page(FLASH_ADDR+0x0f00,FLASH_BUFF_LEN,(unsigned char *)Flash_Write_Buff);
	flash_erase_sector(FLASH_ADDR);

#elif(FLASH_MODE==FLASH_DATA_PROTECTION)
	//You should release related interfaces including flash_lock and flash_unlock in "flash.c".
	flash_lock(FLASH_TYPE_GD, 0x18);	//GD25LD80CGIG_768k = 0x18,
	flash_erase_sector(FLASH_ADDR);		//the area to be software protected against Program and Erase commands.
	flash_write_page(FLASH_ADDR,FLASH_BUFF_LEN,(unsigned char *)Flash_Write_Buff);
	flash_read_page(FLASH_ADDR,FLASH_BUFF_LEN,(unsigned char *)Flash_Read_Buff);	//the protected memory area is read-only.
	flash_unlock(FLASH_TYPE_GD);

#elif(FLASH_MODE==FLASH_OTP_READ||FLASH_MODE==FLASH_OTP_WRITE)
	flash_write_otp(0x1000,256,(unsigned char *)Flash_Write_Buff);
	for(int i=0;i<300;i++)
	{
		buf1[i]=flash_read_otp_byte(0x001000+i);
	}
#elif(FLASH_MODE==FLASH_OTP_ERASE)
	flash_write_otp(0x1000,256,(unsigned char *)Flash_Write_Buff);
	flash_erase_otp_512byte(0x1005);
	for(int i=0;i<300;i++)
	{
		buf1[i]=flash_read_otp_byte(0x001000+i);
	}
#elif(FLASH_MODE==FALSH_OTP_LOCK)
	flash_write_otp(0x2000,256,(unsigned char *)Flash_Write_Buff);
	flash_lock_otp(FLASH_LOCK_OTP_0x002000);
	flash_erase_otp_512byte(0x2000);
	for(int i=0;i<300;i++)
	{
		buf1[i]=flash_read_otp_byte(0x002000+i);
	}

#endif
}


void main_loop (void)
{
	gpio_toggle(LED1);
	sleep_ms(100);
}



