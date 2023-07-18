################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_nn_softmax_common_s8.c \
../Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_q15.c \
../Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_q7.c \
../Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_s16.c \
../Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_s8.c \
../Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_s8_s16.c \
../Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_u8.c \
../Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_with_batch_q7.c 

C_DEPS += \
./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_nn_softmax_common_s8.d \
./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_q15.d \
./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_q7.d \
./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_s16.d \
./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_s8.d \
./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_s8_s16.d \
./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_u8.d \
./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_with_batch_q7.d 

OBJS += \
./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_nn_softmax_common_s8.o \
./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_q15.o \
./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_q7.o \
./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_s16.o \
./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_s8.o \
./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_s8_s16.o \
./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_u8.o \
./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_with_batch_q7.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/NN/Source/SoftmaxFunctions/%.o Drivers/CMSIS/NN/Source/SoftmaxFunctions/%.su: ../Drivers/CMSIS/NN/Source/SoftmaxFunctions/%.c Drivers/CMSIS/NN/Source/SoftmaxFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-CMSIS-2f-NN-2f-Source-2f-SoftmaxFunctions

clean-Drivers-2f-CMSIS-2f-NN-2f-Source-2f-SoftmaxFunctions:
	-$(RM) ./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_nn_softmax_common_s8.d ./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_nn_softmax_common_s8.o ./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_nn_softmax_common_s8.su ./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_q15.d ./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_q15.o ./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_q15.su ./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_q7.d ./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_q7.o ./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_q7.su ./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_s16.d ./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_s16.o ./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_s16.su ./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_s8.d ./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_s8.o ./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_s8.su ./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_s8_s16.d ./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_s8_s16.o ./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_s8_s16.su ./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_u8.d ./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_u8.o ./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_u8.su ./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_with_batch_q7.d ./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_with_batch_q7.o ./Drivers/CMSIS/NN/Source/SoftmaxFunctions/arm_softmax_with_batch_q7.su

.PHONY: clean-Drivers-2f-CMSIS-2f-NN-2f-Source-2f-SoftmaxFunctions
