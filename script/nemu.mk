BUILD_DIR  := ./build
NEMU_EXEC = $(BUILD_DIR)/nemu-cpp/app/nemu-cpp

CPU_TEST = ${BUILD_DIR}/abstract-machine/app/cpu-test/
CPU_TEST_BINS = $(wildcard $(CPU_TEST)*.bin)
IMG_FILE ?= ${CPU_TEST}dummy.bin 

build_nemu:
	$(call git_commit, "build NEMU")
	cmake -S . -B $(BUILD_DIR)
	cmake --build $(BUILD_DIR)

run_nemu: build_nemu
	$(call git_commit, "run NEMU")
	$(NEMU_EXEC) $(IMG_FILE)

gdb_nemu: build_nemu
	$(call git_commit, "gdb NEMU")
	gdbserver localhost:1234 ${NEMU_EXEC} ${IMG_FILE}

%.bin: build_nemu
	${NEMU_EXEC} -b $@

cpu_test_nemu: $(CPU_TEST_BINS)
	${call git_commit, cpu_test NEMU""}
	@echo "All tests passed! $(notdir $^)"

.PHONY: run_nemu gdb_nemu cpu_test_nemu