STUID = ysyx_24070002
STUNAME = 叶家聪 

NEMU_EXEC  := ./build/nemu-cpp/app/nemu-cpp
BUILD_DIR  := ./build

CPU_TEST = /home/woshiren/abstract-machine/build/app/cpu-test/
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
	 
# DO NOT modify the following code!!!

TRACER = tracer-ysyx
GITFLAGS = -q --author='$(TRACER) <tracer@ysyx.org>' --no-verify --allow-empty

YSYX_HOME = $(NEMU_HOME)/..
WORK_BRANCH = $(shell git rev-parse --abbrev-ref HEAD)
WORK_INDEX = $(YSYX_HOME)/.git/index.$(WORK_BRANCH)
TRACER_BRANCH = $(TRACER)

LOCK_DIR = $(YSYX_HOME)/.git/

# prototype: git_soft_checkout(branch)
define git_soft_checkout
	git checkout --detach -q && git reset --soft $(1) -q -- && git checkout $(1) -q --
endef

# prototype: git_commit(msg)
define git_commit
	-@flock $(LOCK_DIR) $(MAKE) -C $(YSYX_HOME) .git_commit MSG='$(1)'
	-@sync $(LOCK_DIR)
endef

.git_commit:
	-@while (test -e .git/index.lock); do sleep 0.1; done;               `# wait for other git instances`
	-@git branch $(TRACER_BRANCH) -q 2>/dev/null || true                 `# create tracer branch if not existent`
	-@cp -a .git/index $(WORK_INDEX)                                     `# backup git index`
	-@$(call git_soft_checkout, $(TRACER_BRANCH))                        `# switch to tracer branch`
	-@git add . -A --ignore-errors                                       `# add files to commit`
	-@(echo "> $(MSG)" && echo $(STUID) $(STUNAME) && uname -a && uptime `# generate commit msg`) \
	                | git commit -F - $(GITFLAGS)                        `# commit changes in tracer branch`
	-@$(call git_soft_checkout, $(WORK_BRANCH))                          `# switch to work branch`
	-@mv $(WORK_INDEX) .git/index                                        `# restore git index`

.clean_index:
	rm -f $(WORK_INDEX)

_default:
	@echo "Please run 'make' under subprojects."

.PHONY: .git_commit .clean_index _default
