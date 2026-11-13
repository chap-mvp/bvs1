#ifndef BVS1_CONTEXT_H
#define BVS1_CONTEXT_H

#include <stdint.h>

/*
 * Saved execution state at the cooperative context-switch boundary.
 *
 * ra and sp are named explicitly because Lecture 03 reasons about them.
 * s0-s11 are the RISC-V callee-saved registers. The provided assembly
 * boundary preserves them so C code may use an ordinary function call to
 * switch between two contexts.
 */
struct context {
  uintptr_t ra;
  uintptr_t sp;
  uintptr_t s[12];
};

/*
 * Save the current context in *current_context, restore *next_context, and
 * continue at the restored return address.
 *
 * This low-level mechanism is provided course infrastructure. The process
 * abstraction and lifecycle built on top of it remain student work.
 */
void context_switch(struct context *current_context,
                    const struct context *next_context);

#endif
