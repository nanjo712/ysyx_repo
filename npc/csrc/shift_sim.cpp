#include "Vshift.h"
#include "nvboard.h"
#include "verilated.h"

void nvboard_bind_all_pins(Vshift* top);

int main()
{
    Vshift top;
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