################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include vendor/8258_UART_Demo/subdir.mk
-include drivers/8258/subdir.mk
-include boot/8258/subdir.mk
-include objects.mk

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LST += \
8258_UART_Demo.lst \

FLASH_IMAGE += \
8258_UART_Demo.bin \

SIZEDUMMY += \
sizedummy \


# All Target
all: drivers_multi_mode.elf secondary-outputs

# Tool invocations
drivers_multi_mode.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: TC32 C Linker'
	tc32-elf-ld --gc-sections  -T ../boot.link -o"drivers_multi_mode.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

8258_UART_Demo.lst: drivers_multi_mode.elf
	@echo 'Invoking: TC32 Create Extended Listing'
	tc32-elf-objdump -x -D -l -S drivers_multi_mode.elf  >"8258_UART_Demo.lst"
	@echo 'Finished building: $@'
	@echo ' '

8258_UART_Demo.bin: drivers_multi_mode.elf
	@echo 'Create Flash image (binary format)'
	tc32-elf-objcopy -O binary drivers_multi_mode.elf  "8258_UART_Demo.bin"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: drivers_multi_mode.elf
	@echo 'Invoking: Print Size'
	tc32-elf-size -t drivers_multi_mode.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(LST)$(SIZEDUMMY) drivers_multi_mode.elf
	-@echo ' '

secondary-outputs: $(LST) $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
