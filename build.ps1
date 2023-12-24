#arm-none-eabi-gcc main.c startup.c -T linker_script.ld -o blink.elf -mcpu=cortex-m4 -mthumb -nostdlib

rd -R cmake-build-debug
cmake -G "MinGW Makefiles" -B ./cmake-build-debug -S . -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE:PATH="../arm-none-eabi-gcc.cmake" -DCMAKE_CXX_COMPILER=$(where arm-none-eabi-g++) -DCMAKE_C_COMPILER=$(where arm-none-eabi-gcc)
cd  cmake-build-debug
make
