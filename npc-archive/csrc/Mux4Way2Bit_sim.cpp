#include <cassert>
#include <cstdlib>
#include <memory>
#include <print>

#include "VMux4Way2Bit.h"
#include "nvboard.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

void nvboard_bind_all_pins(TOP_NAME *top);

int sim_in_NVBoard()
{
    TOP_NAME top;
    nvboard_bind_all_pins(&top);
    nvboard_init();

    while (1)
    {
        top.eval();
        nvboard_update();
        // std::this_thread::sleep_for(std::c)
    }
    nvboard_quit();
    return 0;
}

int sim_in_rawVerilator(int argc, char *argv[])
{
    auto contextp = std::make_unique<VerilatedContext>();
    contextp->commandArgs(argc, argv);
    auto m_VMux4Way2Bit = std::make_unique<VMux4Way2Bit>(contextp.get());
    m_VMux4Way2Bit->x0 = 0x0;
    m_VMux4Way2Bit->x1 = 0x1;
    m_VMux4Way2Bit->x2 = 0x2;
    m_VMux4Way2Bit->x3 = 0x3;
    while (!contextp->gotFinish())
    {
        m_VMux4Way2Bit->y = rand() % 4;
        m_VMux4Way2Bit->eval();
        std::print("y = {}, f = {}\n", (int)m_VMux4Way2Bit->y,
                   (int)m_VMux4Way2Bit->f);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    // sim_in_rawVerilator(argc, argv);
    sim_in_NVBoard();
    return 0;
}