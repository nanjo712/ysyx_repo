#include "Vtop.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include <cassert>
#include <cstdlib>

int main(int argc, char *argv[]) {
  auto contextp = std::make_unique<VerilatedContext>();
  contextp->commandArgs(argc, argv);
  auto top = std::make_unique<Vtop>(contextp.get());

  Verilated::traceEverOn(true);
  auto tfp = std::make_unique<VerilatedVcdC>();
  top->trace(tfp.get(), 99);
  tfp->open("top.vcd");

  int sim_time = 100;

  while (contextp->time() < sim_time && !contextp->gotFinish()) {
    contextp->timeInc(1);
    int a = rand() & 1;
    int b = rand() & 1;
    top->a = a;
    top->b = b;
    top->eval();
    printf("a=%d, b=%d, f=%d\n", a, b, top->f);
    assert(top->f == (a ^ b));
    tfp->dump(contextp->time());
  }
  tfp->close();
  return 0;
}