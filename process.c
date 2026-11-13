#include "process.h"
#include <stdint.h>

static struct context os_context; /* Saved OS State */
static struct process *current_process;

/* The first time a new process runs, context_switch() "returns" into this
 * function, because process_create() set ra to its address. There is no
 * earlier caller frame on the process stack, so this function starts the
 * process body and, if the body returns normally, exits the process with
 * status 0.
 */
static void process_first_entry(void) {
  current_process->entry();
  process_exit(0);
}

/* - entry function is stores (process->entry = entry) [Done]
 * - process owns its stack storage (process->stack, STACK_SIZE) [Done]
 * - initial sp is set to the high end of the stack (process->context.sp = top)
 * [Done]
 * - initial CPI context exists: sp = top = process_first_entry, so the first
 * context_switch() into this process enters process_first_entry [Done]
 * - process is READY (process->state = READY) [Done]
 * - exit_status initialized to 0 (process->exit_status = 0) [Done]
 *
 * ra points at a valid entry path directly, so context_switch can always
 * pretend its running a yielded process.
 * */
void process_create(struct process *process, process_entry_t entry) {
  uintptr_t top = (uintptr_t)(process->stack + STACK_SIZE);

  process->entry = entry;
  process->state = READY;
  process->exit_status = 0;

  process->context.sp = top;
  process->context.ra = (uintptr_t)process_first_entry;
}

/* - Refuse to run a FINISHED process (if-check) [Done]
 * - record which process is current_process (current_process = process) [Done]
 * - mark it RUNNING (process->state = RUNNING) [Done]
 * - save the OS context and restore the process context (context_switch) [Done]
 *
 * context_switch() restores saved ra/sp for execution from process_yield on
 * resume
 * */
void process_run(struct process *process) {
  if (process->state == FINISHED)
    return;

  current_process = process;
  process->state = RUNNING;
  context_switch(&os_context, &process->context);
}

/* - mark current_process as READY (current_process->state = READY) [Done]
 * - save the process CPU context & restore OS CPU context (context_switch)
 * [Done]
 *
 * context_switch() returns here after the saved ra, so execution can continue
 * */
void process_yield(void) {
  current_process->state = READY;
  context_switch(&current_process->context, &os_context);
}

/* - record the exit status (current_process->exit_status) [Done]
 * - mark current_process as FINISHED (current_process->state = FINISHED) [Done]
 * - return contorl to the OS through context_switch (context_switch) [Done]
 * */
void process_exit(int status) {
  current_process->exit_status = status;
  current_process->state = FINISHED;
  context_switch(&current_process->context, &os_context);
}
