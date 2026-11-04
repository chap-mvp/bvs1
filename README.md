# BVS1 – Betriebssysteme: Course Code

This is the student code repository for **BVS1 – Betriebssysteme** at TH Köln, ICCT.

## Exercise 04 — One Trap Path

This branch moves the Exercise 2 application onto the trap path from Lecture 5. Control now enters the OS through the hardware: `process_yield()` and `process_exit()` execute `ecall`, and the OS runs inside `trap_handler()`.

Provided:

- `lib/libkernel.a`: a precompiled RV64 OS with the process table, `process_create()`, `trap_handler()` and `os_run()`;
- `trap.h` and `trap.S`: the visible trap path (`trap_entry`) and return path (`os_start()`);
- `syscall.h` and `syscall.c`: the system-call numbers and the `ecall` stubs behind `process_yield()` and `process_exit()`;
- `riscv.h`: CSR access and `mcause` codes;
- `uart.h` and `uart.c`: UART output shared by the OS and your application;
- startup, linker, build, and QEMU support.

Not provided:

- `process.h`: you bring your Exercise 2 header and change it in three places;
- the C source of the OS.

`context.h`, `context.S` and `lib/libprocess.a` from Exercise 2 are gone. The cooperative `context_switch()` stays on the Exercise 2 and Checkpoint 1 branches.

Bring your Exercise 2 files onto a local work branch:

```bash
git fetch origin
git switch exercise/04-trap-path
git switch -c exercise-04
git checkout exercise-02 -- process.h main.c
```

Do not bring a `process.c` from your Checkpoint 1 work: the Makefile compiles every root-level C file, and a second process implementation clashes with `lib/libkernel.a`.

For Checkpoint 2, the provided kernel library disappears and you implement the OS side yourself around the same provided `trap.S`, extended by timer interrupts and a scheduler.

## Course Workflow

Clone this repository **once** inside the Ubuntu 26.04 course VM:

```bash
git clone https://github.com/stavroskousidis/teaching-bvs1-code.git
cd teaching-bvs1-code
```

Recommended Exercise 04 workflow:

```bash
git fetch origin
git switch exercise/04-trap-path
git switch -c exercise-04
```

Keep your local semester repository and commit meaningful working states. Do not push student work to the course repository.

Local Git history is **not a backup**. Keep regular copies or a Git bundle outside the course VM, especially before checkpoints.

## Build and Run

```bash
make check
make
make run
```

Exit QEMU with **Ctrl-a**, release the keys, then press **x**.

Useful inspection targets:

```bash
make inspect
make disasm
```

Generated files are written to `build/`.

## Environment

The supported development environment is the course-provided Ubuntu 26.04 VM named `bvs1`. Complete Lab 0 before using this repository.

Inside that VM:

- Git and Make manage the local code and build;
- the RISC-V cross-toolchain produces RV64 code;
- QEMU emulates the RV64 `virt` teaching machine.
