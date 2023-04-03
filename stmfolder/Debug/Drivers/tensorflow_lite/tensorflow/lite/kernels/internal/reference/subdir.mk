################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../Drivers/tensorflow_lite/tensorflow/lite/kernels/internal/reference/portable_tensor_utils.cc 

CC_DEPS += \
./Drivers/tensorflow_lite/tensorflow/lite/kernels/internal/reference/portable_tensor_utils.d 

OBJS += \
./Drivers/tensorflow_lite/tensorflow/lite/kernels/internal/reference/portable_tensor_utils.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/tensorflow_lite/tensorflow/lite/kernels/internal/reference/%.o Drivers/tensorflow_lite/tensorflow/lite/kernels/internal/reference/%.su: ../Drivers/tensorflow_lite/tensorflow/lite/kernels/internal/reference/%.cc Drivers/tensorflow_lite/tensorflow/lite/kernels/internal/reference/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-tensorflow_lite-2f-tensorflow-2f-lite-2f-kernels-2f-internal-2f-reference

clean-Drivers-2f-tensorflow_lite-2f-tensorflow-2f-lite-2f-kernels-2f-internal-2f-reference:
	-$(RM) ./Drivers/tensorflow_lite/tensorflow/lite/kernels/internal/reference/portable_tensor_utils.d ./Drivers/tensorflow_lite/tensorflow/lite/kernels/internal/reference/portable_tensor_utils.o ./Drivers/tensorflow_lite/tensorflow/lite/kernels/internal/reference/portable_tensor_utils.su

.PHONY: clean-Drivers-2f-tensorflow_lite-2f-tensorflow-2f-lite-2f-kernels-2f-internal-2f-reference

