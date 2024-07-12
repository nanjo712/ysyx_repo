#include <print>

#include "VKeyboardDisplay.h"
#include "nvboard.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

void nvboard_bind_all_pins(VKeyboardDisplay* top);

int main()
{
    // Verilated::traceEverOn(true);
    VKeyboardDisplay top;
    // auto vcd = std::make_unique<VerilatedVcdC>();
    nvboard_bind_all_pins(&top);
    nvboard_init();
    // top.trace(vcd.get(), 0);
    // vcd->open("trace.vcd");

    top.rst = 1;
    for (int i = 0; i < 100; i++)
    {
        top.clk = 0;
        top.eval();
        // vcd->dump(i * 2);
        top.clk = 1;
        top.eval();
        // vcd->dump(i * 2 + 1);
        nvboard_update();
    }
    top.rst = 0;

    std::print("Reset complete\n");
    int count = 100;
    while (1)
    {
        top.clk = 0;
        top.eval();
        // vcd->dump(count * 2);
        top.clk = 1;
        top.eval();
        // vcd->dump(count * 2 + 1);
        nvboard_update();
        count++;
    }
    nvboard_quit();
    return 0;
}