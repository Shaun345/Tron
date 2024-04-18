################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
ADC.o: C:/Users/shaun/OneDrive/Desktop/S24/ECE\ 319H/MSPM0_ValvanoWare/inc/ADC.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1260/ccs/tools/compiler/ti-cgt-armllvm_3.2.1.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"C:/Users/shaun/OneDrive/Desktop/S24/ECE 319H/MSPM0_ValvanoWare/ADCSWTrigger" -I"C:/Users/shaun/OneDrive/Desktop/S24/ECE 319H/MSPM0_ValvanoWare/ADCSWTrigger/Debug" -I"C:/ti/mspm0_sdk_1_20_01_06/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_1_20_01_06/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"ADC.d_raw" -MT"ADC.o" -I"C:/Users/shaun/OneDrive/Desktop/S24/ECE 319H/MSPM0_ValvanoWare/ADCSWTrigger/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1260/ccs/tools/compiler/ti-cgt-armllvm_3.2.1.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"C:/Users/shaun/OneDrive/Desktop/S24/ECE 319H/MSPM0_ValvanoWare/ADCSWTrigger" -I"C:/Users/shaun/OneDrive/Desktop/S24/ECE 319H/MSPM0_ValvanoWare/ADCSWTrigger/Debug" -I"C:/ti/mspm0_sdk_1_20_01_06/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_1_20_01_06/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)" -I"C:/Users/shaun/OneDrive/Desktop/S24/ECE 319H/MSPM0_ValvanoWare/ADCSWTrigger/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Clock.o: C:/Users/shaun/OneDrive/Desktop/S24/ECE\ 319H/MSPM0_ValvanoWare/inc/Clock.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1260/ccs/tools/compiler/ti-cgt-armllvm_3.2.1.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"C:/Users/shaun/OneDrive/Desktop/S24/ECE 319H/MSPM0_ValvanoWare/ADCSWTrigger" -I"C:/Users/shaun/OneDrive/Desktop/S24/ECE 319H/MSPM0_ValvanoWare/ADCSWTrigger/Debug" -I"C:/ti/mspm0_sdk_1_20_01_06/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_1_20_01_06/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"Clock.d_raw" -MT"Clock.o" -I"C:/Users/shaun/OneDrive/Desktop/S24/ECE 319H/MSPM0_ValvanoWare/ADCSWTrigger/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

JoyStick.o: C:/Users/shaun/OneDrive/Desktop/S24/ECE\ 319H/MSPM0_ValvanoWare/inc/JoyStick.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1260/ccs/tools/compiler/ti-cgt-armllvm_3.2.1.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"C:/Users/shaun/OneDrive/Desktop/S24/ECE 319H/MSPM0_ValvanoWare/ADCSWTrigger" -I"C:/Users/shaun/OneDrive/Desktop/S24/ECE 319H/MSPM0_ValvanoWare/ADCSWTrigger/Debug" -I"C:/ti/mspm0_sdk_1_20_01_06/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_1_20_01_06/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"JoyStick.d_raw" -MT"JoyStick.o" -I"C:/Users/shaun/OneDrive/Desktop/S24/ECE 319H/MSPM0_ValvanoWare/ADCSWTrigger/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

LaunchPad.o: C:/Users/shaun/OneDrive/Desktop/S24/ECE\ 319H/MSPM0_ValvanoWare/inc/LaunchPad.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1260/ccs/tools/compiler/ti-cgt-armllvm_3.2.1.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"C:/Users/shaun/OneDrive/Desktop/S24/ECE 319H/MSPM0_ValvanoWare/ADCSWTrigger" -I"C:/Users/shaun/OneDrive/Desktop/S24/ECE 319H/MSPM0_ValvanoWare/ADCSWTrigger/Debug" -I"C:/ti/mspm0_sdk_1_20_01_06/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_1_20_01_06/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"LaunchPad.d_raw" -MT"LaunchPad.o" -I"C:/Users/shaun/OneDrive/Desktop/S24/ECE 319H/MSPM0_ValvanoWare/ADCSWTrigger/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


