#ifndef BVS1_SYSCALL_H
#define BVS1_SYSCALL_H

/*
 * Provided scaffold: system-call numbers.
 *
 * A process requests an OS service by placing the number in a7 and the
 * arguments in a0, a1 and executing ecall. The OS returns a result in a0.
 */

enum {
    SYS_EXIT  = 0,
    SYS_YIELD = 1,
    SYS_WRITE = 2,      /* Lecture 8 extension, required from CP3 on */
};

long process_write(const char *buf, unsigned long len);

#endif
