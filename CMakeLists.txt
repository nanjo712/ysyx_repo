cmake_minimum_required(VERSION 3.29)

project(ysyx_on_cmake LANGUAGES CXX)

set(STUID "ysyx_24070002")
set(STUNAME "woshiren")
set(TRACER_BRANCH "tracer-ysyx")
set(WORK_BRANCH "main")

find_package(verilator HINTS $ENV{VERILATOR_ROOT})

set(ENV{NEMU_HOME} ${CMAKE_CURRENT_SOURCE_DIR}/nemu)

add_subdirectory(nvboard)
# add_subdirectory(npc)
add_subdirectory(nemu-cpp)








