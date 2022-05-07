## V2.1.0
### Features

* **pm:** 新增pm长睡眠接口
* **pm:** update pm long sleep wakeup
* **uart:** 新增GPIO_PC2作为uart tx
* **uart:** add GPIO_PC2 as the tx of uart
* **gpio:** 修改gpio_set_func function接口
* **gpio:** modify gpio_set_func function
  - 使能GPIO中断前先清除中断避免中断误触发
  - clear gpio interrupt source before enable to avoid a unexpected interrupt 
* **flash:** 修改flash读mid和uid接口适用所以falsh类型
* **flash:** modify flash_read_mid_uid_with_check function to supplement all Flash Type
  - 增加flash-otp接口  
  - add flash-otp interface
* **audio:** 新增双amic功能
* **audio:** add double amic driver in audio.c and audio.h
  - 更新audio的i2s模块
  - update audio of i2s module
* **usb:** 增加usb的断电中断
* **usb:** add usb power down interrupt
* **adc:** 增加api设置pre_saaling和参考电压
* **adc:** add api to set pre_saaling and reference voltage

### BREAKING CHANGES
* N/A

## V2.0.1

### Bug Fixs
* [Audio] Dmic
* [UART ] After wakeup from power-saving mode, you must set the 'uart_TxIndex' to 0 before sending the data for no dma mode.(if have no dma receive,also need to reset index to 0 )
* [SPI ] Modify the clk problem of spi_read	
			
### Features
* [RF]
   + Add rf_multi_mode_drv_init/rf_pn_disable
   + BQB: power:7dbm preamble:6 read flash cap
   + EMI: modify interface
* [ADC]   Default adc_calib_en

## V2.0.0

### Features
* Modified to the API interface of the SDK
* Add reg_rf_rx_timeout = SRX_WAITTIME_US; 

