################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../vendor/8258_PM_Demo/app.c \
../vendor/8258_PM_Demo/main.c 

OBJS += \
./vendor/8258_PM_Demo/app.o \
./vendor/8258_PM_Demo/main.o 


# Each subdirectory must supply rules for building sources it contributes
vendor/8258_PM_Demo/%.o: ../vendor/8258_PM_Demo/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"D:\Rang_Dong_tren_cty\CODE_Telink_normal\example_Telink" -D__PROJECT_8258_PM_DEMO__=1 -DCHIP_TYPE=CHIP_TYPE_8258 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


