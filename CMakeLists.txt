cmake_minimum_required(VERSION 3.29)

project(ysyx_on_cmake LANGUAGES CXX)

find_package(verilator HINTS $ENV{VERILATOR_ROOT})

set(ENV{NEMU_HOME} ${CMAKE_CURRENT_SOURCE_DIR}/nemu)

add_subdirectory(nvboard)
add_subdirectory(npc)





