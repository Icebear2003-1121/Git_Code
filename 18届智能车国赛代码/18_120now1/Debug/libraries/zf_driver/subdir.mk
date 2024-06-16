################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libraries/zf_driver/zf_driver_adc.c \
../libraries/zf_driver/zf_driver_delay.c \
../libraries/zf_driver/zf_driver_dma.c \
../libraries/zf_driver/zf_driver_encoder.c \
../libraries/zf_driver/zf_driver_exti.c \
../libraries/zf_driver/zf_driver_flash.c \
../libraries/zf_driver/zf_driver_gpio.c \
../libraries/zf_driver/zf_driver_pit.c \
../libraries/zf_driver/zf_driver_pwm.c \
../libraries/zf_driver/zf_driver_soft_iic.c \
../libraries/zf_driver/zf_driver_soft_spi.c \
../libraries/zf_driver/zf_driver_spi.c \
../libraries/zf_driver/zf_driver_timer.c \
../libraries/zf_driver/zf_driver_uart.c 

OBJS += \
./libraries/zf_driver/zf_driver_adc.o \
./libraries/zf_driver/zf_driver_delay.o \
./libraries/zf_driver/zf_driver_dma.o \
./libraries/zf_driver/zf_driver_encoder.o \
./libraries/zf_driver/zf_driver_exti.o \
./libraries/zf_driver/zf_driver_flash.o \
./libraries/zf_driver/zf_driver_gpio.o \
./libraries/zf_driver/zf_driver_pit.o \
./libraries/zf_driver/zf_driver_pwm.o \
./libraries/zf_driver/zf_driver_soft_iic.o \
./libraries/zf_driver/zf_driver_soft_spi.o \
./libraries/zf_driver/zf_driver_spi.o \
./libraries/zf_driver/zf_driver_timer.o \
./libraries/zf_driver/zf_driver_uart.o 

COMPILED_SRCS += \
./libraries/zf_driver/zf_driver_adc.src \
./libraries/zf_driver/zf_driver_delay.src \
./libraries/zf_driver/zf_driver_dma.src \
./libraries/zf_driver/zf_driver_encoder.src \
./libraries/zf_driver/zf_driver_exti.src \
./libraries/zf_driver/zf_driver_flash.src \
./libraries/zf_driver/zf_driver_gpio.src \
./libraries/zf_driver/zf_driver_pit.src \
./libraries/zf_driver/zf_driver_pwm.src \
./libraries/zf_driver/zf_driver_soft_iic.src \
./libraries/zf_driver/zf_driver_soft_spi.src \
./libraries/zf_driver/zf_driver_spi.src \
./libraries/zf_driver/zf_driver_timer.src \
./libraries/zf_driver/zf_driver_uart.src 

C_DEPS += \
./libraries/zf_driver/zf_driver_adc.d \
./libraries/zf_driver/zf_driver_delay.d \
./libraries/zf_driver/zf_driver_dma.d \
./libraries/zf_driver/zf_driver_encoder.d \
./libraries/zf_driver/zf_driver_exti.d \
./libraries/zf_driver/zf_driver_flash.d \
./libraries/zf_driver/zf_driver_gpio.d \
./libraries/zf_driver/zf_driver_pit.d \
./libraries/zf_driver/zf_driver_pwm.d \
./libraries/zf_driver/zf_driver_soft_iic.d \
./libraries/zf_driver/zf_driver_soft_spi.d \
./libraries/zf_driver/zf_driver_spi.d \
./libraries/zf_driver/zf_driver_timer.d \
./libraries/zf_driver/zf_driver_uart.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/zf_driver/%.src: ../libraries/zf_driver/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -D__CPU__=tc37x -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\code" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\Configurations" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\Infra" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\Infra\Platform" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\Infra\Platform\Tricore" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\Infra\Platform\Tricore\Compilers" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\Infra\Sfr" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\Infra\Sfr\TC37A" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\Infra\Sfr\TC37A\_Reg" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\Infra\Ssw" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\Infra\Ssw\TC37A" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\Infra\Ssw\TC37A\Tricore" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\Service" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\Service\CpuGeneric" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\Service\CpuGeneric\If" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\Service\CpuGeneric\If\Ccu6If" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\Service\CpuGeneric\StdIf" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\Service\CpuGeneric\SysSe" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\Service\CpuGeneric\SysSe\Bsp" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\Service\CpuGeneric\SysSe\Comm" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\Service\CpuGeneric\SysSe\General" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\Service\CpuGeneric\SysSe\Math" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\Service\CpuGeneric\SysSe\Time" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\Service\CpuGeneric\_Utilities" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Asclin" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Asclin\Asc" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Asclin\Lin" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Asclin\Spi" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Asclin\Std" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Can" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Can\Can" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Can\Std" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Ccu6" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Ccu6\Icu" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Ccu6\PwmBc" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Ccu6\PwmHl" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Ccu6\Std" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Ccu6\TPwm" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Ccu6\Timer" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Ccu6\TimerWithTrigger" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Convctrl" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Convctrl\Std" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Cpu" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Cpu\Irq" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Cpu\Std" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Cpu\Trap" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Dma" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Dma\Dma" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Dma\Std" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Dts" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Dts\Dts" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Dts\Std" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Edsadc" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Edsadc\Edsadc" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Edsadc\Std" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Eray" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Eray\Eray" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Eray\Std" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Evadc" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Evadc\Adc" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Evadc\Std" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Fce" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Fce\Crc" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Fce\Std" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Flash" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Flash\Std" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Geth" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Geth\Eth" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Geth\Std" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Gpt12" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Gpt12\IncrEnc" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Gpt12\Std" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Gtm" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Gtm\Atom" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Gtm\Atom\Pwm" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Gtm\Atom\PwmHl" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Gtm\Atom\Timer" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Gtm\Std" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Gtm\Tim" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Gtm\Tim\In" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Gtm\Tim\Timer" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Gtm\Tom" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Gtm\Tom\Pwm" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Gtm\Tom\PwmHl" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Gtm\Tom\Timer" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Gtm\Trig" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Hssl" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Hssl\Hssl" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Hssl\Std" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\I2c" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\I2c\I2c" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\I2c\Std" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Iom" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Iom\Driver" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Iom\Iom" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Iom\Std" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Msc" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Msc\Msc" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Msc\Std" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Mtu" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Mtu\Std" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Pms" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Pms\Std" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Port" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Port\Io" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Port\Std" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Psi5" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Psi5\Psi5" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Psi5\Std" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Psi5s" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Psi5s\Psi5s" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Psi5s\Std" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Qspi" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Qspi\SpiMaster" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Qspi\SpiSlave" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Qspi\Std" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Scu" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Scu\Std" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Sent" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Sent\Sent" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Sent\Std" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Smu" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Smu\Smu" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Smu\Std" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Src" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Src\Std" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Stm" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Stm\Std" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\Stm\Timer" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\_Impl" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\_Lib" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\_Lib\DataHandling" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\_Lib\InternalMux" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\infineon_libraries\iLLD\TC37A\Tricore\_PinMap" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\zf_common" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\zf_device" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\libraries\zf_driver" -I"C:\Users\sixsun\AURIX-v1.3.2-workspace\4.15\18_120now1\user" --iso=99 --c++14 --language=+volatile --anachronisms --fp-model=3 --fp-model=c --fp-model=f --fp-model=l --fp-model=n --fp-model=r --fp-model=z -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -o "$@"  "$<"  -cs --dep-file=$(@:.src=.d) --misrac-version=2012 -N0 -Z0 -Y0 2>&1;
	@echo 'Finished building: $<'
	@echo ' '

libraries/zf_driver/%.o: ./libraries/zf_driver/%.src
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -o  "$@" "$<" --list-format=L1 --optimize=gs
	@echo 'Finished building: $<'
	@echo ' '


