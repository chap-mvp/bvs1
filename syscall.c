#include "process.h"
#include "syscall.h"

/*
 * Provided scaffold: the process side of the system-call interface.
 *
 * A system call is an ordinary instruction, ecall, that the hardware turns
 * into a trap. By convention, a7 selects the service and a0/a1 carry the
 * arguments; the OS returns a result in a0.
 */

static long syscall2(long number, long arg0, long arg1)
{
    register long a0 __asm__("a0") = arg0;
    register long a1 __asm__("a1") = arg1;
    register long a7 __asm__("a7") = number;

    __asm__ volatile ("ecall"
                      : "+r"(a0)
                      : "r"(a1), "r"(a7)
                      : "memory");

    return a0;
}

void process_yield(void)
{
    syscall2(SYS_YIELD, 0, 0);
}

void process_exit(int status)
{
    syscall2(SYS_EXIT, status, 0);

    for (;;) {
        /* The OS never resumes a FINISHED process. */
    }
}

long process_write(const char *buf, unsigned long len)
{
    return syscall2(SYS_WRITE, (long)buf, (long)len);
}
