#ifndef BVS1_TRAP_H
#define BVS1_TRAP_H

#include <stdint.h>

/*
 * Provided scaffold.
 *
 * The complete saved state of an execution that trapped. Unlike the CP1
 * cooperative context (ra, sp, s0-s11), a trap can happen between any two
 * instructions, so every integer register must be preserved, together with
 * the address where the execution continues.
 *
 * regs[i] holds register xi. regs[0] is unused because x0 is always zero.
 * The target is rv64imac (no F/D), so there is no floating-point state.
 *
 * pc: the saved continuation address for this execution. Trap entry stores
 * the trap pc from mepc here: for an interrupt the instruction not yet
 * executed, for an exception the causing instruction. After an ecall the
 * OS adds 4 to obtain the continuation address. The return path writes
 * pc back into mepc before mret.
 *
 * The layout is fixed: trap.S uses the byte offsets 8 * i and 256.
 */
struct trap_frame {
    uintptr_t regs[32];
    uintptr_t pc;
};

/* ABI names for the registers the C side needs. */
enum {
    REG_RA = 1,
    REG_SP = 2,
    REG_A0 = 10,
    REG_A1 = 11,
    REG_A7 = 17,
};

/*
 * Provided in trap.S.
 *
 * trap_entry (the trap path): installed in mtvec. Saves the process's
 * registers and the trap pc into the frame that mscratch points to,
 * switches to the OS stack and calls trap_handler(). Then returns into the
 * frame that trap_handler() returns.
 *
 * os_start (the return path): restores the given frame and executes mret.
 * It never returns. os_run(), called from main(), uses it for the first
 * start of a process: the first start is a return.
 */
void trap_entry(void);
void os_start(struct trap_frame *first) __attribute__((noreturn));

/*
 * Implemented by the OS in C.
 *
 * Receives the frame of the process that trapped and returns the frame to
 * return into: the same one (resume) or another one (schedule).
 */
struct trap_frame *trap_handler(struct trap_frame *trapped);

#endif
