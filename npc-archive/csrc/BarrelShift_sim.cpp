#include <print>

#include "VBarrelShift.h"
#include "nvboard.h"
#include "verilated.h"

void nvboard_bind_all_pins(VBarrelShift* top);

void sim_in_rawVerilator()
{
    auto contextp = std::make_unique<VerilatedContext>();
    contextp->commandArgs(0, (char**)nullptr);
    auto top = std::make_unique<VBarrelShift>(contextp.get());

    top->din = 0b10101010;
    top->dir = 0;
    top->arith = 0;
    while (!contextp->gotFinish())
    {
        top->shift = rand() % 8;
        top->eval();
        std::print("in: {}, shift: {}, out: {}\n", (int)top->din,
                   (int)top->shift, (int)top->dout);
    }
}

int main()
{
    // sim_in_rawVerilator();
    VBarrelShift top;
    nvboard_bind_all_pins(&top);
    nvboard_init();
    while (1)
    {
        top.eval();
        nvboard_update();
    }
    nvboard_quit();
    return 0;
}