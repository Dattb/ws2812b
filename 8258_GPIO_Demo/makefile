################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include vendor/8258_GPIO_Demo/subdir.mk
-include drivers/8258/subdir.mk
-include boot/8258/subdir.mk
-include 8258_UART_Demo/vendor/8258_UART_Demo/subdir.mk
-include 8258_UART_Demo/drivers/8258/subdir.mk
-include 8258_UART_Demo/subdir.mk
-include 8258_UART_Demo/boot/8258/subdir.mk
-include 8258_PWM_Demo/vendor/8258_PWM_Demo/subdir.mk
-include 8258_PWM_Demo/drivers/8258/subdir.mk
-include 8258_PWM_Demo/subdir.mk
-include 8258_PWM_Demo/boot/8258/subdir.mk
-include 8258_PM_Demo/vendor/8258_PM_Demo/subdir.mk
-include 8258_PM_Demo/drivers/8258/subdir.mk
-include 8258_PM_Demo/subdir.mk
-include 8258_PM_Demo/boot/8258/subdir.mk
-include 8258_I2C_Demo/vendor/8258_I2C_Demo/subdir.mk
-include 8258_I2C_Demo/drivers/8258/subdir.mk
-include 8258_I2C_Demo/subdir.mk
-include 8258_I2C_Demo/boot/8258/subdir.mk
-include objects.mk

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LST += \
8258_GPIO_Demo.lst \

FLASH_IMAGE += \
8258_GPIO_Demo.bin \

SIZEDUMMY += \
sizedummy \


# All Target
all: WS2812B.elf secondary-outputs

# Tool invocations
WS2812B.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: TC32 C Linker'
	tc32-elf-ld --gc-sections  -T ../boot.link -o"WS2812B.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

8258_GPIO_Demo.lst: WS2812B.elf
	@echo 'Invoking: TC32 Create Extended Listing'
	tc32-elf-objdump -x -D -l -S WS2812B.elf  >"8258_GPIO_Demo.lst"
	@echo 'Finished building: $@'
	@echo ' '

8258_GPIO_Demo.bin: WS2812B.elf
	@echo 'Create Flash image (binary format)'
	tc32-elf-objcopy -O binary WS2812B.elf  "8258_GPIO_Demo.bin"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: WS2812B.elf
	@echo 'Invoking: Print Size'
	tc32-elf-size -t WS2812B.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(LST)$(SIZEDUMMY) WS2812B.elf
	-@echo ' '

secondary-outputs: $(LST) $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
