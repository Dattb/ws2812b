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
#include "ws2812b.h"

extern void user_init();
extern void main_loop (void);
volatile unsigned int gpio_irq_cnt;



/**
 * @brief		This function serves to handle the interrupt of MCU
 * @param[in] 	none
 * @return 		none
 */
_attribute_ram_code_ void irq_handler(void)
{
#if (GPIO_MODE == GPIO_IRQ )

	if((reg_irq_src & FLD_IRQ_GPIO_EN)==FLD_IRQ_GPIO_EN){
		reg_irq_src |= FLD_IRQ_GPIO_EN; // clear the relevant irq
		if(gpio_read(SW1)== 0){ // press key with low level to flash light
			sleep_ms(10);
			if(gpio_read(SW1)== 0){
				gpio_irq_cnt++;
				gpio_toggle(LED1);
			}
		}
	}

#elif(GPIO_MODE == GPIO_IRQ_RSIC0)

	if((reg_irq_src & FLD_IRQ_GPIO_RISC0_EN)==FLD_IRQ_GPIO_RISC0_EN){
		reg_irq_src |= FLD_IRQ_GPIO_RISC0_EN; // clear the relevant irq

		if(gpio_read(SW2)== 0){ // press key with low level to flash light
			sleep_ms(10);
			if(gpio_read(SW2)== 0){
				gpio_irq_cnt++;
				gpio_toggle(LED1);
			}
		}
	}

#elif(GPIO_MODE == GPIO_IRQ_RSIC1)

	if((reg_irq_src & FLD_IRQ_GPIO_RISC1_EN)==FLD_IRQ_GPIO_RISC1_EN){
		reg_irq_src |= FLD_IRQ_GPIO_RISC1_EN; // clear the relevant irq

		if(gpio_read(SW2)== 0){ // press key with low level to flash light
			sleep_ms(10);
			if(gpio_read(SW2)== 0){
				gpio_irq_cnt++;
				gpio_toggle(LED1);
			}
		}
	}
#endif

}




extern u8 LedsArray[180];
u16 led_cnt = 0;
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
	gpio_set_func(GPIO_PC1 ,AS_GPIO);
	gpio_set_output_en(GPIO_PC1, 1); 		//enable output
	gpio_set_func(GPIO_PC0 ,AS_GPIO);
	gpio_set_output_en(GPIO_PC0, 1); 		//enable output

	while (1) {
//		led_cnt++;
//		if(led_cnt<200){
//			for(u8 i =0;i<30;i++)
//			rgb_SetColor(i,100,0,0);
//		}
//		else if(led_cnt<400){
//			for(u8 i =0;i<30;i++)
//			rgb_SetColor(i,0,100,0);
//		}
//		else if(led_cnt<600){
//			for(u8 i =0;i<30;i++)
//			rgb_SetColor(i,0,0,110);
//		}
//		else {
//			led_cnt =0;
//		}
//		show_led();
		effect_7 ();
		//effect_n ();

//	      for(int i=0;i<2;i++)  // dat gia tri cua 60 led dau tien cung 1 mau roi gui data ra
//    {
//     // rgb_SetColor(i,250,80,20);
//      LedsArray[0] = 80;
//      LedsArray[1] = 0xff;
//      LedsArray[2] = 0;
//      LedsArray[3] = 80;
//      LedsArray[4] = 0xff;
//      LedsArray[5] = 0;
//    }
//    RGB_Show_LED(100);
	}
	return 0;
}


