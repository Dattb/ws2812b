################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../vendor/8258_PWM_Demo/app_count.c \
../vendor/8258_PWM_Demo/app_ir.c \
../vendor/8258_PWM_Demo/app_ir_dma_fifo.c \
../vendor/8258_PWM_Demo/app_ir_fifo.c \
../vendor/8258_PWM_Demo/app_normal.c \
../vendor/8258_PWM_Demo/main.c 

OBJS += \
./vendor/8258_PWM_Demo/app_count.o \
./vendor/8258_PWM_Demo/app_ir.o \
./vendor/8258_PWM_Demo/app_ir_dma_fifo.o \
./vendor/8258_PWM_Demo/app_ir_fifo.o \
./vendor/8258_PWM_Demo/app_normal.o \
./vendor/8258_PWM_Demo/main.o 


# Each subdirectory must supply rules for building sources it contributes
vendor/8258_PWM_Demo/%.o: ../vendor/8258_PWM_Demo/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"D:\Rang_Dong_tren_cty\CODE_Telink_normal\example_Telink" -D__PROJECT_8258_PWM_DEMO__=1 -DCHIP_TYPE=CHIP_TYPE_8258 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


