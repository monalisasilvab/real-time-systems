cmake_minimum_required(VERSION 3.13)

# Indica onde está o SDK
set(PICO_SDK_PATH "/home/monalisa/pico-sdk")
// set(PICO_BOARD pico 2)

# Importa o SDK
include(${PICO_SDK_PATH}/pico_sdk_init.cmake)

project(blink_led C CXX ASM)
pico_sdk_init()

add_executable(blink_led
    blink_led_pico.c
)

target_link_libraries(blink_led pico_stdlib)

pico_add_extra_outputs(blink_led)
