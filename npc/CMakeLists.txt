cmake_minimum_required(VERSION 3.29)

project(npc LANGUAGES CXX)

set(MODULE_TOP light)
set(CONSTR ${CMAKE_CURRENT_SOURCE_DIR}/constr)
set(BIND_FILE ${CMAKE_CURRENT_BINARY_DIR}/auto_bind.cpp)

add_definitions(-DTOP_NAME=V${MODULE_TOP})

# add CPP file here
set(CXX_FILE
    csrc/light_sim.cpp
)

# add Verilog file here
set(VERILOG_FILE
    vsrc/light.v
)

# generate a bind_file
auto_bind(${CONSTR}/light.nxdc ${CMAKE_CURRENT_BINARY_DIR}/auto_bind.cpp)

# create a verilator target
set(ARGS_VERILATOR 
    -Wall 
    --trace
)
add_executable(npc ${CXX_FILE} ${BIND_FILE})
target_link_libraries(npc PRIVATE nvboard)
verilate(npc
    SOURCES ${VERILOG_FILE}
    TOP_MODULE ${MODULE_TOP}
    VERILATOR_ARGS ${ARGS_VERILATOR}
)

add_custom_command(TARGET npc POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E env NEMU_HOME=$ENV{NEMU_HOME} make commit
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
