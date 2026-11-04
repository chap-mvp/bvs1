CROSS ?= riscv64-unknown-elf-
CC := $(CROSS)gcc
OBJDUMP := $(CROSS)objdump
SIZE := $(CROSS)size
QEMU ?= qemu-system-riscv64

BUILD_DIR := build
ELF := $(BUILD_DIR)/hello-bvs1.elf
MAP := $(BUILD_DIR)/hello-bvs1.map
DISASM := $(BUILD_DIR)/hello-bvs1.disasm
KERNEL_LIB := lib/libkernel.a

# Exercise 04 provides the OS as a precompiled RV64 library: process table,
# trap_handler() and os_run(). trap.S stays visible because the trap path
# and the return path are course material. Student C files remain ordinary
# sources at the repo root.
C_SOURCES := $(sort $(wildcard *.c))
ASM_SOURCES := start.S trap.S
HEADERS := $(sort $(wildcard *.h))
SOURCES := $(ASM_SOURCES) $(C_SOURCES)

CFLAGS := \
	-march=rv64imac_zicsr \
	-mabi=lp64 \
	-mcmodel=medany \
	-msmall-data-limit=0 \
	-ffreestanding \
	-fno-builtin \
	-fno-stack-protector \
	-fno-pie \
	-fno-asynchronous-unwind-tables \
	-Wall \
	-Wextra \
	-Werror \
	-O2

LDFLAGS := \
	-nostdlib \
	-nostartfiles \
	-no-pie \
	-T linker.ld \
	-Wl,--build-id=none \
	-Wl,-Map,$(MAP)

.PHONY: all check run inspect disasm clean

all: check $(ELF)
	@$(SIZE) $(ELF)

check:
	@echo "Checking BVS1 environment..."
	@command -v $(CC) >/dev/null 2>&1 || { echo "Missing tool: $(CC)"; exit 1; }
	@command -v $(OBJDUMP) >/dev/null 2>&1 || { echo "Missing tool: $(OBJDUMP)"; exit 1; }
	@command -v $(SIZE) >/dev/null 2>&1 || { echo "Missing tool: $(SIZE)"; exit 1; }
	@command -v $(QEMU) >/dev/null 2>&1 || { echo "Missing tool: $(QEMU)"; exit 1; }
	@command -v file >/dev/null 2>&1 || { echo "Missing tool: file"; exit 1; }
	@test -f $(KERNEL_LIB) || { echo "Missing provided kernel library: $(KERNEL_LIB)"; exit 1; }
	@$(QEMU) --machine help | grep -q "virt" || { echo "QEMU does not provide the RISC-V virt machine."; exit 1; }
	@echo "Environment OK."

$(BUILD_DIR):
	mkdir -p $@

$(ELF): $(SOURCES) $(HEADERS) $(KERNEL_LIB) linker.ld | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SOURCES) $(KERNEL_LIB) -o $@


run: all
	@echo "Starting bare RV64 machine. Exit QEMU with Ctrl-a, then x."
	$(QEMU) \
		-machine virt \
		-m 128M \
		-smp 1 \
		-nographic \
		-bios none \
		-kernel $(ELF)

inspect: all
	@echo "ELF:"
	@file $(ELF)
	@echo
	@echo "Sections:"
	@$(OBJDUMP) -h $(ELF)

disasm: all
	$(OBJDUMP) -d -S $(ELF) > $(DISASM)
	@echo "Wrote $(DISASM)"

clean:
	rm -rf $(BUILD_DIR)
