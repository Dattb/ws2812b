################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../vendor/8258_GPIO_Demo/app.c \
../vendor/8258_GPIO_Demo/main.c \
../vendor/8258_GPIO_Demo/rd_code.c \
../vendor/8258_GPIO_Demo/ws2812b.c 

OBJS += \
./vendor/8258_GPIO_Demo/app.o \
./vendor/8258_GPIO_Demo/main.o \
./vendor/8258_GPIO_Demo/rd_code.o \
./vendor/8258_GPIO_Demo/ws2812b.o 


# Each subdirectory must supply rules for building sources it contributes
vendor/8258_GPIO_Demo/%.o: ../vendor/8258_GPIO_Demo/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"D:\Rang_Dong_tren_cty\CODE_Telink_normal\WS2812B" -D__PROJECT_8258_GPIO_DEMO__=1 -DCHIP_TYPE=CHIP_TYPE_8258 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


