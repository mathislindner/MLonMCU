################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../Drivers/tensorflow_lite/tensorflow/lite/micro/all_ops_resolver.cc \
../Drivers/tensorflow_lite/tensorflow/lite/micro/fake_micro_context.cc \
../Drivers/tensorflow_lite/tensorflow/lite/micro/flatbuffer_utils.cc \
../Drivers/tensorflow_lite/tensorflow/lite/micro/memory_helpers.cc \
../Drivers/tensorflow_lite/tensorflow/lite/micro/micro_allocation_info.cc \
../Drivers/tensorflow_lite/tensorflow/lite/micro/micro_allocator.cc \
../Drivers/tensorflow_lite/tensorflow/lite/micro/micro_context.cc \
../Drivers/tensorflow_lite/tensorflow/lite/micro/micro_error_reporter.cc \
../Drivers/tensorflow_lite/tensorflow/lite/micro/micro_graph.cc \
../Drivers/tensorflow_lite/tensorflow/lite/micro/micro_interpreter.cc \
../Drivers/tensorflow_lite/tensorflow/lite/micro/micro_profiler.cc \
../Drivers/tensorflow_lite/tensorflow/lite/micro/micro_resource_variable.cc \
../Drivers/tensorflow_lite/tensorflow/lite/micro/micro_string.cc \
../Drivers/tensorflow_lite/tensorflow/lite/micro/micro_time.cc \
../Drivers/tensorflow_lite/tensorflow/lite/micro/micro_utils.cc \
../Drivers/tensorflow_lite/tensorflow/lite/micro/mock_micro_graph.cc \
../Drivers/tensorflow_lite/tensorflow/lite/micro/recording_micro_allocator.cc \
../Drivers/tensorflow_lite/tensorflow/lite/micro/system_setup.cc \
../Drivers/tensorflow_lite/tensorflow/lite/micro/test_helper_custom_ops.cc \
../Drivers/tensorflow_lite/tensorflow/lite/micro/test_helpers.cc 

CC_DEPS += \
./Drivers/tensorflow_lite/tensorflow/lite/micro/all_ops_resolver.d \
./Drivers/tensorflow_lite/tensorflow/lite/micro/fake_micro_context.d \
./Drivers/tensorflow_lite/tensorflow/lite/micro/flatbuffer_utils.d \
./Drivers/tensorflow_lite/tensorflow/lite/micro/memory_helpers.d \
./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_allocation_info.d \
./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_allocator.d \
./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_context.d \
./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_error_reporter.d \
./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_graph.d \
./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_interpreter.d \
./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_profiler.d \
./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_resource_variable.d \
./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_string.d \
./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_time.d \
./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_utils.d \
./Drivers/tensorflow_lite/tensorflow/lite/micro/mock_micro_graph.d \
./Drivers/tensorflow_lite/tensorflow/lite/micro/recording_micro_allocator.d \
./Drivers/tensorflow_lite/tensorflow/lite/micro/system_setup.d \
./Drivers/tensorflow_lite/tensorflow/lite/micro/test_helper_custom_ops.d \
./Drivers/tensorflow_lite/tensorflow/lite/micro/test_helpers.d 

OBJS += \
./Drivers/tensorflow_lite/tensorflow/lite/micro/all_ops_resolver.o \
./Drivers/tensorflow_lite/tensorflow/lite/micro/fake_micro_context.o \
./Drivers/tensorflow_lite/tensorflow/lite/micro/flatbuffer_utils.o \
./Drivers/tensorflow_lite/tensorflow/lite/micro/memory_helpers.o \
./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_allocation_info.o \
./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_allocator.o \
./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_context.o \
./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_error_reporter.o \
./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_graph.o \
./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_interpreter.o \
./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_profiler.o \
./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_resource_variable.o \
./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_string.o \
./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_time.o \
./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_utils.o \
./Drivers/tensorflow_lite/tensorflow/lite/micro/mock_micro_graph.o \
./Drivers/tensorflow_lite/tensorflow/lite/micro/recording_micro_allocator.o \
./Drivers/tensorflow_lite/tensorflow/lite/micro/system_setup.o \
./Drivers/tensorflow_lite/tensorflow/lite/micro/test_helper_custom_ops.o \
./Drivers/tensorflow_lite/tensorflow/lite/micro/test_helpers.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/tensorflow_lite/tensorflow/lite/micro/%.o Drivers/tensorflow_lite/tensorflow/lite/micro/%.su: ../Drivers/tensorflow_lite/tensorflow/lite/micro/%.cc Drivers/tensorflow_lite/tensorflow/lite/micro/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-tensorflow_lite-2f-tensorflow-2f-lite-2f-micro

clean-Drivers-2f-tensorflow_lite-2f-tensorflow-2f-lite-2f-micro:
	-$(RM) ./Drivers/tensorflow_lite/tensorflow/lite/micro/all_ops_resolver.d ./Drivers/tensorflow_lite/tensorflow/lite/micro/all_ops_resolver.o ./Drivers/tensorflow_lite/tensorflow/lite/micro/all_ops_resolver.su ./Drivers/tensorflow_lite/tensorflow/lite/micro/fake_micro_context.d ./Drivers/tensorflow_lite/tensorflow/lite/micro/fake_micro_context.o ./Drivers/tensorflow_lite/tensorflow/lite/micro/fake_micro_context.su ./Drivers/tensorflow_lite/tensorflow/lite/micro/flatbuffer_utils.d ./Drivers/tensorflow_lite/tensorflow/lite/micro/flatbuffer_utils.o ./Drivers/tensorflow_lite/tensorflow/lite/micro/flatbuffer_utils.su ./Drivers/tensorflow_lite/tensorflow/lite/micro/memory_helpers.d ./Drivers/tensorflow_lite/tensorflow/lite/micro/memory_helpers.o ./Drivers/tensorflow_lite/tensorflow/lite/micro/memory_helpers.su ./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_allocation_info.d ./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_allocation_info.o ./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_allocation_info.su ./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_allocator.d ./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_allocator.o ./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_allocator.su ./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_context.d ./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_context.o ./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_context.su ./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_error_reporter.d ./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_error_reporter.o ./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_error_reporter.su ./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_graph.d ./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_graph.o ./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_graph.su ./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_interpreter.d ./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_interpreter.o ./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_interpreter.su ./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_profiler.d ./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_profiler.o ./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_profiler.su ./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_resource_variable.d ./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_resource_variable.o ./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_resource_variable.su ./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_string.d ./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_string.o ./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_string.su ./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_time.d ./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_time.o ./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_time.su ./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_utils.d ./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_utils.o ./Drivers/tensorflow_lite/tensorflow/lite/micro/micro_utils.su ./Drivers/tensorflow_lite/tensorflow/lite/micro/mock_micro_graph.d ./Drivers/tensorflow_lite/tensorflow/lite/micro/mock_micro_graph.o ./Drivers/tensorflow_lite/tensorflow/lite/micro/mock_micro_graph.su ./Drivers/tensorflow_lite/tensorflow/lite/micro/recording_micro_allocator.d ./Drivers/tensorflow_lite/tensorflow/lite/micro/recording_micro_allocator.o ./Drivers/tensorflow_lite/tensorflow/lite/micro/recording_micro_allocator.su ./Drivers/tensorflow_lite/tensorflow/lite/micro/system_setup.d ./Drivers/tensorflow_lite/tensorflow/lite/micro/system_setup.o ./Drivers/tensorflow_lite/tensorflow/lite/micro/system_setup.su ./Drivers/tensorflow_lite/tensorflow/lite/micro/test_helper_custom_ops.d ./Drivers/tensorflow_lite/tensorflow/lite/micro/test_helper_custom_ops.o ./Drivers/tensorflow_lite/tensorflow/lite/micro/test_helper_custom_ops.su ./Drivers/tensorflow_lite/tensorflow/lite/micro/test_helpers.d ./Drivers/tensorflow_lite/tensorflow/lite/micro/test_helpers.o ./Drivers/tensorflow_lite/tensorflow/lite/micro/test_helpers.su

.PHONY: clean-Drivers-2f-tensorflow_lite-2f-tensorflow-2f-lite-2f-micro

