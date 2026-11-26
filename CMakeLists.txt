cmake_minimum_required(VERSION 3.15)
project(MOS6502 LANGUAGES CXX)

add_executable(mos6502 main.cpp)

add_custom_target(run
    COMMAND mos6502
    DEPENDS mos6502
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    USES_TERMINAL
    VERBATIM
)
