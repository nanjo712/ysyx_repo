TOPNAME = top
NXDC_FILES = constr/top.nxdc

VERILATOR_CFLAGS += -MMD --build -cc  \
				-O3 --x-assign fast --x-initial fast --noassert

VSRC = vsrc
CSRC = csrc

VERILOG_FILE = $(wildcard $(VSRC)/*.v)
C_FILE = $(wildcard $(CSRC)/*.c)
CXX_FILE = $(wildcard $(CSRC)/*.cpp)
BIN = ./obj_dir/$(TOPNAME)

SRC_AUTO_BIND = $(abspath ./obj_dir/auto_bind.cpp)
$(SRC_AUTO_BIND): $(NXDC_FILES)
	python3 $(NVBOARD_HOME)/scripts/auto_pin_bind.py $^ $@

include $(NVBOARD_HOME)/scripts/nvboard.mk

INCFLAGS = $(addprefix -I, $(INC_PATH))
CXXFLAGS += $(INCFLAGS) -DTOP_NAME="\"V$(TOPNAME)\""

$(BIN): $(VERILOG_FILE) $(C_FILE) $(CXX_FILE) $(NVBOARD_ARCHIVE) $(SRC_AUTO_BIND)
	verilator $(VERILATOR_CFLAGS) \
		--top-module $(TOPNAME) $^ \
		$(addprefix -CFLAGS , $(CXXFLAGS)) $(addprefix -LDFLAGS , $(LDFLAGS)) \
		--exe -o $(abspath $(BIN))

all:
	@echo "Write this Makefile by your self."

sim: $(BIN)
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	$(BIN)

check_log:
	git log tracer-ysyx
	

include ../Makefile
