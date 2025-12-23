################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# 将这些工具调用的输入和输出添加到构建变量 
C_SRCS += \
C:/Users/leonardo/Desktop/Code/Github/DawnBGC/Dawn/DawnMain/dawn_function.c \
C:/Users/leonardo/Desktop/Code/Github/DawnBGC/Dawn/DawnMain/dawn_main.c \
C:/Users/leonardo/Desktop/Code/Github/DawnBGC/Dawn/DawnMain/dawn_parameter.c \
C:/Users/leonardo/Desktop/Code/Github/DawnBGC/Dawn/DawnMain/gd32f4xx_it.c 

C_DEPS += \
./DawnMain/dawn_function.d \
./DawnMain/dawn_main.d \
./DawnMain/dawn_parameter.d \
./DawnMain/gd32f4xx_it.d 

OBJS += \
./DawnMain/dawn_function.o \
./DawnMain/dawn_main.o \
./DawnMain/dawn_parameter.o \
./DawnMain/gd32f4xx_it.o 


# 每个子目录必须为构建它所贡献的源提供规则
DawnMain/dawn_function.o: C:/Users/leonardo/Desktop/Code/Github/DawnBGC/Dawn/DawnMain/dawn_function.c DawnMain/subdir.mk
	@echo '正在构建文件： $<'
	@echo '正在调用： GD ARM MCU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -std=gnu99 -DGD32F405 -DARM_MATH_CM4 -DGD_ECLIPSE_GCC -DUSE_STDPERIPH_DRIVER -I../../CMSIS/DSP/Include -I../../CMSIS/Core/Include -I"../../CMSIS" -I../../Dawn/DawnLib -I"../../Dawn/DawnSys" -I"../../Dawn/DawnMain" -I"../../Dawn/DawnBsp" -I"../../Dawn/DawnCom" -I"../../Dawn/DawnDevice" -I"../../Dawn/DawnGimbal" -I"../../Dawn/DawnBLDC" -I../../Libraries/FWLIB/inc -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -Wa,-adhlns="$@.lst"   -c -o "$@" "$<"
	@echo '已结束构建： $<'
	@echo ' '

DawnMain/dawn_main.o: C:/Users/leonardo/Desktop/Code/Github/DawnBGC/Dawn/DawnMain/dawn_main.c DawnMain/subdir.mk
	@echo '正在构建文件： $<'
	@echo '正在调用： GD ARM MCU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -std=gnu99 -DGD32F405 -DARM_MATH_CM4 -DGD_ECLIPSE_GCC -DUSE_STDPERIPH_DRIVER -I../../CMSIS/DSP/Include -I../../CMSIS/Core/Include -I"../../CMSIS" -I../../Dawn/DawnLib -I"../../Dawn/DawnSys" -I"../../Dawn/DawnMain" -I"../../Dawn/DawnBsp" -I"../../Dawn/DawnCom" -I"../../Dawn/DawnDevice" -I"../../Dawn/DawnGimbal" -I"../../Dawn/DawnBLDC" -I../../Libraries/FWLIB/inc -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -Wa,-adhlns="$@.lst"   -c -o "$@" "$<"
	@echo '已结束构建： $<'
	@echo ' '

DawnMain/dawn_parameter.o: C:/Users/leonardo/Desktop/Code/Github/DawnBGC/Dawn/DawnMain/dawn_parameter.c DawnMain/subdir.mk
	@echo '正在构建文件： $<'
	@echo '正在调用： GD ARM MCU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -std=gnu99 -DGD32F405 -DARM_MATH_CM4 -DGD_ECLIPSE_GCC -DUSE_STDPERIPH_DRIVER -I../../CMSIS/DSP/Include -I../../CMSIS/Core/Include -I"../../CMSIS" -I../../Dawn/DawnLib -I"../../Dawn/DawnSys" -I"../../Dawn/DawnMain" -I"../../Dawn/DawnBsp" -I"../../Dawn/DawnCom" -I"../../Dawn/DawnDevice" -I"../../Dawn/DawnGimbal" -I"../../Dawn/DawnBLDC" -I../../Libraries/FWLIB/inc -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -Wa,-adhlns="$@.lst"   -c -o "$@" "$<"
	@echo '已结束构建： $<'
	@echo ' '

DawnMain/gd32f4xx_it.o: C:/Users/leonardo/Desktop/Code/Github/DawnBGC/Dawn/DawnMain/gd32f4xx_it.c DawnMain/subdir.mk
	@echo '正在构建文件： $<'
	@echo '正在调用： GD ARM MCU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -std=gnu99 -DGD32F405 -DARM_MATH_CM4 -DGD_ECLIPSE_GCC -DUSE_STDPERIPH_DRIVER -I../../CMSIS/DSP/Include -I../../CMSIS/Core/Include -I"../../CMSIS" -I../../Dawn/DawnLib -I"../../Dawn/DawnSys" -I"../../Dawn/DawnMain" -I"../../Dawn/DawnBsp" -I"../../Dawn/DawnCom" -I"../../Dawn/DawnDevice" -I"../../Dawn/DawnGimbal" -I"../../Dawn/DawnBLDC" -I../../Libraries/FWLIB/inc -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -Wa,-adhlns="$@.lst"   -c -o "$@" "$<"
	@echo '已结束构建： $<'
	@echo ' '


clean: clean-DawnMain

clean-DawnMain:
	-$(RM) ./DawnMain/dawn_function.d ./DawnMain/dawn_function.o ./DawnMain/dawn_main.d ./DawnMain/dawn_main.o ./DawnMain/dawn_parameter.d ./DawnMain/dawn_parameter.o ./DawnMain/gd32f4xx_it.d ./DawnMain/gd32f4xx_it.o

.PHONY: clean-DawnMain

