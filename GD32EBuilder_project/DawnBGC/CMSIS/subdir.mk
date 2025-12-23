################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# 将这些工具调用的输入和输出添加到构建变量 
C_SRCS += \
C:/Users/leonardo/Desktop/Code/Github/DawnBGC/CMSIS/system_gd32f4xx.c 

C_DEPS += \
./CMSIS/system_gd32f4xx.d 

OBJS += \
./CMSIS/system_gd32f4xx.o 


# 每个子目录必须为构建它所贡献的源提供规则
CMSIS/system_gd32f4xx.o: C:/Users/leonardo/Desktop/Code/Github/DawnBGC/CMSIS/system_gd32f4xx.c CMSIS/subdir.mk
	@echo '正在构建文件： $<'
	@echo '正在调用： GD ARM MCU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -std=gnu99 -DGD32F405 -DARM_MATH_CM4 -DGD_ECLIPSE_GCC -DUSE_STDPERIPH_DRIVER -I../../CMSIS/DSP/Include -I../../CMSIS/Core/Include -I"../../CMSIS" -I../../Dawn/DawnLib -I"../../Dawn/DawnSys" -I"../../Dawn/DawnMain" -I"../../Dawn/DawnBsp" -I"../../Dawn/DawnCom" -I"../../Dawn/DawnDevice" -I"../../Dawn/DawnGimbal" -I"../../Dawn/DawnBLDC" -I../../Libraries/FWLIB/inc -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -Wa,-adhlns="$@.lst"   -c -o "$@" "$<"
	@echo '已结束构建： $<'
	@echo ' '


clean: clean-CMSIS

clean-CMSIS:
	-$(RM) ./CMSIS/system_gd32f4xx.d ./CMSIS/system_gd32f4xx.o

.PHONY: clean-CMSIS

