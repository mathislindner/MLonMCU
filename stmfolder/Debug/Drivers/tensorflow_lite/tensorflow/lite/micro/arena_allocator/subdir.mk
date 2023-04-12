################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../Drivers/tensorflow_lite/tensorflow/lite/micro/arena_allocator/non_persistent_arena_buffer_allocator.cc \
../Drivers/tensorflow_lite/tensorflow/lite/micro/arena_allocator/persistent_arena_buffer_allocator.cc \
../Drivers/tensorflow_lite/tensorflow/lite/micro/arena_allocator/recording_single_arena_buffer_allocator.cc \
../Drivers/tensorflow_lite/tensorflow/lite/micro/arena_allocator/single_arena_buffer_allocator.cc 

CC_DEPS += \
./Drivers/tensorflow_lite/tensorflow/lite/micro/arena_allocator/non_persistent_arena_buffer_allocator.d \
./Drivers/tensorflow_lite/tensorflow/lite/micro/arena_allocator/persistent_arena_buffer_allocator.d \
./Drivers/tensorflow_lite/tensorflow/lite/micro/arena_allocator/recording_single_arena_buffer_allocator.d \
./Drivers/tensorflow_lite/tensorflow/lite/micro/arena_allocator/single_arena_buffer_allocator.d 

OBJS += \
./Drivers/tensorflow_lite/tensorflow/lite/micro/arena_allocator/non_persistent_arena_buffer_allocator.o \
./Drivers/tensorflow_lite/tensorflow/lite/micro/arena_allocator/persistent_arena_buffer_allocator.o \
./Drivers/tensorflow_lite/tensorflow/lite/micro/arena_allocator/recording_single_arena_buffer_allocator.o \
./Drivers/tensorflow_lite/tensorflow/lite/micro/arena_allocator/single_arena_buffer_allocator.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/tensorflow_lite/tensorflow/lite/micro/arena_allocator/%.o Drivers/tensorflow_lite/tensorflow/lite/micro/arena_allocator/%.su: ../Drivers/tensorflow_lite/tensorflow/lite/micro/arena_allocator/%.cc Drivers/tensorflow_lite/tensorflow/lite/micro/arena_allocator/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-tensorflow_lite-2f-tensorflow-2f-lite-2f-micro-2f-arena_allocator

clean-Drivers-2f-tensorflow_lite-2f-tensorflow-2f-lite-2f-micro-2f-arena_allocator:
	-$(RM) ./Drivers/tensorflow_lite/tensorflow/lite/micro/arena_allocator/non_persistent_arena_buffer_allocator.d ./Drivers/tensorflow_lite/tensorflow/lite/micro/arena_allocator/non_persistent_arena_buffer_allocator.o ./Drivers/tensorflow_lite/tensorflow/lite/micro/arena_allocator/non_persistent_arena_buffer_allocator.su ./Drivers/tensorflow_lite/tensorflow/lite/micro/arena_allocator/persistent_arena_buffer_allocator.d ./Drivers/tensorflow_lite/tensorflow/lite/micro/arena_allocator/persistent_arena_buffer_allocator.o ./Drivers/tensorflow_lite/tensorflow/lite/micro/arena_allocator/persistent_arena_buffer_allocator.su ./Drivers/tensorflow_lite/tensorflow/lite/micro/arena_allocator/recording_single_arena_buffer_allocator.d ./Drivers/tensorflow_lite/tensorflow/lite/micro/arena_allocator/recording_single_arena_buffer_allocator.o ./Drivers/tensorflow_lite/tensorflow/lite/micro/arena_allocator/recording_single_arena_buffer_allocator.su ./Drivers/tensorflow_lite/tensorflow/lite/micro/arena_allocator/single_arena_buffer_allocator.d ./Drivers/tensorflow_lite/tensorflow/lite/micro/arena_allocator/single_arena_buffer_allocator.o ./Drivers/tensorflow_lite/tensorflow/lite/micro/arena_allocator/single_arena_buffer_allocator.su

.PHONY: clean-Drivers-2f-tensorflow_lite-2f-tensorflow-2f-lite-2f-micro-2f-arena_allocator

