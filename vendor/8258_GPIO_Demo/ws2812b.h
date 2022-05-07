/*
 * ws2812b.h
 *
 *  Created on: Oct 22, 2021
 *      Author: PC5
 */

#ifndef WS2812B_H_
#define WS2812B_H_


typedef struct
{
  unsigned char R;
  unsigned char G;
  unsigned char B;
}RGBColor_t;

#endif /* WS2812B_H_ */
#include "app_config.h"
void effect_n ();
void effect_1 ();
void effect_7 ();
void RGB_Show_LED (unsigned char time);
void RGB_Shift_LED (unsigned char LED_Shift,unsigned char time);
void RGB_Shift_LED_reverb (unsigned char LED_Shift,unsigned char time);
void rgb_SetColor(unsigned char LedId, unsigned char R, unsigned char G, unsigned char B);
void send_data_if(unsigned char *p);
