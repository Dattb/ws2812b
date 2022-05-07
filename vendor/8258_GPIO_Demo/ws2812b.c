/*
 * ws2812b.c
 *
 *  Created on: Oct 22, 2021
 *      Author: PC5
 */

#include "ws2812b.h"

unsigned char LedsArray[180] = {0};

RGBColor_t      LEDTempSave;
RGBColor_t      *LEDTemp1;


u16 effect1_cnt = 0;


void effect_n ()
{
    effect1_cnt++; // hieu ung 1 counter
     if(effect1_cnt<52) // 52 vong goi ham effect_1 ()
    {
      for(int i=0;i<30;i++)  // dat gia tri cua 60 led dau tien cung 1 mau roi gui data ra
      {
        rgb_SetColor(i,255,effect1_cnt*5,0);
      }
              RGB_Show_LED(20);
    }
    // for(int j=0;j<52;j++)
    else if(effect1_cnt<104)// 52 vong goi ham effect_1 ()
    {
      for(int i=0;i<30;i++)
      {
      rgb_SetColor(i,255-5*(effect1_cnt-52),255,0);
      }
              RGB_Show_LED(20);
    }
    //    for(int j=0;j<52;j++)
    else if(effect1_cnt<156)// 52 vong goi ham effect_1 ()
    {
      for(int i=0;i<30;i++)
      {
      rgb_SetColor(i,0,255,5*(effect1_cnt-104));
      }
              RGB_Show_LED(20);
    }
    else if(effect1_cnt<208) // 52 vong goi ham effect_1 ()
    {
      for(int i=0;i<30;i++)
      {
    	  rgb_SetColor(i,0,255-5*(effect1_cnt-156),255);
      }
              RGB_Show_LED(20);
    }
    else if(effect1_cnt<260) // 52 vong goi ham effect_1 ()
    {
      for(int i=0;i<30;i++)
      {
    	  rgb_SetColor(i,5*(effect1_cnt-208),0,255);
      }
              RGB_Show_LED(20);
    }
    else if(effect1_cnt<312) // 52 vong goi ham effect_1 ()
    {
      for(int i=0;i<30;i++)
      {
    	  rgb_SetColor(i,255,0,255-5*(effect1_cnt-260));
      }
              RGB_Show_LED(20);
    } else  effect1_cnt=0;
}

u8 set_up = 0;
void effect_7 () {
	  if(!set_up){
	      for(unsigned char i=0;i<4;i++)
	      {
	        rgb_SetColor(i,255,i*83,0);
	        rgb_SetColor(i+4,255-i*83,255,0);
	        rgb_SetColor(i+8,0,255,i*83);
	        rgb_SetColor(i+12,0,255-i*83,255);
	        rgb_SetColor(i+16,i*83,0,255);
	        rgb_SetColor(i+20,255,0,255-i*83);
	      }
	      set_up=1;
	  }
	  RGB_Shift_LED_reverb(23,15); // tu du lieu settup ben tren dich tung led (4 byte) trong mang LedsArray theo chieu thuan
}

void RGB_Shift_LED_reverb (unsigned char LED_Shift,unsigned char time)
{
    LEDTemp1 = (RGBColor_t *)(LedsArray);
    LEDTempSave = *LEDTemp1;
    for(unsigned char i = 0; i < 23; i++){
      *LEDTemp1 = *(LEDTemp1+1);
      LEDTemp1++;
    }
    *LEDTemp1 = LEDTempSave;
    RGB_Show_LED (time);

}

void RGB_Shift_LED (unsigned char LED_Shift,unsigned char time)
{
	LedsArray[74] = LedsArray[71];
	LedsArray[73] = LedsArray[70];
	LedsArray[72] = LedsArray[69];
	for(u8 i =0;i<23;i++){
		LedsArray[71-i*3] = LedsArray[68-i*3];
		LedsArray[70-i*3] = LedsArray[67-i*3];
		LedsArray[69-i*3] = LedsArray[66-i*3];
	}
	LedsArray[2] = LedsArray[74] ;
	LedsArray[1] = LedsArray[73] ;
	LedsArray[0] = LedsArray[72] ;
    RGB_Show_LED (time);
}

void RGB_Show_LED (unsigned char time){
	for(u8 i = 0;i<time;i++){
		reg_gpio_out(0x201)&=~1;
		send_data_if(&LedsArray);
		reg_gpio_out(0x201)&=~1;
		sleep_us(100);
	}
}

void rgb_SetColor(unsigned char LedId, unsigned char R, unsigned char G, unsigned char B)
{
	LedsArray[LedId*3] = G;
	LedsArray[LedId*3+1] = R;
	LedsArray[LedId*3+2] = B;
}





void send_data_if(unsigned char *p){
	for(unsigned char j = 0;j<180;j++){
		unsigned char data = *p;
		if(data>>7&1){
			reg_gpio_out(0x201)|=1;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_gpio_out(0x201)&=~1;
			reg_system_tick;
			reg_system_tick;
		}
		else{
			reg_gpio_out(0x201)|=1;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_gpio_out(0x201)&=~1;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
		}
		if(data>>6&1){
			reg_gpio_out(0x201)|=1;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_gpio_out(0x201)&=~1;
			reg_system_tick;
			reg_system_tick;
		}
		else{
			reg_gpio_out(0x201)|=1;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_gpio_out(0x201)&=~1;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
		}
		if(data>>5&1){
			reg_gpio_out(0x201)|=1;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_gpio_out(0x201)&=~1;
			reg_system_tick;
			reg_system_tick;
		}
		else{
			reg_gpio_out(0x201)|=1;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_gpio_out(0x201)&=~1;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
		}
		if(data>>4&1){
			reg_gpio_out(0x201)|=1;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_gpio_out(0x201)&=~1;
			reg_system_tick;
			reg_system_tick;
		}
		else{
			reg_gpio_out(0x201)|=1;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_gpio_out(0x201)&=~1;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
		}
		if(data>>3&1){
			reg_gpio_out(0x201)|=1;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_gpio_out(0x201)&=~1;
			reg_system_tick;
			reg_system_tick;
		}
		else{
			reg_gpio_out(0x201)|=1;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_gpio_out(0x201)&=~1;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
		}
		if(data>>2&1){
			reg_gpio_out(0x201)|=1;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_gpio_out(0x201)&=~1;
			reg_system_tick;
			reg_system_tick;
		}
		else{
			reg_gpio_out(0x201)|=1;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_gpio_out(0x201)&=~1;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
		}
		if(data>>1&1){
			reg_gpio_out(0x201)|=1;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_gpio_out(0x201)&=~1;
			reg_system_tick;
			reg_system_tick;
		}
		else{
			reg_gpio_out(0x201)|=1;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_gpio_out(0x201)&=~1;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
		}
		if(data&1){
			reg_gpio_out(0x201)|=1;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_gpio_out(0x201)&=~1;
		}
		else{
			reg_gpio_out(0x201)|=1;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_gpio_out(0x201)&=~1;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
			reg_system_tick;
		}
		p++;
	}
	//reg_gpio_out(0x201)|=1;
}

