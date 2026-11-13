#include "process.h"
#include <stdint.h>

static struct context os_context;
static struct process *current_process;

static void process_first_entry(void) {
  current_process->entry();
  process_exit(0);
}

void process_create(struct process *process, process_entry_t entry) {
  uintptr_t top = (uintptr_t)(process->stack + STACK_SIZE);

  process->entry = entry;
  process->state = READY;
  process->exit_status = 0;

  // top &= ~(uintptr_t)0xF;
  process->context.sp = top;
  process->context.ra = (uintptr_t)process_first_entry;
}

void process_run(struct process *process) {
  // if (process->state == FINISHED)
  //   return;

  current_process = process;
  process->state = RUNNING;
  context_switch(&os_context, &process->context);
}

void process_yield(void) {
  current_process->state = READY;
  context_switch(&current_process->context, &os_context);
}

void process_exit(int status) {
  current_process->exit_status = status;
  current_process->state = FINISHED;
  context_switch(&current_process->context, &os_context);
}
