# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Configure PICO_SDK_PATH
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Import SDK
include(pico_sdk_import.cmake)
project(reflexo C CXX ASM)
pico_sdk_init()

# Add executable first
add_executable(reflexo reflexo.c inc/ssd1306.c)

# Then generate PIO header
pico_generate_pio_header(reflexo ${CMAKE_CURRENT_LIST_DIR}/ws2818b.pio)

# Set program properties
pico_set_program_name(reflexo "reflexo")
pico_set_program_version(reflexo "0.1")

# Configure stdio
pico_enable_stdio_uart(reflexo 0)
pico_enable_stdio_usb(reflexo 1)

# Add include directories
target_include_directories(reflexo PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

# Link libraries
target_link_libraries(reflexo
    pico_stdlib
    hardware_pio
    hardware_clocks
    hardware_i2c
)

# Add extra outputs
pico_add_extra_outputs(reflexo)

