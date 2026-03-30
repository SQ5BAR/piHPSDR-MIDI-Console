cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(rbc_midi_pico C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()
add_executable(rbc_midi_pico
    main.c
    usb_descriptors.c
)
target_include_directories(rbc_midi_pico PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(rbc_midi_pico pico_stdlib tinyusb_device tinyusb_board hardware_gpio hardware_timer)
pico_enable_stdio_usb(rbc_midi_pico 0)
pico_enable_stdio_uart(rbc_midi_pico 1)
pico_add_extra_outputs(rbc_midi_pico)
