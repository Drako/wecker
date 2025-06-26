cmake_minimum_required(VERSION 3.27)
cmake_policy(VERSION 3.27)

set(PICO_CXX_ENABLE_RTTI 1)
include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)

project(wecker VERSION 0.1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

pico_sdk_init()

add_subdirectory(contrib)

add_executable(wecker
        src/main.cxx
        src/on_board_led.cxx src/on_board_led.hxx
        src/beeper.cxx src/beeper.hxx
        src/color.hxx
        src/rgb_led.cxx src/rgb_led.hxx
        src/singleton.hxx
        src/lock_guard.hxx
        src/joystick.cxx src/joystick.hxx
        src/vec2.hxx
        src/led.cxx src/led.hxx
        src/button.cxx src/button.hxx
        # custom implementation of _getentropy. might be required by some things like thread-safe statics
        src/entropy.c
        # these are copied from the pico_breadboard_kit
        lv_port_disp.c lv_port_disp.h
        lv_port_indev.c lv_port_indev.h
)

pico_generate_pio_header(wecker "${CMAKE_CURRENT_LIST_DIR}/ws2812.pio" OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}")
pico_enable_stdio_uart(wecker 1)
pico_enable_stdio_usb(wecker 1)

target_compile_definitions(wecker PRIVATE LV_LVGL_H_INCLUDE_SIMPLE=1 LV_GT911_SWAPXY=1 LV_GT911_INVERT_X=1)
target_include_directories(wecker PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(wecker PRIVATE
        pico_stdlib pico_stdio pico_multicore pico_sync pico_cyw43_arch_none pico_time pico_rand pico_standard_link
        hardware_gpio hardware_pwm hardware_rtc hardware_pio hardware_adc hardware_irq hardware_i2c hardware_spi hardware_dma
        lvgl
)
pico_add_extra_outputs(wecker)
