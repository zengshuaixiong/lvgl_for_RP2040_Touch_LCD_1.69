cmake_minimum_required(VERSION 3.12)
include(pico_sdk_import.cmake)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

project(Pico_ePaper_Code)
pico_sdk_init()

# Add compilation subdirectory
add_subdirectory(./lib/Config)
add_subdirectory(./lib/LCD)
add_subdirectory(./lib/Fonts)
add_subdirectory(./lib/GUI)
add_subdirectory(./lib/porting)
add_subdirectory(./examples)
add_subdirectory(./lvgl)
add_subdirectory(./lib/out)

# Add header file directory
include_directories(./examples)
include_directories(./lib/Config)
include_directories(./lib/Fonts)
include_directories(./lib/GUI)
include_directories(./lib/LCD)
include_directories(./lib/porting)
include_directories(./lvgl)
include_directories(./lib/out)

# Generate executable file
add_executable(main 
main.c
)

# enable usb output, disable uart output
pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 0)

# create map/bin/hex/uf2 file etc.

target_include_directories(main
    PUBLIC
    ${PROJECT_SOURCE_DIR}/lib/out
)


target_link_libraries(main examples LCD GUI Fonts Config pico_stdlib hardware_spi hardware_i2c hardware_rtc hardware_pwm lvgl porting out)
target_include_directories(lvgl PUBLIC
    ./
)

pico_add_extra_outputs(main)

