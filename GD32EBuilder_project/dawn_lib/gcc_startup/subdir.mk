################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# 将这些工具调用的输入和输出添加到构建变量 
S_UPPER_SRCS += \
../gcc_startup/startup_gd32f405_425.S 

OBJS += \
./gcc_startup/startup_gd32f405_425.o 

S_UPPER_DEPS += \
./gcc_startup/startup_gd32f405_425.d 


# 每个子目录必须为构建它所贡献的源提供规则
gcc_startup/%.o: ../gcc_startup/%.S gcc_startup/subdir.mk
	@echo '正在构建文件： $<'
	@echo '正在调用： GD ARM MCU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -Wa,-adhlns="$@.lst"   -c -o "$@" "$<"
	@echo '已结束构建： $<'
	@echo ' '


clean: clean-gcc_startup

clean-gcc_startup:
	-$(RM) ./gcc_startup/startup_gd32f405_425.d ./gcc_startup/startup_gd32f405_425.o

.PHONY: clean-gcc_startup

