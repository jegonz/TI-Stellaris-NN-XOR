################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/rit128x96x4.c 

OBJS += \
./Drivers/rit128x96x4.obj 

C_DEPS += \
./Drivers/rit128x96x4.pp 

OBJS__QTD += \
".\Drivers\rit128x96x4.obj" 

C_DEPS__QTD += \
".\Drivers\rit128x96x4.pp" 

C_SRCS_QUOTED += \
"../Drivers/rit128x96x4.c" 


# Each subdirectory must supply rules for building sources it contributes
Drivers/rit128x96x4.obj: ../Drivers/rit128x96x4.c $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/bin/cl470" -mv7M3 -g -O2 --gcc --define=ccs --define=PART_LM3S1968 --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/include" --include_path="C:/StellarisWare/boards/MyBoards/LM3S1968/NNXOR/ccs/../../../../.." --include_path="C:/StellarisWare/boards/MyBoards/LM3S1968/NNXOR/ccs/../.." --diag_warning=225 -me --gen_func_subsections --abi=eabi --code_state=16 --ual --preproc_with_compile --preproc_dependency="Drivers/rit128x96x4.pp" --obj_directory="Drivers" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '


