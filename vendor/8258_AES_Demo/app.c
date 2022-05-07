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

#define LED1     		        GPIO_PD0


#if(AES_MODE == RISC_MODE)

unsigned char sPlainText[16] 	= {0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};
unsigned char sKey[16] 			= {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
unsigned char EncryptResult[16] = {};
unsigned char sCipherText[16] 	= {};
unsigned char DecryptResult[16] = {};

#elif(AES_MODE == DMA_MODE)

unsigned int sPlainText[64]={0x00000040,
							 0x33221100,0x77665544,0xbbaa9988,0xffeeddcc,
							 0x33221100,0x77665544,0xbbaa9988,0xffeeddcc,
							 0x33221100,0x77665544,0xbbaa9988,0xffeeddcc,
							 0x33221100,0x77665544,0xbbaa9988,0xffeeddcc};
unsigned char sKey[16]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
unsigned int EncryptResult[64];
unsigned int sCipherText[64];
unsigned int DecryptResult[64];

#elif(AES_MODE == CBC_MODE)

unsigned char EncryptResult1[16] = {};
unsigned char EncryptResult2[16] = {};
unsigned char DecryptResult1[16] = {};
unsigned char DecryptResult2[16] = {};
unsigned char init[16]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
unsigned char sPlainText1[16]={0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a};
unsigned char sPlainText2[16]={0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51};
unsigned char sKey[16]={0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
unsigned char counter1[16]={0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff};
unsigned char counter2[16]={0xf0,0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xff,0x00};

#endif
void user_init()
{
	sleep_ms(2000);

#if(AES_MODE == RISC_MODE)


	//step1: encrypt the plain text
	aes_encrypt(sKey, sPlainText, EncryptResult);
	//step2: save the result being encrypted to sCipherText[]
	for(unsigned char i=0;i<16;i++)
	{
		sCipherText[i] = EncryptResult[i];
	}
	//step3: decrypt the result being encrypted and save result to DecryptResult[]
	aes_decrypt(sKey, sCipherText, DecryptResult);

#elif(AES_MODE == DMA_MODE)
	//step1: encrypt the plain text
	aes_dma_encrypt(sKey,sPlainText,sizeof(sPlainText),EncryptResult,sizeof(EncryptResult) );
	//step2: save the result being encrypted to cipher_buf[]
	for(int i=0;i<64;i++)
	{
		sCipherText[i] = EncryptResult[i];
	}
	//step3: decrypt the result being encrypted and save result to decrypt_result[]
	aes_dma_decrypt(sKey,sCipherText,sizeof(sCipherText),DecryptResult,sizeof(DecryptResult) );

#elif(AES_MODE == CBC_MODE)

	for(int i = 0;i<16;i++)
	{
		sPlainText1[i] = sPlainText1[i]^init[i];
	}
	aes_encrypt(sKey, sPlainText1, EncryptResult1);
	for(int i = 0;i<16;i++)
	{
		sPlainText2[i] = sPlainText2[i]^EncryptResult1[i];
	}
	aes_encrypt(sKey, sPlainText2, EncryptResult2);


	aes_decrypt(sKey, EncryptResult1, DecryptResult1);
	for(int i = 0;i<16;i++)
	{
		DecryptResult1[i] = DecryptResult1[i]^init[i];
	}


	aes_decrypt(sKey, EncryptResult2, DecryptResult2);
	for(int i = 0;i<16;i++)
	{
		DecryptResult2[i] = DecryptResult2[i]^EncryptResult1[i];
	}


#endif

}

void main_loop (void)
{
	gpio_toggle(LED1);
	sleep_ms(100);
}




