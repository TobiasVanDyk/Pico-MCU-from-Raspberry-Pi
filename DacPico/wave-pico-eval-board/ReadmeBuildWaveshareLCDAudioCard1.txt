Microsoft Windows [Version 10.0.19045.2251]
(c) Microsoft Corporation. All rights reserved.

C:\Windows\system32>setx PICO_SDK_PATH "W:\Pico\pico-sdk"

SUCCESS: Specified value was saved.

C:\Windows\system32>w:

W:\>cd Pico

W:\Pico>cd pico-sdk

W:\Pico\pico-sdk>



**********************************************************************
** Visual Studio 2022 Developer Command Prompt v17.2.5
** Copyright (c) 2022 Microsoft Corporation
**********************************************************************
[ERROR:team_explorer.bat] Directory not found : "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\IDE\CommonExtensions\Microsoft\TeamFoundation\Team Explorer"

C:\Windows\System32>w:

W:\>cd Pico

W:\Pico>cd pico-sdk

W:\Pico\pico-sdk>cd pico-playground
The system cannot find the path specified.

W:\Pico\pico-sdk>cd ..

W:\Pico>cd pico-playground

W:\Pico\pico-playground>cd build

W:\Pico\pico-playground\build>cmake -G "NMake Makefiles" ..
Using PICO_SDK_PATH from environment ('W:\Pico\pico-sdk')
PICO_SDK_PATH is W:/Pico/pico-sdk
Defaulting PICO_PLATFORM to rp2040 since not specified.
Defaulting PICO platform compiler to pico_arm_gcc since not specified.
-- Defaulting build type to 'Release' since not specified.
PICO compiler is pico_arm_gcc
Defaulting PICO_EXTRAS_PATH as sibling of PICO_SDK_PATH: W:/Pico/pico-sdk/../pico-extras
-- The C compiler identification is GNU 11.2.1
-- The CXX compiler identification is GNU 11.2.1
-- The ASM compiler identification is GNU
-- Found assembler: C:/Dev/ArmGNUToolchain/bin/arm-none-eabi-gcc.exe
Build type is Release
Defaulting PICO target board to pico since not specified.
Using board configuration from W:/Pico/pico-sdk/src/boards/include/boards/pico.h
-- Found Python3: C:/Dev/Python310/python.exe (found version "3.10.5") found components: Interpreter
TinyUSB available at W:/Pico/pico-sdk/lib/tinyusb/src/portable/raspberrypi/rp2040; enabling build support for USB.
cyw43-driver available at W:/Pico/pico-sdk/lib/cyw43-driver
lwIP available at W:/Pico/pico-sdk/lib/lwip
lwIP available at W:/Pico/pico-extras/lib/lwip/src/core/tcp.c; TCP/IP support is available.
-- Configuring done
-- Generating done
-- Build files have been written to: W:/Pico/pico-playground/build

W:\Pico\pico-playground\build>nmake

Microsoft (R) Program Maintenance Utility Version 14.32.31332.0
Copyright (C) Microsoft Corporation.  All rights reserved.

Scanning dependencies of target bs2_default
[  0%] Building ASM object pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default.dir/compile_time_choice.S.obj
[  0%] Linking ASM executable bs2_default.elf
[  0%] Built target bs2_default
[  0%] Creating directories for 'PioasmBuild'
[  0%] No download step for 'PioasmBuild'
[  0%] No update step for 'PioasmBuild'
[  0%] No patch step for 'PioasmBuild'
[  0%] Performing configure step for 'PioasmBuild'
loading initial cache file W:/Pico/pico-playground/build/pico-sdk/src/rp2_common/cyw43_driver/pioasm/tmp/PioasmBuild-cache-Release.cmake
-- The CXX compiler identification is MSVC 19.32.31332.0
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Tools/MSVC/14.32.31326/bin/Hostx86/x86/cl.exe - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: W:/Pico/pico-playground/build/pioasm
[  1%] Performing build step for 'PioasmBuild'

Microsoft (R) Program Maintenance Utility Version 14.32.31332.0
Copyright (C) Microsoft Corporation.  All rights reserved.

[ 10%] Building CXX object CMakeFiles/pioasm.dir/main.cpp.obj
main.cpp
[ 20%] Building CXX object CMakeFiles/pioasm.dir/pio_assembler.cpp.obj
pio_assembler.cpp
[ 30%] Building CXX object CMakeFiles/pioasm.dir/pio_disassembler.cpp.obj
pio_disassembler.cpp
[ 40%] Building CXX object CMakeFiles/pioasm.dir/gen/lexer.cpp.obj
lexer.cpp
[ 50%] Building CXX object CMakeFiles/pioasm.dir/gen/parser.cpp.obj
parser.cpp
[ 60%] Building CXX object CMakeFiles/pioasm.dir/c_sdk_output.cpp.obj
c_sdk_output.cpp
[ 70%] Building CXX object CMakeFiles/pioasm.dir/python_output.cpp.obj
python_output.cpp
[ 80%] Building CXX object CMakeFiles/pioasm.dir/hex_output.cpp.obj
hex_output.cpp
[ 90%] Building CXX object CMakeFiles/pioasm.dir/ada_output.cpp.obj
ada_output.cpp
[100%] Linking CXX executable pioasm.exe
[100%] Built target pioasm
[  1%] No install step for 'PioasmBuild'
[  1%] Completed 'PioasmBuild'
[  1%] Built target PioasmBuild
[  1%] Generating bs2_default.bin
[  2%] Generating bs2_default_padded_checksummed.S
[  2%] Built target bs2_default_padded_checksummed_asm
[  2%] Generating audio_i2s.pio.h
[  2%] Built target pico_audio_i2s_audio_i2s_pio_h
[  2%] Creating directories for 'ELF2UF2Build'
[  2%] No download step for 'ELF2UF2Build'
[  2%] No update step for 'ELF2UF2Build'
[  2%] No patch step for 'ELF2UF2Build'
[  2%] Performing configure step for 'ELF2UF2Build'
-- The C compiler identification is MSVC 19.32.31332.0
-- The CXX compiler identification is MSVC 19.32.31332.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Tools/MSVC/14.32.31326/bin/Hostx86/x86/cl.exe - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Tools/MSVC/14.32.31326/bin/Hostx86/x86/cl.exe - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: W:/Pico/pico-playground/build/elf2uf2
[  2%] Performing build step for 'ELF2UF2Build'

Microsoft (R) Program Maintenance Utility Version 14.32.31332.0
Copyright (C) Microsoft Corporation.  All rights reserved.

[ 50%] Building CXX object CMakeFiles/elf2uf2.dir/main.cpp.obj
main.cpp
W:\Pico\pico-sdk\tools\elf2uf2\main.cpp(359): warning C4996: 'fopen': This function or variable may be unsafe. Consider using fopen_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
W:\Pico\pico-sdk\tools\elf2uf2\main.cpp(365): warning C4996: 'fopen': This function or variable may be unsafe. Consider using fopen_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
[100%] Linking CXX executable elf2uf2.exe
[100%] Built target elf2uf2
[  2%] No install step for 'ELF2UF2Build'
[  2%] Completed 'ELF2UF2Build'
[  2%] Built target ELF2UF2Build
Scanning dependencies of target sine_wave_i2s
[  3%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/sine_wave.c.obj
[  3%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdlib/stdlib.c.obj
[  3%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_gpio/gpio.c.obj
[  3%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_claim/claim.c.obj
[  3%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/pico_platform/platform.c.obj
[  3%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_sync/sync.c.obj
[  3%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_irq/irq.c.obj
[  3%] Building ASM object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_irq/irq_handler_chain.S.obj
[  3%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/common/pico_sync/sem.c.obj
[  3%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/common/pico_sync/lock_core.c.obj
[  4%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/common/pico_time/time.c.obj
[  4%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/common/pico_time/timeout_helper.c.obj
[  4%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_timer/timer.c.obj
[  4%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/common/pico_util/datetime.c.obj
[  4%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/common/pico_util/pheap.c.obj
[  4%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/common/pico_util/queue.c.obj
[  4%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/common/pico_sync/mutex.c.obj
[  4%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/common/pico_sync/critical_section.c.obj
[  4%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_uart/uart.c.obj
[  5%] Building ASM object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_divider/divider.S.obj
[  5%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/pico_runtime/runtime.c.obj
[  5%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_clocks/clocks.c.obj
[  5%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_pll/pll.c.obj
[  5%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_vreg/vreg.c.obj
[  5%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_watchdog/watchdog.c.obj
[  5%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_xosc/xosc.c.obj
[  5%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/pico_printf/printf.c.obj
[  5%] Building ASM object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/pico_bit_ops/bit_ops_aeabi.S.obj
[  5%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/pico_bootrom/bootrom.c.obj
[  6%] Building ASM object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/pico_divider/divider.S.obj
[  6%] Building ASM object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_aeabi.S.obj
[  6%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_init_rom.c.obj
[  6%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_math.c.obj
[  6%] Building ASM object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_v1_rom_shim.S.obj
[  6%] Building ASM object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/pico_int64_ops/pico_int64_ops_aeabi.S.obj
[  6%] Building ASM object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_aeabi.S.obj
[  6%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_init_rom.c.obj
[  6%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_math.c.obj
[  6%] Building ASM object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_v1_rom_shim.S.obj
[  7%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/pico_malloc/pico_malloc.c.obj
[  7%] Building ASM object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/pico_mem_ops/mem_ops_aeabi.S.obj
[  7%] Building ASM object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/crt0.S.obj
[  7%] Building CXX object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/new_delete.cpp.obj
[  7%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/binary_info.c.obj
[  7%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio/stdio.c.obj
[  7%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio_uart/stdio_uart.c.obj
[  7%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-extras/src/rp2_common/pico_audio_i2s/audio_i2s.c.obj
[  7%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_dma/dma.c.obj
[  7%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_pio/pio.c.obj
[  8%] Building CXX object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-extras/src/common/pico_audio/audio.cpp.obj
[  8%] Building ASM object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-extras/src/common/pico_audio/audio_utils.S.obj
[  8%] Building C object audio/sine_wave/CMakeFiles/sine_wave_i2s.dir/W_/Pico/pico-extras/src/common/pico_util_buffer/buffer.c.obj
[  8%] Linking CXX executable sine_wave_i2s.elf
[  8%] Built target sine_wave_i2s
[  8%] Generating audio_pwm.pio.h
[  8%] Built target pico_audio_pwm_audio_pwm_pio_h
Scanning dependencies of target sine_wave_pwm
[  8%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/sine_wave.c.obj
[  8%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdlib/stdlib.c.obj
[  8%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_gpio/gpio.c.obj
[  8%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_claim/claim.c.obj
[  8%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/pico_platform/platform.c.obj
[  8%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_sync/sync.c.obj
[  9%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_irq/irq.c.obj
[  9%] Building ASM object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_irq/irq_handler_chain.S.obj
[  9%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/common/pico_sync/sem.c.obj
[  9%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/common/pico_sync/lock_core.c.obj
[  9%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/common/pico_time/time.c.obj
[  9%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/common/pico_time/timeout_helper.c.obj
[  9%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_timer/timer.c.obj
[  9%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/common/pico_util/datetime.c.obj
[  9%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/common/pico_util/pheap.c.obj
[ 10%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/common/pico_util/queue.c.obj
[ 10%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/common/pico_sync/mutex.c.obj
[ 10%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/common/pico_sync/critical_section.c.obj
[ 10%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_uart/uart.c.obj
[ 10%] Building ASM object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_divider/divider.S.obj
[ 10%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/pico_runtime/runtime.c.obj
[ 10%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_clocks/clocks.c.obj
[ 10%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_pll/pll.c.obj
[ 10%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_vreg/vreg.c.obj
[ 10%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_watchdog/watchdog.c.obj
[ 11%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_xosc/xosc.c.obj
[ 11%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/pico_printf/printf.c.obj
[ 11%] Building ASM object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/pico_bit_ops/bit_ops_aeabi.S.obj
[ 11%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/pico_bootrom/bootrom.c.obj
[ 11%] Building ASM object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/pico_divider/divider.S.obj
[ 11%] Building ASM object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_aeabi.S.obj
[ 11%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_init_rom.c.obj
[ 11%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_math.c.obj
[ 11%] Building ASM object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_v1_rom_shim.S.obj
[ 11%] Building ASM object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/pico_int64_ops/pico_int64_ops_aeabi.S.obj
[ 12%] Building ASM object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_aeabi.S.obj
[ 12%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_init_rom.c.obj
[ 12%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_math.c.obj
[ 12%] Building ASM object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_v1_rom_shim.S.obj
[ 12%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/pico_malloc/pico_malloc.c.obj
[ 12%] Building ASM object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/pico_mem_ops/mem_ops_aeabi.S.obj
[ 12%] Building ASM object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/crt0.S.obj
[ 12%] Building CXX object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/new_delete.cpp.obj
[ 12%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/binary_info.c.obj
[ 12%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio/stdio.c.obj
[ 13%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio_uart/stdio_uart.c.obj
[ 13%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-extras/src/rp2_common/pico_audio_pwm/audio_pwm.c.obj
[ 13%] Building CXX object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-extras/src/rp2_common/pico_audio_pwm/sample_encoding.cpp.obj
[ 13%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_dma/dma.c.obj
[ 13%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_pio/pio.c.obj
[ 13%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_interp/interp.c.obj
[ 13%] Building CXX object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-extras/src/common/pico_audio/audio.cpp.obj
[ 13%] Building ASM object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-extras/src/common/pico_audio/audio_utils.S.obj
[ 13%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-extras/src/common/pico_util_buffer/buffer.c.obj
[ 13%] Building C object audio/sine_wave/CMakeFiles/sine_wave_pwm.dir/W_/Pico/pico-sdk/src/rp2_common/pico_multicore/multicore.c.obj
[ 14%] Linking CXX executable sine_wave_pwm.elf
[ 14%] Built target sine_wave_pwm
[ 14%] Generating audio_spdif.pio.h
[ 14%] Built target pico_audio_spdif_audio_spdif_pio_h
Scanning dependencies of target sine_wave_spdif
[ 14%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/sine_wave.c.obj
[ 14%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdlib/stdlib.c.obj
[ 14%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_gpio/gpio.c.obj
[ 14%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_claim/claim.c.obj
[ 14%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/pico_platform/platform.c.obj
[ 14%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_sync/sync.c.obj
[ 14%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_irq/irq.c.obj
[ 14%] Building ASM object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_irq/irq_handler_chain.S.obj
[ 15%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/common/pico_sync/sem.c.obj
[ 15%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/common/pico_sync/lock_core.c.obj
[ 15%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/common/pico_time/time.c.obj
[ 15%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/common/pico_time/timeout_helper.c.obj
[ 15%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_timer/timer.c.obj
[ 15%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/common/pico_util/datetime.c.obj
[ 15%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/common/pico_util/pheap.c.obj
[ 15%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/common/pico_util/queue.c.obj
[ 15%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/common/pico_sync/mutex.c.obj
[ 15%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/common/pico_sync/critical_section.c.obj
[ 16%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_uart/uart.c.obj
[ 16%] Building ASM object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_divider/divider.S.obj
[ 16%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/pico_runtime/runtime.c.obj
[ 16%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_clocks/clocks.c.obj
[ 16%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_pll/pll.c.obj
[ 16%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_vreg/vreg.c.obj
[ 16%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_watchdog/watchdog.c.obj
[ 16%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_xosc/xosc.c.obj
[ 16%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/pico_printf/printf.c.obj
[ 16%] Building ASM object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/pico_bit_ops/bit_ops_aeabi.S.obj
[ 17%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/pico_bootrom/bootrom.c.obj
[ 17%] Building ASM object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/pico_divider/divider.S.obj
[ 17%] Building ASM object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_aeabi.S.obj
[ 17%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_init_rom.c.obj
[ 17%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_math.c.obj
[ 17%] Building ASM object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_v1_rom_shim.S.obj
[ 17%] Building ASM object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/pico_int64_ops/pico_int64_ops_aeabi.S.obj
[ 17%] Building ASM object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_aeabi.S.obj
[ 17%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_init_rom.c.obj
[ 17%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_math.c.obj
[ 18%] Building ASM object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_v1_rom_shim.S.obj
[ 18%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/pico_malloc/pico_malloc.c.obj
[ 18%] Building ASM object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/pico_mem_ops/mem_ops_aeabi.S.obj
[ 18%] Building ASM object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/crt0.S.obj
[ 18%] Building CXX object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/new_delete.cpp.obj
[ 18%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/binary_info.c.obj
[ 18%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio/stdio.c.obj
[ 18%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio_uart/stdio_uart.c.obj
[ 18%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-extras/src/rp2_common/pico_audio_spdif/audio_spdif.c.obj
[ 18%] Building CXX object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-extras/src/rp2_common/pico_audio_spdif/sample_encoding.cpp.obj
[ 19%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_dma/dma.c.obj
[ 19%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_pio/pio.c.obj
[ 19%] Building CXX object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-extras/src/common/pico_audio/audio.cpp.obj
[ 19%] Building ASM object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-extras/src/common/pico_audio/audio_utils.S.obj
[ 19%] Building C object audio/sine_wave/CMakeFiles/sine_wave_spdif.dir/W_/Pico/pico-extras/src/common/pico_util_buffer/buffer.c.obj
[ 19%] Linking CXX executable sine_wave_spdif.elf
[ 19%] Built target sine_wave_spdif
[ 20%] Generating scanvideo.pio.h
[ 20%] Built target pico_scanvideo_scanvideo_pio_h
[ 20%] Generating timing.pio.h
[ 20%] Built target pico_scanvideo_dpi_timing_pio_h
[ 20%] Generating sd_card.pio.h
[ 20%] Built target pico_sd_card_sd_card_pio_h
Scanning dependencies of target popcorn
[ 20%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/popcorn.c.obj
[ 20%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/atlantis.c.obj
[ 20%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/lcd12.c.obj
[ 20%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/lcd18.c.obj
[ 20%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/pico_multicore/multicore.c.obj
[ 20%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/common/pico_sync/sem.c.obj
[ 20%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/common/pico_sync/lock_core.c.obj
[ 20%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_sync/sync.c.obj
[ 21%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_claim/claim.c.obj
[ 21%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/pico_platform/platform.c.obj
[ 21%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/common/pico_time/time.c.obj
[ 21%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/common/pico_time/timeout_helper.c.obj
[ 21%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_timer/timer.c.obj
[ 21%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/common/pico_util/datetime.c.obj
[ 21%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/common/pico_util/pheap.c.obj
[ 21%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/common/pico_util/queue.c.obj
[ 21%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/common/pico_sync/mutex.c.obj
[ 21%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/common/pico_sync/critical_section.c.obj
[ 22%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_irq/irq.c.obj
[ 22%] Building ASM object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_irq/irq_handler_chain.S.obj
[ 22%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdlib/stdlib.c.obj
[ 22%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_gpio/gpio.c.obj
[ 22%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_uart/uart.c.obj
[ 22%] Building ASM object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_divider/divider.S.obj
[ 22%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/pico_runtime/runtime.c.obj
[ 22%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_clocks/clocks.c.obj
[ 22%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_pll/pll.c.obj
[ 22%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_vreg/vreg.c.obj
[ 23%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_watchdog/watchdog.c.obj
[ 23%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_xosc/xosc.c.obj
[ 23%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/pico_printf/printf.c.obj
[ 23%] Building ASM object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/pico_bit_ops/bit_ops_aeabi.S.obj
[ 23%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/pico_bootrom/bootrom.c.obj
[ 23%] Building ASM object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/pico_divider/divider.S.obj
[ 23%] Building ASM object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_aeabi.S.obj
[ 23%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_init_rom.c.obj
[ 23%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_math.c.obj
[ 24%] Building ASM object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_v1_rom_shim.S.obj
[ 24%] Building ASM object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/pico_int64_ops/pico_int64_ops_aeabi.S.obj
[ 24%] Building ASM object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_aeabi.S.obj
[ 24%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_init_rom.c.obj
[ 24%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_math.c.obj
[ 24%] Building ASM object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_v1_rom_shim.S.obj
[ 24%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/pico_malloc/pico_malloc.c.obj
[ 24%] Building ASM object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/pico_mem_ops/mem_ops_aeabi.S.obj
[ 24%] Building ASM object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/crt0.S.obj
[ 24%] Building CXX object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/new_delete.cpp.obj
[ 25%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/binary_info.c.obj
[ 25%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio/stdio.c.obj
[ 25%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio_uart/stdio_uart.c.obj
[ 25%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-extras/src/common/platypus/platypus.c.obj
[ 25%] Building ASM object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-extras/src/common/platypus/decompress_row.S.obj
[ 25%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_interp/interp.c.obj
[ 25%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-extras/src/rp2_common/pico_scanvideo_dpi/scanvideo.c.obj
[ 25%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_dma/dma.c.obj
[ 25%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_pio/pio.c.obj
[ 25%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-extras/src/common/pico_scanvideo/vga_modes.c.obj
[ 26%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-extras/src/common/pico_util_buffer/buffer.c.obj
[ 26%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-extras/src/rp2_common/pico_sd_card/sd_card.c.obj
[ 26%] Building C object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-extras/src/rp2_common/pico_audio_i2s/audio_i2s.c.obj
[ 26%] Building CXX object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-extras/src/common/pico_audio/audio.cpp.obj
[ 26%] Building ASM object apps/popcorn/CMakeFiles/popcorn.dir/W_/Pico/pico-extras/src/common/pico_audio/audio_utils.S.obj
[ 26%] Linking CXX executable popcorn.elf
[ 26%] Built target popcorn
Scanning dependencies of target usb_sound_card
[ 26%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/usb_sound_card.c.obj
[ 26%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdlib/stdlib.c.obj
[ 26%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_gpio/gpio.c.obj
[ 26%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_claim/claim.c.obj
[ 26%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/pico_platform/platform.c.obj
[ 26%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_sync/sync.c.obj
[ 26%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_irq/irq.c.obj
[ 27%] Building ASM object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_irq/irq_handler_chain.S.obj
[ 27%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/common/pico_sync/sem.c.obj
[ 27%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/common/pico_sync/lock_core.c.obj
[ 27%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/common/pico_time/time.c.obj
[ 27%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/common/pico_time/timeout_helper.c.obj
[ 27%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_timer/timer.c.obj
[ 27%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/common/pico_util/datetime.c.obj
[ 27%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/common/pico_util/pheap.c.obj
[ 27%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/common/pico_util/queue.c.obj
[ 27%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/common/pico_sync/mutex.c.obj
[ 28%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/common/pico_sync/critical_section.c.obj
[ 28%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_uart/uart.c.obj
[ 28%] Building ASM object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_divider/divider.S.obj
[ 28%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/pico_runtime/runtime.c.obj
[ 28%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_clocks/clocks.c.obj
[ 28%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_pll/pll.c.obj
[ 28%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_vreg/vreg.c.obj
[ 28%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_watchdog/watchdog.c.obj
[ 28%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_xosc/xosc.c.obj
[ 28%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/pico_printf/printf.c.obj
[ 29%] Building ASM object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/pico_bit_ops/bit_ops_aeabi.S.obj
[ 29%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/pico_bootrom/bootrom.c.obj
[ 29%] Building ASM object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/pico_divider/divider.S.obj
[ 29%] Building ASM object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_aeabi.S.obj
[ 29%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_init_rom.c.obj
[ 29%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_math.c.obj
[ 29%] Building ASM object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_v1_rom_shim.S.obj
[ 29%] Building ASM object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/pico_int64_ops/pico_int64_ops_aeabi.S.obj
[ 29%] Building ASM object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_aeabi.S.obj
[ 29%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_init_rom.c.obj
[ 30%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_math.c.obj
[ 30%] Building ASM object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_v1_rom_shim.S.obj
[ 30%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/pico_malloc/pico_malloc.c.obj
[ 30%] Building ASM object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/pico_mem_ops/mem_ops_aeabi.S.obj
[ 30%] Building ASM object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/crt0.S.obj
[ 30%] Building CXX object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/new_delete.cpp.obj
[ 30%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/binary_info.c.obj
[ 30%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio/stdio.c.obj
[ 30%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio_uart/stdio_uart.c.obj
[ 30%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-extras/src/rp2_common/usb_device/usb_device.c.obj
[ 31%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-extras/src/rp2_common/usb_device/usb_stream_helper.c.obj
[ 31%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_dma/dma.c.obj
[ 31%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_pio/pio.c.obj
[ 31%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/pico_fix/rp2040_usb_device_enumeration/rp2040_usb_device_enumeration.c.obj
[ 31%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-extras/src/rp2_common/pico_audio_i2s/audio_i2s.c.obj
[ 31%] Building CXX object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-extras/src/common/pico_audio/audio.cpp.obj
[ 31%] Building ASM object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-extras/src/common/pico_audio/audio_utils.S.obj
[ 31%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-extras/src/common/pico_util_buffer/buffer.c.obj
[ 31%] Building C object apps/usb_sound_card/CMakeFiles/usb_sound_card.dir/W_/Pico/pico-sdk/src/rp2_common/pico_multicore/multicore.c.obj
[ 32%] Linking CXX executable usb_sound_card.elf
[ 32%] Built target usb_sound_card
Scanning dependencies of target demo1
[ 32%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/demo1.c.obj
[ 32%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/data.c.obj
[ 32%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdlib/stdlib.c.obj
[ 32%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_gpio/gpio.c.obj
[ 32%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_claim/claim.c.obj
[ 32%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/pico_platform/platform.c.obj
[ 33%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_sync/sync.c.obj
[ 33%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_irq/irq.c.obj
[ 33%] Building ASM object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_irq/irq_handler_chain.S.obj
[ 33%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/common/pico_sync/sem.c.obj
[ 33%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/common/pico_sync/lock_core.c.obj
[ 33%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/common/pico_time/time.c.obj
[ 33%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/common/pico_time/timeout_helper.c.obj
[ 33%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_timer/timer.c.obj
[ 33%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/common/pico_util/datetime.c.obj
[ 33%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/common/pico_util/pheap.c.obj
[ 34%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/common/pico_util/queue.c.obj
[ 34%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/common/pico_sync/mutex.c.obj
[ 34%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/common/pico_sync/critical_section.c.obj
[ 34%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_uart/uart.c.obj
[ 34%] Building ASM object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_divider/divider.S.obj
[ 34%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/pico_runtime/runtime.c.obj
[ 34%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_clocks/clocks.c.obj
[ 34%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_pll/pll.c.obj
[ 34%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_vreg/vreg.c.obj
[ 35%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_watchdog/watchdog.c.obj
[ 35%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_xosc/xosc.c.obj
[ 35%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/pico_printf/printf.c.obj
[ 35%] Building ASM object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/pico_bit_ops/bit_ops_aeabi.S.obj
[ 35%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/pico_bootrom/bootrom.c.obj
[ 35%] Building ASM object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/pico_divider/divider.S.obj
[ 35%] Building ASM object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_aeabi.S.obj
[ 35%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_init_rom.c.obj
[ 35%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_math.c.obj
[ 35%] Building ASM object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_v1_rom_shim.S.obj
[ 36%] Building ASM object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/pico_int64_ops/pico_int64_ops_aeabi.S.obj
[ 36%] Building ASM object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_aeabi.S.obj
[ 36%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_init_rom.c.obj
[ 36%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_math.c.obj
[ 36%] Building ASM object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_v1_rom_shim.S.obj
[ 36%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/pico_malloc/pico_malloc.c.obj
[ 36%] Building ASM object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/pico_mem_ops/mem_ops_aeabi.S.obj
[ 36%] Building ASM object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/crt0.S.obj
[ 36%] Building CXX object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/new_delete.cpp.obj
[ 36%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/binary_info.c.obj
[ 37%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio/stdio.c.obj
[ 37%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio_uart/stdio_uart.c.obj
[ 37%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-extras/src/rp2_common/pico_scanvideo_dpi/scanvideo.c.obj
[ 37%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_dma/dma.c.obj
[ 37%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_pio/pio.c.obj
[ 37%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-extras/src/common/pico_scanvideo/vga_modes.c.obj
[ 37%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-extras/src/common/pico_util_buffer/buffer.c.obj
[ 37%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/__/render/image.c.obj
[ 37%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/__/render/spans.c.obj
[ 37%] Building C object scanvideo/demo1/CMakeFiles/demo1.dir/W_/Pico/pico-sdk/src/rp2_common/pico_multicore/multicore.c.obj
[ 38%] Linking CXX executable demo1.elf
[ 38%] Built target demo1
Scanning dependencies of target hscroll_dma_tiles
[ 38%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/hscroll_dma_tiles.c.obj
[ 38%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/data.c.obj
[ 38%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdlib/stdlib.c.obj
[ 38%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_gpio/gpio.c.obj
[ 39%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_claim/claim.c.obj
[ 39%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_platform/platform.c.obj
[ 39%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_sync/sync.c.obj
[ 39%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_irq/irq.c.obj
[ 39%] Building ASM object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_irq/irq_handler_chain.S.obj
[ 39%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/common/pico_sync/sem.c.obj
[ 39%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/common/pico_sync/lock_core.c.obj
[ 39%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/common/pico_time/time.c.obj
[ 39%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/common/pico_time/timeout_helper.c.obj
[ 40%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_timer/timer.c.obj
[ 40%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/common/pico_util/datetime.c.obj
[ 40%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/common/pico_util/pheap.c.obj
[ 40%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/common/pico_util/queue.c.obj
[ 40%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/common/pico_sync/mutex.c.obj
[ 40%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/common/pico_sync/critical_section.c.obj
[ 40%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_uart/uart.c.obj
[ 40%] Building ASM object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_divider/divider.S.obj
[ 40%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_runtime/runtime.c.obj
[ 40%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_clocks/clocks.c.obj
[ 41%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_pll/pll.c.obj
[ 41%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_vreg/vreg.c.obj
[ 41%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_watchdog/watchdog.c.obj
[ 41%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_xosc/xosc.c.obj
[ 41%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_printf/printf.c.obj
[ 41%] Building ASM object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_bit_ops/bit_ops_aeabi.S.obj
[ 41%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_bootrom/bootrom.c.obj
[ 41%] Building ASM object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_divider/divider.S.obj
[ 41%] Building ASM object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_aeabi.S.obj
[ 41%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_init_rom.c.obj
[ 42%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_math.c.obj
[ 42%] Building ASM object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_v1_rom_shim.S.obj
[ 42%] Building ASM object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_int64_ops/pico_int64_ops_aeabi.S.obj
[ 42%] Building ASM object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_aeabi.S.obj
[ 42%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_init_rom.c.obj
[ 42%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_math.c.obj
[ 42%] Building ASM object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_v1_rom_shim.S.obj
[ 42%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_malloc/pico_malloc.c.obj
[ 42%] Building ASM object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_mem_ops/mem_ops_aeabi.S.obj
[ 42%] Building ASM object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/crt0.S.obj
[ 43%] Building CXX object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/new_delete.cpp.obj
[ 43%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/binary_info.c.obj
[ 43%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio/stdio.c.obj
[ 43%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio_uart/stdio_uart.c.obj
[ 43%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_multicore/multicore.c.obj
[ 43%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-extras/src/rp2_common/pico_scanvideo_dpi/scanvideo.c.obj
[ 43%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_dma/dma.c.obj
[ 43%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_pio/pio.c.obj
[ 43%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-extras/src/common/pico_scanvideo/vga_modes.c.obj
[ 43%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/W_/Pico/pico-extras/src/common/pico_util_buffer/buffer.c.obj
[ 44%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/__/render/image.c.obj
[ 44%] Building C object scanvideo/hscroll_dma_tiles/CMakeFiles/hscroll_dma_tiles.dir/__/render/spans.c.obj
[ 44%] Linking CXX executable hscroll_dma_tiles.elf
[ 44%] Built target hscroll_dma_tiles
Scanning dependencies of target flash_stream
[ 44%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/flash_stream.c.obj
[ 44%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdlib/stdlib.c.obj
[ 44%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_gpio/gpio.c.obj
[ 44%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_claim/claim.c.obj
[ 44%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/pico_platform/platform.c.obj
[ 44%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_sync/sync.c.obj
[ 44%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_irq/irq.c.obj
[ 44%] Building ASM object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_irq/irq_handler_chain.S.obj
[ 44%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/common/pico_sync/sem.c.obj
[ 45%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/common/pico_sync/lock_core.c.obj
[ 45%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/common/pico_time/time.c.obj
[ 45%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/common/pico_time/timeout_helper.c.obj
[ 45%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_timer/timer.c.obj
[ 45%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/common/pico_util/datetime.c.obj
[ 45%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/common/pico_util/pheap.c.obj
[ 45%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/common/pico_util/queue.c.obj
[ 45%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/common/pico_sync/mutex.c.obj
[ 45%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/common/pico_sync/critical_section.c.obj
[ 46%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_uart/uart.c.obj
[ 46%] Building ASM object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_divider/divider.S.obj
[ 46%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/pico_runtime/runtime.c.obj
[ 46%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_clocks/clocks.c.obj
[ 46%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_pll/pll.c.obj
[ 46%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_vreg/vreg.c.obj
[ 46%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_watchdog/watchdog.c.obj
[ 46%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_xosc/xosc.c.obj
[ 46%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/pico_printf/printf.c.obj
[ 46%] Building ASM object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/pico_bit_ops/bit_ops_aeabi.S.obj
[ 47%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/pico_bootrom/bootrom.c.obj
[ 47%] Building ASM object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/pico_divider/divider.S.obj
[ 47%] Building ASM object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_aeabi.S.obj
[ 47%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_init_rom.c.obj
[ 47%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_math.c.obj
[ 47%] Building ASM object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_v1_rom_shim.S.obj
[ 47%] Building ASM object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/pico_int64_ops/pico_int64_ops_aeabi.S.obj
[ 47%] Building ASM object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_aeabi.S.obj
[ 47%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_init_rom.c.obj
[ 47%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_math.c.obj
[ 48%] Building ASM object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_v1_rom_shim.S.obj
[ 48%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/pico_malloc/pico_malloc.c.obj
[ 48%] Building ASM object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/pico_mem_ops/mem_ops_aeabi.S.obj
[ 48%] Building ASM object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/crt0.S.obj
[ 48%] Building CXX object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/new_delete.cpp.obj
[ 48%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/binary_info.c.obj
[ 48%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio/stdio.c.obj
[ 48%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio_uart/stdio_uart.c.obj
[ 48%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-extras/src/rp2_common/pico_scanvideo_dpi/scanvideo.c.obj
[ 48%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_dma/dma.c.obj
[ 49%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_pio/pio.c.obj
[ 49%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-extras/src/common/pico_scanvideo/vga_modes.c.obj
[ 49%] Building C object scanvideo/flash_stream/CMakeFiles/flash_stream.dir/W_/Pico/pico-extras/src/common/pico_util_buffer/buffer.c.obj
[ 49%] Linking CXX executable flash_stream.elf
[ 49%] Built target flash_stream
Scanning dependencies of target mandelbrot
[ 49%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/mandelbrot.c.obj
[ 49%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdlib/stdlib.c.obj
[ 49%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_gpio/gpio.c.obj
[ 49%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_claim/claim.c.obj
[ 49%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/pico_platform/platform.c.obj
[ 49%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_sync/sync.c.obj
[ 50%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_irq/irq.c.obj
[ 50%] Building ASM object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_irq/irq_handler_chain.S.obj
[ 50%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/common/pico_sync/sem.c.obj
[ 50%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/common/pico_sync/lock_core.c.obj
[ 50%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/common/pico_time/time.c.obj
[ 50%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/common/pico_time/timeout_helper.c.obj
[ 50%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_timer/timer.c.obj
[ 50%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/common/pico_util/datetime.c.obj
[ 50%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/common/pico_util/pheap.c.obj
[ 50%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/common/pico_util/queue.c.obj
[ 51%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/common/pico_sync/mutex.c.obj
[ 51%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/common/pico_sync/critical_section.c.obj
[ 51%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_uart/uart.c.obj
[ 51%] Building ASM object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_divider/divider.S.obj
[ 51%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/pico_runtime/runtime.c.obj
[ 51%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_clocks/clocks.c.obj
[ 51%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_pll/pll.c.obj
[ 51%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_vreg/vreg.c.obj
[ 51%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_watchdog/watchdog.c.obj
[ 51%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_xosc/xosc.c.obj
[ 52%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/pico_printf/printf.c.obj
[ 52%] Building ASM object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/pico_bit_ops/bit_ops_aeabi.S.obj
[ 52%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/pico_bootrom/bootrom.c.obj
[ 52%] Building ASM object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/pico_divider/divider.S.obj
[ 52%] Building ASM object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_aeabi.S.obj
[ 52%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_init_rom.c.obj
[ 52%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_math.c.obj
[ 52%] Building ASM object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_v1_rom_shim.S.obj
[ 52%] Building ASM object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/pico_int64_ops/pico_int64_ops_aeabi.S.obj
[ 52%] Building ASM object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_aeabi.S.obj
[ 53%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_init_rom.c.obj
[ 53%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_math.c.obj
[ 53%] Building ASM object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_v1_rom_shim.S.obj
[ 53%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/pico_malloc/pico_malloc.c.obj
[ 53%] Building ASM object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/pico_mem_ops/mem_ops_aeabi.S.obj
[ 53%] Building ASM object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/crt0.S.obj
[ 53%] Building CXX object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/new_delete.cpp.obj
[ 53%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/binary_info.c.obj
[ 53%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio/stdio.c.obj
[ 53%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio_uart/stdio_uart.c.obj
[ 54%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-extras/src/rp2_common/pico_scanvideo_dpi/scanvideo.c.obj
[ 54%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_dma/dma.c.obj
[ 54%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_pio/pio.c.obj
[ 54%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-extras/src/common/pico_scanvideo/vga_modes.c.obj
[ 54%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-extras/src/common/pico_util_buffer/buffer.c.obj
[ 54%] Building C object scanvideo/mandelbrot/CMakeFiles/mandelbrot.dir/W_/Pico/pico-sdk/src/rp2_common/pico_multicore/multicore.c.obj
[ 54%] Linking CXX executable mandelbrot.elf
[ 54%] Built target mandelbrot
Scanning dependencies of target mario_tiles
[ 54%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/mario_tiles.c.obj
[ 54%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/data.c.obj
[ 55%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdlib/stdlib.c.obj
[ 55%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_gpio/gpio.c.obj
[ 55%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_claim/claim.c.obj
[ 55%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_platform/platform.c.obj
[ 55%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_sync/sync.c.obj
[ 55%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_irq/irq.c.obj
[ 55%] Building ASM object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_irq/irq_handler_chain.S.obj
[ 55%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/common/pico_sync/sem.c.obj
[ 55%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/common/pico_sync/lock_core.c.obj
[ 55%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/common/pico_time/time.c.obj
[ 56%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/common/pico_time/timeout_helper.c.obj
[ 56%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_timer/timer.c.obj
[ 56%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/common/pico_util/datetime.c.obj
[ 56%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/common/pico_util/pheap.c.obj
[ 56%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/common/pico_util/queue.c.obj
[ 56%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/common/pico_sync/mutex.c.obj
[ 56%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/common/pico_sync/critical_section.c.obj
[ 56%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_uart/uart.c.obj
[ 56%] Building ASM object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_divider/divider.S.obj
[ 56%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_runtime/runtime.c.obj
[ 57%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_clocks/clocks.c.obj
[ 57%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_pll/pll.c.obj
[ 57%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_vreg/vreg.c.obj
[ 57%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_watchdog/watchdog.c.obj
[ 57%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_xosc/xosc.c.obj
[ 57%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_printf/printf.c.obj
[ 57%] Building ASM object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_bit_ops/bit_ops_aeabi.S.obj
[ 57%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_bootrom/bootrom.c.obj
[ 57%] Building ASM object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_divider/divider.S.obj
[ 57%] Building ASM object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_aeabi.S.obj
[ 58%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_init_rom.c.obj
[ 58%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_math.c.obj
[ 58%] Building ASM object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_v1_rom_shim.S.obj
[ 58%] Building ASM object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_int64_ops/pico_int64_ops_aeabi.S.obj
[ 58%] Building ASM object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_aeabi.S.obj
[ 58%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_init_rom.c.obj
[ 58%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_math.c.obj
[ 58%] Building ASM object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_v1_rom_shim.S.obj
[ 58%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_malloc/pico_malloc.c.obj
[ 58%] Building ASM object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_mem_ops/mem_ops_aeabi.S.obj
[ 59%] Building ASM object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/crt0.S.obj
[ 59%] Building CXX object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/new_delete.cpp.obj
[ 59%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/binary_info.c.obj
[ 59%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio/stdio.c.obj
[ 59%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio_uart/stdio_uart.c.obj
[ 59%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/pico_multicore/multicore.c.obj
[ 59%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-extras/src/rp2_common/pico_scanvideo_dpi/scanvideo.c.obj
[ 59%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_dma/dma.c.obj
[ 59%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_pio/pio.c.obj
[ 60%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-extras/src/common/pico_scanvideo/vga_modes.c.obj
[ 60%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-extras/src/common/pico_util_buffer/buffer.c.obj
[ 60%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/__/render/image.c.obj
[ 60%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/__/render/spans.c.obj
[ 60%] Building C object scanvideo/mario_tiles/CMakeFiles/mario_tiles.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_interp/interp.c.obj
[ 60%] Linking CXX executable mario_tiles.elf
[ 60%] Built target mario_tiles
Scanning dependencies of target scanvideo_minimal
[ 60%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/scanvideo_minimal.c.obj
[ 60%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/pico_multicore/multicore.c.obj
[ 60%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/common/pico_sync/sem.c.obj
[ 60%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/common/pico_sync/lock_core.c.obj
[ 61%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_sync/sync.c.obj
[ 61%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_claim/claim.c.obj
[ 61%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/pico_platform/platform.c.obj
[ 61%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/common/pico_time/time.c.obj
[ 61%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/common/pico_time/timeout_helper.c.obj
[ 61%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_timer/timer.c.obj
[ 61%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/common/pico_util/datetime.c.obj
[ 61%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/common/pico_util/pheap.c.obj
[ 61%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/common/pico_util/queue.c.obj
[ 61%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/common/pico_sync/mutex.c.obj
[ 62%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/common/pico_sync/critical_section.c.obj
[ 62%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_irq/irq.c.obj
[ 62%] Building ASM object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_irq/irq_handler_chain.S.obj
[ 62%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdlib/stdlib.c.obj
[ 62%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_gpio/gpio.c.obj
[ 62%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_uart/uart.c.obj
[ 62%] Building ASM object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_divider/divider.S.obj
[ 62%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/pico_runtime/runtime.c.obj
[ 62%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_clocks/clocks.c.obj
[ 63%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_pll/pll.c.obj
[ 63%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_vreg/vreg.c.obj
[ 63%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_watchdog/watchdog.c.obj
[ 63%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_xosc/xosc.c.obj
[ 63%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/pico_printf/printf.c.obj
[ 63%] Building ASM object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/pico_bit_ops/bit_ops_aeabi.S.obj
[ 63%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/pico_bootrom/bootrom.c.obj
[ 63%] Building ASM object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/pico_divider/divider.S.obj
[ 63%] Building ASM object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_aeabi.S.obj
[ 63%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_init_rom.c.obj
[ 64%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_math.c.obj
[ 64%] Building ASM object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_v1_rom_shim.S.obj
[ 64%] Building ASM object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/pico_int64_ops/pico_int64_ops_aeabi.S.obj
[ 64%] Building ASM object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_aeabi.S.obj
[ 64%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_init_rom.c.obj
[ 64%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_math.c.obj
[ 64%] Building ASM object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_v1_rom_shim.S.obj
[ 64%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/pico_malloc/pico_malloc.c.obj
[ 64%] Building ASM object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/pico_mem_ops/mem_ops_aeabi.S.obj
[ 64%] Building ASM object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/crt0.S.obj
[ 65%] Building CXX object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/new_delete.cpp.obj
[ 65%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/binary_info.c.obj
[ 65%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio/stdio.c.obj
[ 65%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio_uart/stdio_uart.c.obj
[ 65%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-extras/src/rp2_common/pico_scanvideo_dpi/scanvideo.c.obj
[ 65%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_dma/dma.c.obj
[ 65%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_pio/pio.c.obj
[ 65%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-extras/src/common/pico_scanvideo/vga_modes.c.obj
[ 65%] Building C object scanvideo/scanvideo_minimal/CMakeFiles/scanvideo_minimal.dir/W_/Pico/pico-extras/src/common/pico_util_buffer/buffer.c.obj
[ 65%] Linking CXX executable scanvideo_minimal.elf
[ 65%] Built target scanvideo_minimal
Scanning dependencies of target sprite_demo
[ 65%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/sprite_demo.c.obj
[ 65%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/pico_multicore/multicore.c.obj
[ 65%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/common/pico_sync/sem.c.obj
[ 66%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/common/pico_sync/lock_core.c.obj
[ 66%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_sync/sync.c.obj
[ 66%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_claim/claim.c.obj
[ 66%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/pico_platform/platform.c.obj
[ 66%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/common/pico_time/time.c.obj
[ 66%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/common/pico_time/timeout_helper.c.obj
[ 66%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_timer/timer.c.obj
[ 66%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/common/pico_util/datetime.c.obj
[ 66%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/common/pico_util/pheap.c.obj
[ 66%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/common/pico_util/queue.c.obj
[ 67%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/common/pico_sync/mutex.c.obj
[ 67%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/common/pico_sync/critical_section.c.obj
[ 67%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_irq/irq.c.obj
[ 67%] Building ASM object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_irq/irq_handler_chain.S.obj
[ 67%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdlib/stdlib.c.obj
[ 67%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_gpio/gpio.c.obj
[ 67%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_uart/uart.c.obj
[ 67%] Building ASM object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_divider/divider.S.obj
[ 67%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/pico_runtime/runtime.c.obj
[ 67%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_clocks/clocks.c.obj
[ 68%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_pll/pll.c.obj
[ 68%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_vreg/vreg.c.obj
[ 68%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_watchdog/watchdog.c.obj
[ 68%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_xosc/xosc.c.obj
[ 68%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/pico_printf/printf.c.obj
[ 68%] Building ASM object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/pico_bit_ops/bit_ops_aeabi.S.obj
[ 68%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/pico_bootrom/bootrom.c.obj
[ 68%] Building ASM object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/pico_divider/divider.S.obj
[ 68%] Building ASM object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_aeabi.S.obj
[ 68%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_init_rom.c.obj
[ 69%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_math.c.obj
[ 69%] Building ASM object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_v1_rom_shim.S.obj
[ 69%] Building ASM object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/pico_int64_ops/pico_int64_ops_aeabi.S.obj
[ 69%] Building ASM object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_aeabi.S.obj
[ 69%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_init_rom.c.obj
[ 69%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_math.c.obj
[ 69%] Building ASM object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_v1_rom_shim.S.obj
[ 69%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/pico_malloc/pico_malloc.c.obj
[ 69%] Building ASM object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/pico_mem_ops/mem_ops_aeabi.S.obj
[ 69%] Building ASM object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/crt0.S.obj
[ 70%] Building CXX object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/new_delete.cpp.obj
[ 70%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/binary_info.c.obj
[ 70%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio/stdio.c.obj
[ 70%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio_uart/stdio_uart.c.obj
[ 70%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-extras/src/rp2_common/pico_scanvideo_dpi/scanvideo.c.obj
[ 70%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_dma/dma.c.obj
[ 70%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_pio/pio.c.obj
[ 70%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-extras/src/common/pico_scanvideo/vga_modes.c.obj
[ 70%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-extras/src/common/pico_util_buffer/buffer.c.obj
[ 71%] Building ASM object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/__/sprite/sprite.S.obj
[ 71%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/__/sprite/sprite.c.obj
[ 71%] Building C object scanvideo/sprite_demo/CMakeFiles/sprite_demo.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_interp/interp.c.obj
[ 71%] Linking CXX executable sprite_demo.elf
[ 71%] Built target sprite_demo
Scanning dependencies of target test_pattern
[ 71%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/test_pattern.c.obj
[ 71%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/pico_multicore/multicore.c.obj
[ 71%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/common/pico_sync/sem.c.obj
[ 71%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/common/pico_sync/lock_core.c.obj
[ 71%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_sync/sync.c.obj
[ 71%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_claim/claim.c.obj
[ 72%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/pico_platform/platform.c.obj
[ 72%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/common/pico_time/time.c.obj
[ 72%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/common/pico_time/timeout_helper.c.obj
[ 72%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_timer/timer.c.obj
[ 72%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/common/pico_util/datetime.c.obj
[ 72%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/common/pico_util/pheap.c.obj
[ 72%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/common/pico_util/queue.c.obj
[ 72%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/common/pico_sync/mutex.c.obj
[ 72%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/common/pico_sync/critical_section.c.obj
[ 72%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_irq/irq.c.obj
[ 73%] Building ASM object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_irq/irq_handler_chain.S.obj
[ 73%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdlib/stdlib.c.obj
[ 73%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_gpio/gpio.c.obj
[ 73%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_uart/uart.c.obj
[ 73%] Building ASM object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_divider/divider.S.obj
[ 73%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/pico_runtime/runtime.c.obj
[ 73%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_clocks/clocks.c.obj
[ 73%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_pll/pll.c.obj
[ 73%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_vreg/vreg.c.obj
[ 73%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_watchdog/watchdog.c.obj
[ 74%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_xosc/xosc.c.obj
[ 74%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/pico_printf/printf.c.obj
[ 74%] Building ASM object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/pico_bit_ops/bit_ops_aeabi.S.obj
[ 74%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/pico_bootrom/bootrom.c.obj
[ 74%] Building ASM object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/pico_divider/divider.S.obj
[ 74%] Building ASM object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_aeabi.S.obj
[ 74%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_init_rom.c.obj
[ 74%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_math.c.obj
[ 74%] Building ASM object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_v1_rom_shim.S.obj
[ 74%] Building ASM object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/pico_int64_ops/pico_int64_ops_aeabi.S.obj
[ 75%] Building ASM object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_aeabi.S.obj
[ 75%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_init_rom.c.obj
[ 75%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_math.c.obj
[ 75%] Building ASM object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_v1_rom_shim.S.obj
[ 75%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/pico_malloc/pico_malloc.c.obj
[ 75%] Building ASM object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/pico_mem_ops/mem_ops_aeabi.S.obj
[ 75%] Building ASM object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/crt0.S.obj
[ 75%] Building CXX object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/new_delete.cpp.obj
[ 75%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/binary_info.c.obj
[ 76%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio/stdio.c.obj
[ 76%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio_uart/stdio_uart.c.obj
[ 76%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio_usb/reset_interface.c.obj
[ 76%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio_usb/stdio_usb.c.obj
[ 76%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio_usb/stdio_usb_descriptors.c.obj
[ 76%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/lib/tinyusb/src/portable/raspberrypi/rp2040/dcd_rp2040.c.obj
[ 76%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/lib/tinyusb/src/portable/raspberrypi/rp2040/rp2040_usb.c.obj
W:\Pico\pico-sdk\lib\tinyusb\src\portable\raspberrypi\rp2040\rp2040_usb.c: In function 'rp2040_usb_init':
W:\Pico\pico-sdk\lib\tinyusb\src\portable\raspberrypi\rp2040\rp2040_usb.c:61:3: warning: 'memset' writing 156 bytes into a region of size 0 overflows the destination [-Wstringop-overflow=]
   61 |   memset(usb_hw, 0, sizeof(*usb_hw));
      |   ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
W:\Pico\pico-sdk\lib\tinyusb\src\portable\raspberrypi\rp2040\rp2040_usb.c:62:3: warning: 'memset' writing 4096 bytes into a region of size 0 overflows the destination [-Wstringop-overflow=]
   62 |   memset(usb_dpram, 0, sizeof(*usb_dpram));
      |   ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
[ 76%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/lib/tinyusb/src/device/usbd.c.obj
[ 76%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/lib/tinyusb/src/device/usbd_control.c.obj
[ 76%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/lib/tinyusb/src/class/audio/audio_device.c.obj
[ 77%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/lib/tinyusb/src/class/cdc/cdc_device.c.obj
[ 77%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/lib/tinyusb/src/class/dfu/dfu_device.c.obj
[ 77%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/lib/tinyusb/src/class/dfu/dfu_rt_device.c.obj
[ 77%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/lib/tinyusb/src/class/hid/hid_device.c.obj
[ 77%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/lib/tinyusb/src/class/midi/midi_device.c.obj
[ 77%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/lib/tinyusb/src/class/msc/msc_device.c.obj
[ 77%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/lib/tinyusb/src/class/net/ecm_rndis_device.c.obj
[ 77%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/lib/tinyusb/src/class/net/ncm_device.c.obj
[ 77%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/lib/tinyusb/src/class/usbtmc/usbtmc_device.c.obj
[ 77%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/lib/tinyusb/src/class/vendor/vendor_device.c.obj
[ 78%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/lib/tinyusb/src/class/video/video_device.c.obj
[ 78%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/lib/tinyusb/src/tusb.c.obj
[ 78%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/lib/tinyusb/src/common/tusb_fifo.c.obj
[ 78%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/pico_fix/rp2040_usb_device_enumeration/rp2040_usb_device_enumeration.c.obj
[ 78%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/pico_unique_id/unique_id.c.obj
[ 78%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_flash/flash.c.obj
[ 78%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-extras/src/rp2_common/pico_scanvideo_dpi/scanvideo.c.obj
[ 78%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_dma/dma.c.obj
[ 78%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_pio/pio.c.obj
[ 78%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-extras/src/common/pico_scanvideo/vga_modes.c.obj
[ 79%] Building C object scanvideo/test_pattern/CMakeFiles/test_pattern.dir/W_/Pico/pico-extras/src/common/pico_util_buffer/buffer.c.obj
[ 79%] Linking CXX executable test_pattern.elf
[ 79%] Built target test_pattern
Scanning dependencies of target textmode
[ 79%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/textmode.c.obj
[ 79%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/font6.c.obj
[ 79%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/font8.c.obj
[ 79%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/font10.c.obj
[ 79%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/lcd.c.obj
[ 79%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/pico_multicore/multicore.c.obj
[ 79%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/common/pico_sync/sem.c.obj
[ 79%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/common/pico_sync/lock_core.c.obj
[ 80%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_sync/sync.c.obj
[ 80%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_claim/claim.c.obj
[ 80%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/pico_platform/platform.c.obj
[ 80%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/common/pico_time/time.c.obj
[ 80%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/common/pico_time/timeout_helper.c.obj
[ 80%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_timer/timer.c.obj
[ 80%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/common/pico_util/datetime.c.obj
[ 80%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/common/pico_util/pheap.c.obj
[ 80%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/common/pico_util/queue.c.obj
[ 81%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/common/pico_sync/mutex.c.obj
[ 81%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/common/pico_sync/critical_section.c.obj
[ 81%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_irq/irq.c.obj
[ 81%] Building ASM object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_irq/irq_handler_chain.S.obj
[ 81%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdlib/stdlib.c.obj
[ 81%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_gpio/gpio.c.obj
[ 81%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_uart/uart.c.obj
[ 81%] Building ASM object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_divider/divider.S.obj
[ 81%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/pico_runtime/runtime.c.obj
[ 81%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_clocks/clocks.c.obj
[ 82%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_pll/pll.c.obj
[ 82%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_vreg/vreg.c.obj
[ 82%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_watchdog/watchdog.c.obj
[ 82%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_xosc/xosc.c.obj
[ 82%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/pico_printf/printf.c.obj
[ 82%] Building ASM object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/pico_bit_ops/bit_ops_aeabi.S.obj
[ 82%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/pico_bootrom/bootrom.c.obj
[ 82%] Building ASM object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/pico_divider/divider.S.obj
[ 82%] Building ASM object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_aeabi.S.obj
[ 82%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_init_rom.c.obj
[ 83%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_math.c.obj
[ 83%] Building ASM object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_v1_rom_shim.S.obj
[ 83%] Building ASM object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/pico_int64_ops/pico_int64_ops_aeabi.S.obj
[ 83%] Building ASM object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_aeabi.S.obj
[ 83%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_init_rom.c.obj
[ 83%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_math.c.obj
[ 83%] Building ASM object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_v1_rom_shim.S.obj
[ 83%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/pico_malloc/pico_malloc.c.obj
[ 83%] Building ASM object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/pico_mem_ops/mem_ops_aeabi.S.obj
[ 83%] Building ASM object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/crt0.S.obj
[ 84%] Building CXX object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/new_delete.cpp.obj
[ 84%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/binary_info.c.obj
[ 84%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio/stdio.c.obj
[ 84%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio_uart/stdio_uart.c.obj
[ 84%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-extras/src/rp2_common/pico_scanvideo_dpi/scanvideo.c.obj
[ 84%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_dma/dma.c.obj
[ 84%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_pio/pio.c.obj
[ 84%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-extras/src/common/pico_scanvideo/vga_modes.c.obj
[ 84%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/W_/Pico/pico-extras/src/common/pico_util_buffer/buffer.c.obj
[ 84%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/__/render/image.c.obj
[ 85%] Building C object scanvideo/textmode/CMakeFiles/textmode.dir/__/render/spans.c.obj
[ 85%] Linking CXX executable textmode.elf
[ 85%] Built target textmode
Scanning dependencies of target hello_dormant
[ 85%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/hello_dormant.c.obj
[ 85%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdlib/stdlib.c.obj
[ 85%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_gpio/gpio.c.obj
[ 85%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_claim/claim.c.obj
[ 85%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/pico_platform/platform.c.obj
[ 85%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_sync/sync.c.obj
[ 86%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_irq/irq.c.obj
[ 86%] Building ASM object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_irq/irq_handler_chain.S.obj
[ 86%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/common/pico_sync/sem.c.obj
[ 86%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/common/pico_sync/lock_core.c.obj
[ 86%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/common/pico_time/time.c.obj
[ 86%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/common/pico_time/timeout_helper.c.obj
[ 86%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_timer/timer.c.obj
[ 86%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/common/pico_util/datetime.c.obj
[ 86%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/common/pico_util/pheap.c.obj
[ 87%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/common/pico_util/queue.c.obj
[ 87%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/common/pico_sync/mutex.c.obj
[ 87%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/common/pico_sync/critical_section.c.obj
[ 87%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_uart/uart.c.obj
[ 87%] Building ASM object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_divider/divider.S.obj
[ 87%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/pico_runtime/runtime.c.obj
[ 87%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_clocks/clocks.c.obj
[ 87%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_pll/pll.c.obj
[ 87%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_vreg/vreg.c.obj
[ 87%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_watchdog/watchdog.c.obj
[ 88%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_xosc/xosc.c.obj
[ 88%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/pico_printf/printf.c.obj
[ 88%] Building ASM object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/pico_bit_ops/bit_ops_aeabi.S.obj
[ 88%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/pico_bootrom/bootrom.c.obj
[ 88%] Building ASM object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/pico_divider/divider.S.obj
[ 88%] Building ASM object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_aeabi.S.obj
[ 88%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_init_rom.c.obj
[ 88%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_math.c.obj
[ 88%] Building ASM object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_v1_rom_shim.S.obj
[ 88%] Building ASM object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/pico_int64_ops/pico_int64_ops_aeabi.S.obj
[ 89%] Building ASM object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_aeabi.S.obj
[ 89%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_init_rom.c.obj
[ 89%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_math.c.obj
[ 89%] Building ASM object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_v1_rom_shim.S.obj
[ 89%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/pico_malloc/pico_malloc.c.obj
[ 89%] Building ASM object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/pico_mem_ops/mem_ops_aeabi.S.obj
[ 89%] Building ASM object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/crt0.S.obj
[ 89%] Building CXX object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/new_delete.cpp.obj
[ 89%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/binary_info.c.obj
[ 89%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio/stdio.c.obj
[ 90%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio_uart/stdio_uart.c.obj
[ 90%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-extras/src/rp2_common/pico_sleep/sleep.c.obj
[ 90%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-extras/src/rp2_common/hardware_rosc/rosc.c.obj
[ 90%] Building C object sleep/hello_dormant/CMakeFiles/hello_dormant.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_rtc/rtc.c.obj
[ 90%] Linking CXX executable hello_dormant.elf
[ 90%] Built target hello_dormant
Scanning dependencies of target hello_sleep
[ 90%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/hello_sleep.c.obj
[ 90%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdlib/stdlib.c.obj
[ 90%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_gpio/gpio.c.obj
[ 90%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_claim/claim.c.obj
[ 90%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/pico_platform/platform.c.obj
[ 91%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_sync/sync.c.obj
[ 91%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_irq/irq.c.obj
[ 91%] Building ASM object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_irq/irq_handler_chain.S.obj
[ 91%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/common/pico_sync/sem.c.obj
[ 91%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/common/pico_sync/lock_core.c.obj
[ 91%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/common/pico_time/time.c.obj
[ 91%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/common/pico_time/timeout_helper.c.obj
[ 91%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_timer/timer.c.obj
[ 91%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/common/pico_util/datetime.c.obj
[ 92%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/common/pico_util/pheap.c.obj
[ 92%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/common/pico_util/queue.c.obj
[ 92%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/common/pico_sync/mutex.c.obj
[ 92%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/common/pico_sync/critical_section.c.obj
[ 92%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_uart/uart.c.obj
[ 92%] Building ASM object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_divider/divider.S.obj
[ 92%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/pico_runtime/runtime.c.obj
[ 92%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_clocks/clocks.c.obj
[ 92%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_pll/pll.c.obj
[ 92%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_vreg/vreg.c.obj
[ 93%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_watchdog/watchdog.c.obj
[ 93%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_xosc/xosc.c.obj
[ 93%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/pico_printf/printf.c.obj
[ 93%] Building ASM object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/pico_bit_ops/bit_ops_aeabi.S.obj
[ 93%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/pico_bootrom/bootrom.c.obj
[ 93%] Building ASM object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/pico_divider/divider.S.obj
[ 93%] Building ASM object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_aeabi.S.obj
[ 93%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_init_rom.c.obj
[ 93%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_math.c.obj
[ 93%] Building ASM object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_v1_rom_shim.S.obj
[ 94%] Building ASM object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/pico_int64_ops/pico_int64_ops_aeabi.S.obj
[ 94%] Building ASM object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_aeabi.S.obj
[ 94%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_init_rom.c.obj
[ 94%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_math.c.obj
[ 94%] Building ASM object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_v1_rom_shim.S.obj
[ 94%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/pico_malloc/pico_malloc.c.obj
[ 94%] Building ASM object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/pico_mem_ops/mem_ops_aeabi.S.obj
[ 94%] Building ASM object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/crt0.S.obj
[ 94%] Building CXX object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/new_delete.cpp.obj
[ 94%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/binary_info.c.obj
[ 95%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio/stdio.c.obj
[ 95%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio_uart/stdio_uart.c.obj
[ 95%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-extras/src/rp2_common/pico_sleep/sleep.c.obj
[ 95%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-extras/src/rp2_common/hardware_rosc/rosc.c.obj
[ 95%] Building C object sleep/hello_sleep/CMakeFiles/hello_sleep.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_rtc/rtc.c.obj
[ 95%] Linking CXX executable hello_sleep.elf
[ 95%] Built target hello_sleep
[ 95%] Generating uart_tx.pio.h
[ 95%] Built target stdio_pio_uart_tx_pio_h
Scanning dependencies of target stdio_pio
[ 95%] Building C object stdio/pio/CMakeFiles/stdio_pio.dir/stdio_pio.c.obj
[ 95%] Building C object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdlib/stdlib.c.obj
[ 95%] Building C object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_gpio/gpio.c.obj
[ 95%] Building C object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_claim/claim.c.obj
[ 95%] Building C object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/pico_platform/platform.c.obj
[ 95%] Building C object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_sync/sync.c.obj
[ 96%] Building C object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_irq/irq.c.obj
[ 96%] Building ASM object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_irq/irq_handler_chain.S.obj
[ 96%] Building C object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/common/pico_sync/sem.c.obj
[ 96%] Building C object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/common/pico_sync/lock_core.c.obj
[ 96%] Building C object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/common/pico_time/time.c.obj
[ 96%] Building C object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/common/pico_time/timeout_helper.c.obj
[ 96%] Building C object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_timer/timer.c.obj
[ 96%] Building C object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/common/pico_util/datetime.c.obj
[ 96%] Building C object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/common/pico_util/pheap.c.obj
[ 96%] Building C object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/common/pico_util/queue.c.obj
[ 97%] Building C object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/common/pico_sync/mutex.c.obj
[ 97%] Building C object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/common/pico_sync/critical_section.c.obj
[ 97%] Building C object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_uart/uart.c.obj
[ 97%] Building ASM object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_divider/divider.S.obj
[ 97%] Building C object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/pico_runtime/runtime.c.obj
[ 97%] Building C object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_clocks/clocks.c.obj
[ 97%] Building C object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_pll/pll.c.obj
[ 97%] Building C object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_vreg/vreg.c.obj
[ 97%] Building C object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_watchdog/watchdog.c.obj
[ 97%] Building C object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_xosc/xosc.c.obj
[ 98%] Building C object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/pico_printf/printf.c.obj
[ 98%] Building ASM object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/pico_bit_ops/bit_ops_aeabi.S.obj
[ 98%] Building C object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/pico_bootrom/bootrom.c.obj
[ 98%] Building ASM object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/pico_divider/divider.S.obj
[ 98%] Building ASM object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_aeabi.S.obj
[ 98%] Building C object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_init_rom.c.obj
[ 98%] Building C object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_math.c.obj
[ 98%] Building ASM object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/pico_double/double_v1_rom_shim.S.obj
[ 98%] Building ASM object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/pico_int64_ops/pico_int64_ops_aeabi.S.obj
[ 98%] Building ASM object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_aeabi.S.obj
[ 99%] Building C object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_init_rom.c.obj
[ 99%] Building C object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_math.c.obj
[ 99%] Building ASM object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/pico_float/float_v1_rom_shim.S.obj
[ 99%] Building C object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/pico_malloc/pico_malloc.c.obj
[ 99%] Building ASM object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/pico_mem_ops/mem_ops_aeabi.S.obj
[ 99%] Building ASM object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/crt0.S.obj
[ 99%] Building CXX object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/new_delete.cpp.obj
[ 99%] Building C object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/pico_standard_link/binary_info.c.obj
[ 99%] Building C object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio/stdio.c.obj
[100%] Building C object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/pico_stdio_uart/stdio_uart.c.obj
[100%] Building C object stdio/pio/CMakeFiles/stdio_pio.dir/W_/Pico/pico-sdk/src/rp2_common/hardware_pio/pio.c.obj
[100%] Linking CXX executable stdio_pio.elf
[100%] Built target stdio_pio

W:\Pico\pico-playground\build>