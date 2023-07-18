################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/add.cc \
../Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/conv.cc \
../Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/depthwise_conv.cc \
../Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/fully_connected.cc \
../Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/mul.cc \
../Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/pooling.cc \
../Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/softmax.cc \
../Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/svdf.cc 

CC_DEPS += \
./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/add.d \
./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/conv.d \
./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/depthwise_conv.d \
./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/fully_connected.d \
./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/mul.d \
./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/pooling.d \
./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/softmax.d \
./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/svdf.d 

OBJS += \
./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/add.o \
./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/conv.o \
./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/depthwise_conv.o \
./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/fully_connected.o \
./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/mul.o \
./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/pooling.o \
./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/softmax.o \
./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/svdf.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/%.o Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/%.su: ../Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/%.cc Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-tensorflow_lite-2f-tensorflow-2f-lite-2f-micro-2f-kernels-2f-cmsis_nn

clean-Drivers-2f-tensorflow_lite-2f-tensorflow-2f-lite-2f-micro-2f-kernels-2f-cmsis_nn:
	-$(RM) ./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/add.d ./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/add.o ./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/add.su ./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/conv.d ./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/conv.o ./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/conv.su ./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/depthwise_conv.d ./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/depthwise_conv.o ./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/depthwise_conv.su ./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/fully_connected.d ./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/fully_connected.o ./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/fully_connected.su ./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/mul.d ./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/mul.o ./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/mul.su ./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/pooling.d ./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/pooling.o ./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/pooling.su ./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/softmax.d ./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/softmax.o ./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/softmax.su ./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/svdf.d ./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/svdf.o ./Drivers/tensorflow_lite/tensorflow/lite/micro/kernels/cmsis_nn/svdf.su

.PHONY: clean-Drivers-2f-tensorflow_lite-2f-tensorflow-2f-lite-2f-micro-2f-kernels-2f-cmsis_nn

