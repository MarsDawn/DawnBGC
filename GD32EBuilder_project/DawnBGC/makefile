################################################################################
# 自动生成的文件。不要编辑！
################################################################################

-include ../makefile.init

RM := rm -rf

# 此处定义了参与生成的所有源
-include sources.mk
-include gcc_startup/subdir.mk
-include dawn_lib/DawnLib/subdir.mk
-include FWLIB/subdir.mk
-include DawnSys/subdir.mk
-include DawnMain/subdir.mk
-include DawnGimbal/subdir.mk
-include DawnDevice/subdir.mk
-include DawnCom/subdir.mk
-include DawnBsp/subdir.mk
-include DawnBLDC/subdir.mk
-include CMSIS/subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := DawnBGC
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# 将这些工具调用的输入和输出添加到构建变量 
SECONDARY_FLASH_HEX += \
DawnBGC.hex \

SECONDARY_FLASH_BIN += \
DawnBGC.bin \

SECONDARY_LIST += \
DawnBGC.lst \

SECONDARY_SIZE += \
DawnBGC.siz \


# 所有目标
all: main-build

# 主构建目标
main-build: DawnBGC.elf secondary-outputs

# 工具调用
DawnBGC.elf: $(OBJS) $(USER_OBJS) C:\Users\leonardo\Desktop\Code\Github\DawnGBC\GD32EBuilder_project\ldscripts\gd32f4xx_flash.ld makefile $(OPTIONAL_TOOL_DEPS)
	@echo '正在构建目标： $@'
	@echo '正在调用： GD ARM MCU Linker'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3  -T gd32f4xx_flash.ld -Xlinker --gc-sections -L"../ldscripts" -L"C:/Users/leonardo/Desktop/Code/Github/DawnGBC/GD32EBuilder_project\dawn_lib" -Wl,-Map,"DawnBGC.map" --specs=nano.specs --specs=nosys.specs -o "DawnBGC.elf" @"objects.list" $(USER_OBJS) $(LIBS) -lDawnBGC
	@echo '已结束构建目标： $@'
	@echo ' '

DawnBGC.hex: DawnBGC.elf makefile $(OPTIONAL_TOOL_DEPS)
	@echo '正在调用： GD ARM MCU Flash Image(Hex)'
	arm-none-eabi-objcopy -O ihex  "DawnBGC.elf"  "DawnBGC.hex"
	@echo '已结束构建： $@'
	@echo ' '

DawnBGC.bin: DawnBGC.elf makefile $(OPTIONAL_TOOL_DEPS)
	@echo '正在调用： GD ARM MCU Flash Image(Bin)'
	arm-none-eabi-objcopy -O binary  "DawnBGC.elf"  "DawnBGC.bin"
	@echo '已结束构建： $@'
	@echo ' '

DawnBGC.lst: DawnBGC.elf makefile $(OPTIONAL_TOOL_DEPS)
	@echo '正在调用： GD ARM MCU Listing'
	arm-none-eabi-objdump --source --all-headers --demangle --line-numbers --wide "DawnBGC.elf" > "DawnBGC.lst"
	@echo '已结束构建： $@'
	@echo ' '

DawnBGC.siz: DawnBGC.elf makefile $(OPTIONAL_TOOL_DEPS)
	@echo '正在调用： GD ARM MCU Print Size'
	arm-none-eabi-size --format=berkeley "DawnBGC.elf"
	@echo '已结束构建： $@'
	@echo ' '

# 其他目标
clean:
	-$(RM) DawnBGC.bin DawnBGC.elf DawnBGC.hex DawnBGC.lst DawnBGC.siz
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH_HEX) $(SECONDARY_FLASH_BIN) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents main-build

-include ../makefile.targets
