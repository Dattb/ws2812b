/********************************************************************************************************
 * @file     app_config.h
 *
 * @brief    This is the application configuration file for the TLSR8258 Audio.
 *
 * @author   Driver Group
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

#pragma once
#include "drivers.h"
/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif


#define AUDIO_AMIC_TO_SDM					1
#define AUDIO_DMIC_TO_SDM					2
#define AUDIO_AMIC_TO_CODEC					3
#define AUDIO_BUF_TO_SDM					4
#define AUDIO_AMIC_TO_BUF					5
#define AUDIO_AMIC_TO_BUF_TO_SDM			6			/* MUST enable AUDIO_DBL_BUF_ENABLE in audio.h */
#define AUDIO_AMIC_TO_SDM_BIDIR_SIGFIFO		7
#define AUDIO_AMIC_TO_SDM_BIDIR_BIDFIFO		8			/* MUST enable AUDIO_DBL_BUF_ENABLE in audio.h */
#define AUDIO_CODEC_TO_CODEC				9
#define AUDIO_BUF_TO_SDM_BY_TIMER_IRQ		10

#define AUDIO_MODE							AUDIO_BUF_TO_SDM_BY_TIMER_IRQ





/////////////////// Clock  /////////////////////////////////
#define CLOCK_SYS_CLOCK_HZ  	24000000

#if(CLOCK_SYS_CLOCK_HZ==12000000)
	#define SYS_CLK  	SYS_CLK_12M_Crystal
#elif (CLOCK_SYS_CLOCK_HZ==16000000)
	#define SYS_CLK  	SYS_CLK_16M_Crystal
#elif (CLOCK_SYS_CLOCK_HZ==24000000)
	#define SYS_CLK  	SYS_CLK_24M_Crystal
#elif (CLOCK_SYS_CLOCK_HZ==32000000)
	#define SYS_CLK  	SYS_CLK_32M_Crystal
#elif (CLOCK_SYS_CLOCK_HZ==48000000)
	#define SYS_CLK  	SYS_CLK_48M_Crystal
#endif

enum{
	CLOCK_SYS_CLOCK_1S = CLOCK_SYS_CLOCK_HZ,
	CLOCK_SYS_CLOCK_1MS = (CLOCK_SYS_CLOCK_1S / 1000),
	CLOCK_SYS_CLOCK_1US = (CLOCK_SYS_CLOCK_1S / 1000000),
};












/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
