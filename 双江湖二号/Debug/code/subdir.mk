################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../code/Contol.c \
../code/Encoder.c \
../code/ImageProcess.c \
../code/Key.c \
../code/Motor.c \
../code/Pid.c \
../code/ScanLine.c \
../code/Steer.c \
../code/ZW_Tools.c \
../code/camera.c 

COMPILED_SRCS += \
./code/Contol.src \
./code/Encoder.src \
./code/ImageProcess.src \
./code/Key.src \
./code/Motor.src \
./code/Pid.src \
./code/ScanLine.src \
./code/Steer.src \
./code/ZW_Tools.src \
./code/camera.src 

C_DEPS += \
./code/Contol.d \
./code/Encoder.d \
./code/ImageProcess.d \
./code/Key.d \
./code/Motor.d \
./code/Pid.d \
./code/ScanLine.d \
./code/Steer.d \
./code/ZW_Tools.d \
./code/camera.d 

OBJS += \
./code/Contol.o \
./code/Encoder.o \
./code/ImageProcess.o \
./code/Key.o \
./code/Motor.o \
./code/Pid.o \
./code/ScanLine.o \
./code/Steer.o \
./code/ZW_Tools.o \
./code/camera.o 


# Each subdirectory must supply rules for building sources it contributes
code/%.src: ../code/%.c code/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -cs --dep-file="$(basename $@).d" --misrac-version=2004 -D__CPU__=tc26xb "-fD:/17.5s/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<" && \
	if [ -f "$(basename $@).d" ]; then sed.exe -r  -e 's/\b(.+\.o)\b/code\/\1/g' -e 's/\\/\//g' -e 's/\/\//\//g' -e 's/"//g' -e 's/([a-zA-Z]:\/)/\L\1/g' -e 's/\d32:/@TARGET_DELIMITER@/g; s/\\\d32/@ESCAPED_SPACE@/g; s/\d32/\\\d32/g; s/@ESCAPED_SPACE@/\\\d32/g; s/@TARGET_DELIMITER@/\d32:/g' "$(basename $@).d" > "$(basename $@).d_sed" && cp "$(basename $@).d_sed" "$(basename $@).d" && rm -f "$(basename $@).d_sed" 2>/dev/null; else echo 'No dependency file to process';fi
	@echo 'Finished building: $<'
	@echo ' '

code/%.o: ./code/%.src code/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-code

clean-code:
	-$(RM) ./code/Contol.d ./code/Contol.o ./code/Contol.src ./code/Encoder.d ./code/Encoder.o ./code/Encoder.src ./code/ImageProcess.d ./code/ImageProcess.o ./code/ImageProcess.src ./code/Key.d ./code/Key.o ./code/Key.src ./code/Motor.d ./code/Motor.o ./code/Motor.src ./code/Pid.d ./code/Pid.o ./code/Pid.src ./code/ScanLine.d ./code/ScanLine.o ./code/ScanLine.src ./code/Steer.d ./code/Steer.o ./code/Steer.src ./code/ZW_Tools.d ./code/ZW_Tools.o ./code/ZW_Tools.src ./code/camera.d ./code/camera.o ./code/camera.src

.PHONY: clean-code

