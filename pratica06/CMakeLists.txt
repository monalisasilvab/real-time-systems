cmake_minimum_required(VERSION 3.13)

# Pull in SDK (must be before project)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Pull in FreeRTOS
include($ENV{FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(app C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_executable(pratica06 main.c)

target_include_directories(pratica06 PRIVATE ${CMAKE_CURRENT_LIST_DIR})
 
# pull in common dependencies
target_link_libraries(pratica06 pico_stdlib hardware_adc FreeRTOS-Kernel FreeRTOS-Kernel-Heap4)

# enable usb output, disable uart output
pico_enable_stdio_usb(pratica06 1)
pico_enable_stdio_uart(pratica06 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(pratica06)
