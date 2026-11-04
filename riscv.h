#ifndef BVS1_RISCV_H
#define BVS1_RISCV_H

#include <stdint.h>

/* Provided scaffold: control and status registers, CLINT timer. */

#define csr_read(csr) ({ uintptr_t v_; \
    __asm__ volatile ("csrr %0, " #csr : "=r"(v_)); v_; })

#define csr_write(csr, val) \
    __asm__ volatile ("csrw " #csr ", %0" :: "r"((uintptr_t)(val)))

#define csr_set(csr, bits) \
    __asm__ volatile ("csrs " #csr ", %0" :: "r"((uintptr_t)(bits)))

/* mcause */
#define MCAUSE_INTERRUPT       (1UL << 63)
#define MCAUSE_CODE(c)         ((c) & ~MCAUSE_INTERRUPT)
#define IRQ_MACHINE_TIMER      7
#define EXC_ILLEGAL_INSTRUCTION 2
#define EXC_ECALL_FROM_M       11

/* mie */
#define MIE_MTIE               (1UL << 7)

/* QEMU virt CLINT: mtime runs at 10 MHz. */
#define CLINT_MTIMECMP0 ((volatile uint64_t *)0x02004000UL)
#define CLINT_MTIME     ((volatile uint64_t *)0x0200bff8UL)
#define TIMER_HZ        10000000UL

static inline uint64_t timer_now(void)
{
    return *CLINT_MTIME;
}

/* Request the next timer interrupt `ticks` mtime ticks from now. */
static inline void timer_arm(uint64_t ticks)
{
    *CLINT_MTIMECMP0 = *CLINT_MTIME + ticks;
}

#endif
