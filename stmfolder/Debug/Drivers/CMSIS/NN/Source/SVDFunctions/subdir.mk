################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/NN/Source/SVDFunctions/arm_svdf_s8.c \
../Drivers/CMSIS/NN/Source/SVDFunctions/arm_svdf_state_s16_s8.c 

C_DEPS += \
./Drivers/CMSIS/NN/Source/SVDFunctions/arm_svdf_s8.d \
./Drivers/CMSIS/NN/Source/SVDFunctions/arm_svdf_state_s16_s8.d 

OBJS += \
./Drivers/CMSIS/NN/Source/SVDFunctions/arm_svdf_s8.o \
./Drivers/CMSIS/NN/Source/SVDFunctions/arm_svdf_state_s16_s8.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/NN/Source/SVDFunctions/%.o Drivers/CMSIS/NN/Source/SVDFunctions/%.su: ../Drivers/CMSIS/NN/Source/SVDFunctions/%.c Drivers/CMSIS/NN/Source/SVDFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-CMSIS-2f-NN-2f-Source-2f-SVDFunctions

clean-Drivers-2f-CMSIS-2f-NN-2f-Source-2f-SVDFunctions:
	-$(RM) ./Drivers/CMSIS/NN/Source/SVDFunctions/arm_svdf_s8.d ./Drivers/CMSIS/NN/Source/SVDFunctions/arm_svdf_s8.o ./Drivers/CMSIS/NN/Source/SVDFunctions/arm_svdf_s8.su ./Drivers/CMSIS/NN/Source/SVDFunctions/arm_svdf_state_s16_s8.d ./Drivers/CMSIS/NN/Source/SVDFunctions/arm_svdf_state_s16_s8.o ./Drivers/CMSIS/NN/Source/SVDFunctions/arm_svdf_state_s16_s8.su

.PHONY: clean-Drivers-2f-CMSIS-2f-NN-2f-Source-2f-SVDFunctions

