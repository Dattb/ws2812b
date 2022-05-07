/********************************************************************************************************
 * @file     app.c
 *
 * @brief    This is the EMI file for TLSR8258
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

#if(TEST_DEMO==EMI_DEMO)


#define   GPIO_SYS 0xffffffff
const GPIO_PinTypeDef gpio_map[48] = {
	GPIO_PD7,//0
	GPIO_PA0,//1
	GPIO_PA1,//2
	GPIO_PA2,//3
	GPIO_PA3,//4
	GPIO_PA4,//5
	GPIO_PA5,//6
	GPIO_PA6,//7
	GPIO_PA7,//8
	GPIO_PB0,//9
	GPIO_PB1,//10
//	GPIO_SYS,//11
//	GPIO_SYS,//12
//	GPIO_SYS,//13
//	GPIO_SYS,//14
//	GPIO_SYS,//15
//	GPIO_SYS,//16
//	GPIO_SYS,//17
//	GPIO_SYS,//18
	GPIO_PB2,//19
	GPIO_PB3,//20
	GPIO_PB4,//21
	GPIO_PB5,//22
	GPIO_PB6,//23
	GPIO_PB7,//24
//	GPIO_SYS,//25
//	GPIO_SYS,//26
	GPIO_PC0,//27
	GPIO_PC1,//28
	GPIO_PC2,//29
	GPIO_PC3,//30
	GPIO_PC4,//31
	GPIO_PC5,//32
	GPIO_PC6,//33
	GPIO_PC7,//34
//	GPIO_SYS,//35
//	GPIO_SYS,//36
//	GPIO_SYS,//37
//	GPIO_SYS,//38
//	GPIO_SYS,//39
//	GPIO_SYS,//40
	GPIO_PD0,//41
	GPIO_PD1,//42
	GPIO_PD2,//43
	GPIO_PD3,//44
	GPIO_PD4,//45
	GPIO_PD5,//46
	GPIO_PD6//47
};

#define MAX_RF_CHANNEL  40
const unsigned char rf_chn_hipping[MAX_RF_CHANNEL] = {
	48,  4, 66, 62, 44,
	12, 38, 16 ,26, 20,
	22, 68, 18, 28,	42,
	32, 34, 36, 14,	40,
	30, 54, 46, 2,	50,
	52, 80, 56, 78,	74,
	8,  64, 6,  24,	70,
	72, 60, 76, 58, 10,
};



unsigned char  mode=1;//1
unsigned char  power_level = 0;
unsigned char  chn = 2;//2
unsigned char  cmd_now=1;//1
unsigned char  run=1;
unsigned char  tx_cnt=0;
unsigned char  hop=0;






struct  test_list_s {
	unsigned char  cmd_id;
	void	 (*func)(RF_ModeTypeDef, unsigned char, signed char);
};

void emicarrieronly(RF_ModeTypeDef rf_mode,unsigned char pwr,signed char rf_chn);
void emi_con_prbs9(RF_ModeTypeDef rf_mode,unsigned char pwr,signed char rf_chn);
void emirx(RF_ModeTypeDef rf_mode,unsigned char pwr,signed char rf_chn);
void emitxprbs9(RF_ModeTypeDef rf_mode,unsigned char pwr,signed char rf_chn);
void emitx55(RF_ModeTypeDef rf_mode,unsigned char pwr,signed char rf_chn);
void emitx0f(RF_ModeTypeDef rf_mode,unsigned char pwr,signed char rf_chn);
void emi_con_tx55(RF_ModeTypeDef rf_mode,unsigned char pwr,signed char rf_chn);
void emi_con_tx0f(RF_ModeTypeDef rf_mode,unsigned char pwr,signed char rf_chn);

void emi_deepio_noren(RF_ModeTypeDef rf_mode,unsigned char pin,signed char rf_chn);
void emi_deepio_ren(RF_ModeTypeDef rf_mode,unsigned char pin,signed char rf_chn);
void emi_deeptimer_noren(RF_ModeTypeDef rf_mode,unsigned char Sec,signed char rf_chn);
void emi_deeptimer_ren(RF_ModeTypeDef rf_mode,unsigned char Sec,signed char rf_chn);
void emi_suspendio_noren(RF_ModeTypeDef rf_mode,unsigned char pin,signed char rf_chn);
void emi_suspendtimer_noren(RF_ModeTypeDef rf_mode,unsigned char Sec,signed char rf_chn);
void led_init(void);
void key_init(void);
void key_serviceloop(void);
void led_serviceloop(void);

struct  test_list_s  ate_list[] = {
		{0x01,emicarrieronly},
		{0x02,emi_con_prbs9},
		{0x03,emirx},
		{0x04,emitxprbs9},
		{0x05,emitx55},
		{0x06,emitx0f},
//		{0x07,emi_con_tx55},
//		{0x08,emi_con_tx0f},
		{0x07,emi_deepio_noren},//deep with io wakeup without retension
		{0x08,emi_deeptimer_noren},//deep with timer without retension
		{0x09,emi_suspendio_noren},//suspend with io wakeup without retension
		{0x0a,emi_suspendtimer_noren},//suspend with timer without retension
		{0x0b,emi_deepio_ren},//deep with io wakeup with retension
		{0x0c,emi_deeptimer_ren},//deep with timer with retension
};



void emi_init(void)
{
	unsigned char chnidx=0;
	write_reg32(0x408,0x29417671 );//access code  0xf8118ac9
	write_reg8(0x13c,0x10); // print buffer size set

	write_reg8(0x40005,tx_cnt);//tx_cnt
	write_reg8(0x40006,run);//run
	write_reg8(0x40007,cmd_now);//cmd
	write_reg8(0x40008,power_level);//power
	write_reg8(0x40009,chn);//chn
	write_reg8(0x4000a,mode);//mode
	write_reg8(0x4000b,hop);//hop
	write_reg8(0x40004,0);
	write_reg32(0x4000c,0);
	gpio_shutdown(GPIO_ALL);//for pm
	analog_write(0x33,0xff);

	flash_read_page(0x77000,1,&chnidx);
	if(chnidx!=0xff)
	{
		chnidx&=0x3f;
		analog_write(0x8a,(analog_read(0x8a)&0xc0)|chnidx);//close internal cap
	}
	led_init();
	key_init();
	irq_enable();
}



void emi_serviceloop(void)
{
	unsigned char i=0;
	unsigned int t0=0;
	while(1)
	{
	   run = read_reg8(0x40006);  // get the run state!
	   if(run!=0)
	   {
		   power_level = read_reg8(0x40008);
		   chn = read_reg8(0x40009);
		   mode=read_reg8(0x4000a);
		   cmd_now = read_reg8(0x40007);  // get the command!
		   tx_cnt = read_reg8(0x40005);
			for (i=0; i<sizeof (ate_list)/sizeof (struct test_list_s); i++)
			{
				if(cmd_now == ate_list[i].cmd_id)
				{
					if(mode==0)//ble 2M mode
					{
						ate_list[i].func(RF_MODE_BLE_2M,power_level,chn);
					}
					else if(mode==1)//ble 1M mode
					{
						ate_list[i].func(RF_MODE_BLE_1M_NO_PN,power_level,chn);
					}
					else if(mode==2)//zigbee mode
					{
						ate_list[i].func(RF_MODE_ZIGBEE_250K,power_level,chn);
					}
					else if(mode==3)//BLE125K mode
					{
						ate_list[i].func(RF_MODE_LR_S8_125K,power_level,chn);
					}
					else if(mode==4)//BLE500K mode
					{
						ate_list[i].func(RF_MODE_LR_S2_500K,power_level,chn);
					}
					break;
				}
			}
			run = 0;
			write_reg8(0x40006, run);
			t0 =reg_system_tick;
	   }
	   else
	   {
		   if(clock_time_exceed(t0,500))
		   {
			   key_serviceloop();
			   led_serviceloop();
			   t0 =reg_system_tick;
		   }

	   }
	}

}



void emicarrieronly(RF_ModeTypeDef rf_mode,unsigned char pwr,signed char rf_chn)
{
	RF_PowerTypeDef power = rf_power_Level_list[pwr];
	rf_emi_single_tone(power,rf_chn);
	while( ((read_reg8(0x40006)) == run ) &&  ((read_reg8(0x40007)) == cmd_now )\
			&& ((read_reg8(0x40008)) == power_level ) &&  ((read_reg8(0x40009)) == chn )\
			&& ((read_reg8(0x4000a)) == mode ));
	rf_emi_stop();
}

void emi_con_prbs9(RF_ModeTypeDef rf_mode,unsigned char pwr,signed char rf_chn)
{
	unsigned int t0 = reg_system_tick,chnidx=1;
	RF_PowerTypeDef power = rf_power_Level_list[pwr];
	hop = read_reg8(0x4000b);
	rf_emi_tx_continue_setup(rf_mode,power,rf_chn,0);
	while( ((read_reg8(0x40006)) == run ) &&  ((read_reg8(0x40007)) == cmd_now )\
			&& ((read_reg8(0x40008)) == power_level ) &&  ((read_reg8(0x40009)) == chn )\
			&& ((read_reg8(0x4000a)) == mode ))
	{
		rf_continue_mode_run();
		if(hop)
		{
			while(!clock_time_exceed(t0,1000000))//run 1ms
			rf_continue_mode_run();

			while(!clock_time_exceed(t0,2000000));//stop 10ms

			t0 = reg_system_tick;
			rf_emi_tx_continue_setup(rf_mode,power,rf_chn_hipping[chnidx-1],0);
			(chnidx>=MAX_RF_CHANNEL)?(chnidx=1):(chnidx++);
		}
//		else
//		{
//			rf_continue_mode_run();
//		}
	}
	rf_emi_stop();

	write_reg16(0x60, 0x480);
	write_reg8(0xc24, 0x0);
	write_reg8(0xc0e, 0x60);
	write_reg16(0x60, 0x0);

}

void emirx(RF_ModeTypeDef rf_mode,unsigned char pwr,signed char rf_chn)
{
	rf_emi_rx(rf_mode,rf_chn);
	write_reg8(0x40004,0);
	write_reg32(0x4000c,0);

	cmd_now = 0;//Those two sentences for dealing with the problem that click RxTest again the value of emi_rx_cnt not be cleared in emi rx test
	write_reg8(0x40007, cmd_now); //add by zhiwei,confirmed by kaixin
	while( ((read_reg8(0x40006)) == run ) &&  ((read_reg8(0x40007)) == cmd_now )\
			&& ((read_reg8(0x40008)) == power_level ) &&  ((read_reg8(0x40009)) == chn )\
			&& ((read_reg8(0x4000a)) == mode ))
	{
		rf_emi_rx_loop();
		if(rf_emi_get_rxpkt_cnt()!=read_reg32(0x4000c))
		{
		write_reg8(0x40004,rf_emi_get_rssi_avg());
		write_reg32(0x4000c,rf_emi_get_rxpkt_cnt());
		}
	}
	rf_emi_stop();
}

void emitxprbs9(RF_ModeTypeDef rf_mode,unsigned char pwr,signed char rf_chn)
{
	unsigned int tx_num=0;
	RF_PowerTypeDef power = rf_power_Level_list[pwr];
	rf_emi_tx_burst_setup(rf_mode,power,rf_chn,0);
	while( ((read_reg8(0x40006)) == run ) &&  ((read_reg8(0x40007)) == cmd_now )\
			&& ((read_reg8(0x40008)) == power_level ) &&  ((read_reg8(0x40009)) == chn )\
			&& ((read_reg8(0x4000a)) == mode  && ((read_reg8(0x40005)) == tx_cnt ) ))
	{
		rf_emi_tx_burst_loop(rf_mode,0);
		if(tx_cnt)
		{
			tx_num++;
			if(tx_num>=1000)
				break;
		}
	}
	rf_emi_stop();
}


void emitx55(RF_ModeTypeDef rf_mode,unsigned char pwr,signed char rf_chn)
{
	unsigned int tx_num=0;
	RF_PowerTypeDef power = rf_power_Level_list[pwr];
//	rf_emi_tx_burst_setup_ramp(rf_mode,pwr,rf_chn,2);
	rf_emi_tx_burst_setup(rf_mode,power,rf_chn,2);
	while( ((read_reg8(0x40006)) == run ) &&  ((read_reg8(0x40007)) == cmd_now )\
			&& ((read_reg8(0x40008)) == power_level ) &&  ((read_reg8(0x40009)) == chn )\
			&& ((read_reg8(0x4000a)) == mode && ((read_reg8(0x40005)) == tx_cnt ) ))
	{
		rf_emi_tx_burst_loop(rf_mode,2);
//		rf_emi_tx_burst_loop_ramp(rf_mode,2);
		if(tx_cnt)
		{
			tx_num++;
			if(tx_num>=1000)
				break;
		}
	}
	rf_emi_stop();
}

void emitx0f(RF_ModeTypeDef rf_mode,unsigned char pwr,signed char rf_chn)
{
	unsigned int tx_num=0;
	RF_PowerTypeDef power = rf_power_Level_list[pwr];
	rf_emi_tx_burst_setup(rf_mode,power,rf_chn,1);
	while( ((read_reg8(0x40006)) == run ) &&  ((read_reg8(0x40007)) == cmd_now )\
			&& ((read_reg8(0x40008)) == power_level ) &&  ((read_reg8(0x40009)) == chn )\
			&& ((read_reg8(0x4000a)) == mode && ((read_reg8(0x40005)) == tx_cnt ) ))
	{
		rf_emi_tx_burst_loop(rf_mode,1);
		if(tx_cnt)
		{
			tx_num++;
			if(tx_num>=1000)
				break;
		}
	}
	rf_emi_stop();
}

void emi_con_tx55(RF_ModeTypeDef rf_mode,unsigned char pwr,signed char rf_chn)
{
	RF_PowerTypeDef power = rf_power_Level_list[pwr];
	rf_emi_tx_continue_setup(rf_mode,power,rf_chn,2);
	while( ((read_reg8(0x40006)) == run ) &&  ((read_reg8(0x40007)) == cmd_now )\
			&& ((read_reg8(0x40008)) == power_level ) &&  ((read_reg8(0x40009)) == chn )\
			&& ((read_reg8(0x4000a)) == mode ))
	rf_continue_mode_run();
	rf_emi_stop();
}



void emi_con_tx0f(RF_ModeTypeDef rf_mode,unsigned char pwr,signed char rf_chn)
{
	RF_PowerTypeDef power = rf_power_Level_list[pwr];
	rf_emi_tx_continue_setup(rf_mode,power,rf_chn,1);
	while( ((read_reg8(0x40006)) == run ) &&  ((read_reg8(0x40007)) == cmd_now )\
			&& ((read_reg8(0x40008)) == power_level ) &&  ((read_reg8(0x40009)) == chn )\
			&& ((read_reg8(0x4000a)) == mode ))
	rf_continue_mode_run();
	rf_emi_stop();
}



void emi_deepio_noren(RF_ModeTypeDef rf_mode,unsigned char pin,signed char rf_chn)
{
	rf_mode = rf_mode;
	rf_chn = rf_chn;
	if(gpio_map[pin]==GPIO_SYS) return;
	cpu_set_gpio_wakeup(gpio_map[pin], Level_High, 1);  //gpio pad wakeup
	gpio_setup_up_down_resistor(gpio_map[pin], PM_PIN_PULLDOWN_100K);
	cpu_sleep_wakeup(DEEPSLEEP_MODE , PM_WAKEUP_PAD,0);
}

void emi_deeptimer_noren(RF_ModeTypeDef rf_mode,unsigned char Sec,signed char rf_chn)
{
	rf_mode = rf_mode;
	rf_chn = rf_chn;
	cpu_sleep_wakeup(DEEPSLEEP_MODE , PM_WAKEUP_TIMER,(reg_system_tick+Sec*CLOCK_16M_SYS_TIMER_CLK_1S));
}

void emi_deeptimer_ren(RF_ModeTypeDef rf_mode,unsigned char Sec,signed char rf_chn)
{
	rf_mode = rf_mode;
	rf_chn = rf_chn;
	cpu_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW16K , PM_WAKEUP_TIMER,(reg_system_tick+Sec*CLOCK_16M_SYS_TIMER_CLK_1S));
}


void emi_deepio_ren(RF_ModeTypeDef rf_mode,unsigned char pin,signed char rf_chn)
{
	rf_mode = rf_mode;
	rf_chn = rf_chn;
	if(gpio_map[pin]==GPIO_SYS) return;
	cpu_set_gpio_wakeup(gpio_map[pin], Level_High, 1);  //gpio pad wakeup
	gpio_setup_up_down_resistor(gpio_map[pin], PM_PIN_PULLDOWN_100K);
	cpu_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW16K , PM_WAKEUP_PAD,0);
}


void emi_suspendio_noren(RF_ModeTypeDef rf_mode,unsigned char pin,signed char rf_chn)
{
	rf_mode = rf_mode;
	rf_chn = rf_chn;
	if(gpio_map[pin]==GPIO_SYS) return;
	cpu_set_gpio_wakeup(gpio_map[pin], Level_High, 1);  //gpio pad wakeup
	gpio_setup_up_down_resistor(gpio_map[pin], PM_PIN_PULLDOWN_100K);
	cpu_sleep_wakeup(SUSPEND_MODE , PM_WAKEUP_PAD,0);
}

void emi_suspendtimer_noren(RF_ModeTypeDef rf_mode,unsigned char Sec,signed char rf_chn)
{
	rf_mode = rf_mode;
	rf_chn = rf_chn;
	cpu_sleep_wakeup(SUSPEND_MODE , PM_WAKEUP_TIMER,(reg_system_tick+Sec*CLOCK_16M_SYS_TIMER_CLK_1S));
}


//for key control
#define KEY1_IN		GPIO_PB2
#define KEY2_IN		GPIO_PB3
#define KEY3_OUT	GPIO_PB4
#define KEY4_OUT	GPIO_PB5
#define KeyCnt		0x20

#define LED1		GPIO_PD2
#define LED2		GPIO_PD3
#define LED3		GPIO_PD4
#define LED4		GPIO_PD5
#define LedCnt		400


const char Channel[3] = {40,80,2};//low middle high
const char Power[4] = {30,7,49,0};//0dbm,8.7dbm,minpowewr,maxpower,
const char Mode[5] = {0,2,4,3,1};//ble1m,ble2m,zigbee,s2,s8
const char Cmd[4] = {3,2,4,1};//carrier,rx,cd,tx

volatile unsigned int KeyState,KeyBuf,KeyDounce,KeyValue,KeyMask;
volatile int LedNum;
volatile int ChnIndex=0,PowIndex=0,ModeIndex=0,CmdIndex=0;

void key_init(void)
{
	//KEY INIT
	gpio_set_func(KEY3_OUT|KEY4_OUT, AS_GPIO);//as gpio  line
	gpio_set_output_en(KEY3_OUT|KEY4_OUT,1);
	gpio_set_input_en(KEY3_OUT|KEY4_OUT,0);
	gpio_write(KEY3_OUT|KEY4_OUT,1);

	gpio_set_func(KEY1_IN|KEY2_IN, AS_GPIO);//as gpio row
	gpio_set_output_en(KEY1_IN|KEY2_IN,0);
	gpio_set_input_en(KEY1_IN|KEY2_IN,1);
	gpio_setup_up_down_resistor(KEY1_IN, PM_PIN_PULLUP_10K);
	gpio_setup_up_down_resistor(KEY2_IN, PM_PIN_PULLUP_10K);

	KeyState = 0;
	KeyBuf = 0;
	KeyDounce = 0;
	KeyValue = 0;
	KeyMask = 1;
	timer0_set_mode(TIMER_MODE_SYSCLK,0,CLOCK_SYS_CLOCK_1US*500);
	timer_start(TIMER0);
}


void key_scan(void)
{
	unsigned int key;
	gpio_write(KEY3_OUT,0);
	gpio_write(KEY4_OUT,1);
	key = gpio_read(KEY1_IN|KEY2_IN);
	key <<=4;
	gpio_write(KEY3_OUT,1);
	gpio_write(KEY4_OUT,0);
	key |=gpio_read(KEY1_IN|KEY2_IN);
	key ^=0xcc;
	gpio_write(KEY3_OUT,0);

	if(key!=0&&KeyBuf!=0)
	{
		if(KeyBuf==KeyState)
		{
				KeyDounce++;
			if(KeyDounce==KeyCnt)
			{
				KeyValue = KeyState;
				write_reg8(0x40006,0);
			}
		}
		else
		{
			KeyDounce=0;
			KeyValue=0;
			KeyState = KeyBuf;
		}
	}
	else
	{
		KeyDounce=0;
		KeyValue=0;
	}

	KeyBuf = key;
}


unsigned int key_getvalue(void)
{
	unsigned int i;
	i = KeyValue;
	KeyValue = 0;
	if(KeyMask)
	return i;
	else
	return 0;
}

unsigned int key_tstvalue(void)
{
	return KeyValue;
}

void key_serviceloop(void)
{
	unsigned int i=0;
	if(key_tstvalue())
	{
		i = key_getvalue();
		if(i==0x40)
		{
			KeyMask = 0;
			write_reg8(0x40009,Channel[ChnIndex]);
			(ChnIndex>=2)?(ChnIndex=0):(ChnIndex++);
			write_reg8(0x4000b,0);//hop
			LedNum=LedCnt;
			gpio_write(LED1,1);
		}
		else if(i==0x4)
		{
			KeyMask = 0;
			write_reg8(0x4000a,Mode[ModeIndex]);
			(ModeIndex>=4)?(ModeIndex=0):(ModeIndex++);
			LedNum=LedCnt;
			gpio_write(LED2,1);
		}
		else if(i==0x80)
		{
			KeyMask = 0;
			write_reg8(0x40008,Power[PowIndex]);
			(PowIndex>=3)?(PowIndex=0):(PowIndex++);
			LedNum=LedCnt;
			gpio_write(LED3,1);
		}
		else if(i==0x8)
		{
			KeyMask = 0;
			write_reg8(0x40007,Cmd[CmdIndex]);
			(CmdIndex>=3)?(CmdIndex=0):(CmdIndex++);
			LedNum=LedCnt;
			gpio_write(LED4,1);
		}
		else
		{
			LedNum=LedCnt;
		}

	}
}


void led_init(void)
{
	gpio_set_func(LED1|LED2|LED3|LED4, AS_GPIO);//as gpio  line
	gpio_set_output_en(LED1|LED2|LED3|LED4,1);
	gpio_set_input_en(LED1|LED2|LED3|LED4,0);
	gpio_write(LED1|LED2|LED3|LED4,0);
	LedNum=-1;
}

void led_serviceloop()
{
	if(LedNum>=0)
	{
		LedNum--;
		if(LedNum==0)
		{
			KeyValue = 0;
			KeyMask = 1;
			gpio_write(LED1|LED2|LED3|LED4,1);
			write_reg8(0x40006,1);
		}
	}
}



void set_gpio(void)
{
	unsigned char i;
	for(i=0;i<32;i++)
	{
		gpio_set_func(gpio_map[i], AS_GPIO);//as gpio  line
		gpio_set_output_en(gpio_map[i],1);
		gpio_set_input_en(gpio_map[i],0);
	}
}

void set_gpio_high(void)
{
	unsigned char i;
	for(i=0;i<32;i++)
	{
		gpio_write(gpio_map[i],1);
	}
}

void set_gpio_low(void)
{
	unsigned char i;
	for(i=0;i<32;i++)
	{
	    gpio_write(gpio_map[i],0);
	}
}


void user_init(void)
{
	emi_init();
}

void main_loop(void)
{
	emi_serviceloop();
}
#endif


