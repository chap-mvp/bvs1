/*
 * Provided scaffold: the only assembly boundary of the CP2 system.
 *
 * Invariant: while a process runs, mscratch holds the address of that
 * process's struct trap_frame (see trap.h for the layout).
 *
 * Two paths:
 *   trap   trap_entry: save the process's registers and the trap pc into
 *          its frame, switch to the OS stack, call trap_handler();
 *   return os_start: restore a frame and execute mret. Trap entry falls
 *          through into it after the handler; os_run() uses it for the
 *          first start of a process.
 *
 * The CPU's trap rule, applied before trap_entry runs:
 *   mepc   <- trap pc (the causing instruction for an exception,
 *             the next instruction for an interrupt)
 *   mcause <- reason (interrupt bit + code)
 *   mstatus.MPIE <- mstatus.MIE, mstatus.MIE <- 0, mstatus.MPP <- old mode
 *   pc     <- mtvec (= trap_entry)
 *
 * The CPU's return rule, applied by mret:
 *   pc <- mepc, mstatus.MIE <- mstatus.MPIE, mode <- mstatus.MPP
 */

#define FRAME_PC 256

    .section .text
    .balign 4
    .globl trap_entry
    .type trap_entry, @function
trap_entry:
    /* Swap t6 (x31) with mscratch: t6 = frame, mscratch = the process's t6. */
    csrrw t6, mscratch, t6

    sd x1,    8(t6)
    sd x2,   16(t6)
    sd x3,   24(t6)
    sd x4,   32(t6)
    sd x5,   40(t6)
    sd x6,   48(t6)
    sd x7,   56(t6)
    sd x8,   64(t6)
    sd x9,   72(t6)
    sd x10,  80(t6)
    sd x11,  88(t6)
    sd x12,  96(t6)
    sd x13, 104(t6)
    sd x14, 112(t6)
    sd x15, 120(t6)
    sd x16, 128(t6)
    sd x17, 136(t6)
    sd x18, 144(t6)
    sd x19, 152(t6)
    sd x20, 160(t6)
    sd x21, 168(t6)
    sd x22, 176(t6)
    sd x23, 184(t6)
    sd x24, 192(t6)
    sd x25, 200(t6)
    sd x26, 208(t6)
    sd x27, 216(t6)
    sd x28, 224(t6)
    sd x29, 232(t6)
    sd x30, 240(t6)

    /* The process's t6 waits in mscratch; save it as x31. */
    csrr t5, mscratch
    sd t5,  248(t6)

    /* The trap pc, copied by the CPU into mepc. */
    csrr t5, mepc
    sd t5, FRAME_PC(t6)

    /* Switch to the OS stack. The CPU did not do this for us. */
    la sp, _stack_top

    /* next = trap_handler(trapped) */
    mv a0, t6
    call trap_handler

    /* fall through: return into the frame in a0 */

    .globl os_start
    .type os_start, @function
os_start:
    /* a0 = frame to restore. It becomes the current frame. */
    csrw mscratch, a0

    ld t5, FRAME_PC(a0)
    csrw mepc, t5

    /* Return to machine mode with interrupts enabled after mret:
       MPP = 3 (M), MPIE = 1. A trap sets MPP = M again, but the first
       mret from os_start needs it set explicitly. */
    li t5, (3 << 11) | (1 << 7)
    csrs mstatus, t5

    ld x1,    8(a0)
    ld x2,   16(a0)
    ld x3,   24(a0)
    ld x4,   32(a0)
    ld x5,   40(a0)
    ld x6,   48(a0)
    ld x7,   56(a0)
    ld x8,   64(a0)
    ld x9,   72(a0)
    ld x11,  88(a0)
    ld x12,  96(a0)
    ld x13, 104(a0)
    ld x14, 112(a0)
    ld x15, 120(a0)
    ld x16, 128(a0)
    ld x17, 136(a0)
    ld x18, 144(a0)
    ld x19, 152(a0)
    ld x20, 160(a0)
    ld x21, 168(a0)
    ld x22, 176(a0)
    ld x23, 184(a0)
    ld x24, 192(a0)
    ld x25, 200(a0)
    ld x26, 208(a0)
    ld x27, 216(a0)
    ld x28, 224(a0)
    ld x29, 232(a0)
    ld x30, 240(a0)
    ld x31, 248(a0)
    ld x10,  80(a0)     /* a0 last: it held the frame address */

    /* pc <- mepc, MIE <- MPIE, mode <- MPP */
    mret

    .size trap_entry, os_start - trap_entry
    .size os_start, . - os_start
