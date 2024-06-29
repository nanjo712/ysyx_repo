VSRC = vsrc
CSRC = csrc

VERILOG_FILE = $(wildcard $(VSRC)/*.v)
C_FILE = $(wildcard $(CSRC)/*.c)
CXX_FILE = $(wildcard $(CSRC)/*.cpp)

all:
	@echo "Write this Makefile by your self."

sim:
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	verilator --cc --exe --build -j 0 -Wall $(CXX_FILE) $(C_FILE) $(VERILOG_FILE) --trace
	./obj_dir/Vtop
	gtkwave top.vcd

check_log:
	git log tracer-ysyx
	

include ../Makefile
