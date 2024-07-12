cmake_minimum_required(VERSION 3.29)

project(npc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

set(CONSTR ${CMAKE_CURRENT_SOURCE_DIR}/constr)
set(ARGS_VERILATOR 
    -Wall 
    --trace
    -j 8
)
set(MUX_TEMPLATE
    MuxKeyInternal.v 
    MuxKey.v
    MuxKeyWithDefault.v
)

function(VerilateTarget target top_module )
    auto_bind(${target} ${CONSTR}/${top_module}.nxdc ${CMAKE_CURRENT_BINARY_DIR}/${top_module}_auto_bind.cpp)
    add_executable(${target} 
        csrc/${top_module}_sim.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/${top_module}_auto_bind.cpp
    )
    target_compile_definitions(${target} PRIVATE TOP_NAME=V${top_module})
    target_link_libraries(${target} PRIVATE nvboard)
    foreach (src IN LISTS ARGN)
        list(APPEND SOURCES vsrc/${src})
    endforeach()
    verilate(${target}
        SOURCES ${SOURCES}
        TOP_MODULE ${top_module}
        PREFIX V${top_module}
        VERILATOR_ARGS ${ARGS_VERILATOR}
    )
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E env NEMU_HOME=$ENV{NEMU_HOME} make commit
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endfunction()

VerilateTarget(Mux Mux4Way2Bit Mux4Way2Bit.v Mux4Way.v ${MUX_TEMPLATE})

VerilateTarget(Code CodeTop CodeTop.v Encoder8to3.v Seg7Decoder.v)

VerilateTarget(ALU ALU ALU.v adder.v)

VerilateTarget(shift shift shift.v dff.v)

VerilateTarget(BarrelShift BarrelShift BarrelShift.v ${MUX_TEMPLATE})

VerilateTarget(LSFR LSFR LSFR.v shift.v dff.v)

VerilateTarget(KeyboardDisplay 
    KeyboardDisplay 
    KeyboardDisplay.v 
    Seg7Decoder.v 
    PS2Keyboard.v 
    ScanCodeToASCII.v 
    ${MUX_TEMPLATE}
)