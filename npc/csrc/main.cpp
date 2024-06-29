#include "Vtop.h"
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

  while (1) {
    nvboard_update();
    top.eval();
  }

  nvboard_quit();
  return 0;
}