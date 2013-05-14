################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../NN_XOR.c \
../startup_ccs.c \
../supervisedNN.c 

CMD_SRCS += \
../lm3s1968.cmd 

OBJS += \
./NN_XOR.obj \
./startup_ccs.obj \
./supervisedNN.obj 

C_DEPS += \
./NN_XOR.pp \
./startup_ccs.pp \
./supervisedNN.pp 

OBJS__QTD += \
".\NN_XOR.obj" \
".\startup_ccs.obj" \
".\supervisedNN.obj" 

C_DEPS__QTD += \
".\NN_XOR.pp" \
".\startup_ccs.pp" \
".\supervisedNN.pp" 

C_SRCS_QUOTED += \
"../NN_XOR.c" \
"../startup_ccs.c" \
"../supervisedNN.c" 


# Each subdirectory must supply rules for building sources it contributes
NN_XOR.obj: ../NN_XOR.c $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/bin/cl470" -mv7M3 -g -O2 --gcc --define=ccs --define=PART_LM3S1968 --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/include" --include_path="C:/StellarisWare/boards/MyBoards/LM3S1968/NNXOR/ccs/../../../../.." --include_path="C:/StellarisWare/boards/MyBoards/LM3S1968/NNXOR/ccs/../.." --diag_warning=225 -me --gen_func_subsections --abi=eabi --code_state=16 --ual --preproc_with_compile --preproc_dependency="NN_XOR.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

startup_ccs.obj: ../startup_ccs.c $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/bin/cl470" -mv7M3 -g -O2 --gcc --define=ccs --define=PART_LM3S1968 --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/include" --include_path="C:/StellarisWare/boards/MyBoards/LM3S1968/NNXOR/ccs/../../../../.." --include_path="C:/StellarisWare/boards/MyBoards/LM3S1968/NNXOR/ccs/../.." --diag_warning=225 -me --gen_func_subsections --abi=eabi --code_state=16 --ual --preproc_with_compile --preproc_dependency="startup_ccs.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '

supervisedNN.obj: ../supervisedNN.c $(GEN_OPTS)
	@echo 'Building file: $<'
	@echo 'Invoking: Compiler'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/bin/cl470" -mv7M3 -g -O2 --gcc --define=ccs --define=PART_LM3S1968 --include_path="C:/Program Files/Texas Instruments/ccsv4/tools/compiler/tms470/include" --include_path="C:/StellarisWare/boards/MyBoards/LM3S1968/NNXOR/ccs/../../../../.." --include_path="C:/StellarisWare/boards/MyBoards/LM3S1968/NNXOR/ccs/../.." --diag_warning=225 -me --gen_func_subsections --abi=eabi --code_state=16 --ual --preproc_with_compile --preproc_dependency="supervisedNN.pp" $(GEN_OPTS_QUOTED) $(subst #,$(wildcard $(subst $(SPACE),\$(SPACE),$<)),"#")
	@echo 'Finished building: $<'
	@echo ' '


