#include "Vtop.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include <cassert>
#include <cstdlib>
#include <iostream>

int main(int argc, char *argv[]) {
  auto contextp = std::make_unique<VerilatedContext>();
  contextp->commandArgs(argc, argv);
  auto top = std::make_unique<Vtop>(contextp.get());

  int sim_time = 100;
  while (contextp->time() < sim_time && !contextp->gotFinish()) {
    top->a = rand() & 1;
    top->b = rand() & 1;
    top->eval();
    std::cout << "a: " << (int)top->a << " b: " << (int)top->b
              << " f: " << (int)top->f << std::endl;
    contextp->timeInc(1);
    assert(top->f == (top->a ^ top->b));
  }
  return 0;
}