#include "Vlight.h"
#include "nvboard.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include <cassert>
#include <cstdlib>

void nvboard_bind_all_pins(TOP_NAME *top);

int main(int argc, char *argv[]) {
  TOP_NAME top;
  nvboard_bind_all_pins(&top);
  nvboard_init();

  top.rst = 1;
  for (int i = 0; i < 10; i++) {
    top.clk = 0;
    top.eval();
    top.clk = 1;
    top.eval();
  }

  top.rst = 0;

  while (1) {
    nvboard_update();
    top.clk = 1;
    top.eval();
    top.clk = 0;
    top.eval();
  }

  nvboard_quit();
  return 0;
}