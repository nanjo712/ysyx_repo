#include <print>

#include "VCodeTop.h"
#include "nvboard.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

void nvboard_bind_all_pins(TOP_NAME *top);

int main()
{
    TOP_NAME top;
    nvboard_bind_all_pins(&top);
    nvboard_init();
    while (1)
    {
        top.eval();
        // std::print("out: {}\n", (int)top.out);
        nvboard_update();
    }
    return 0;
}