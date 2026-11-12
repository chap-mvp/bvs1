#include "process.h"
#include <stdint.h>

// process_create()
// Prepare a process without running it.
// After creation:
// • the entry function is known;
// • the process owns its stack storage;
// • the initial stack pointer is correctly aligned near the high-address end of
// that stack; • an initial CPU context exists that can enter the process for
// the first time; • the process is READY. A process that has never run must be
// made to look, to context_switch(), like an execution that can be restored.
//
//
// process_run()
// process_yield()
// process_exit(status)
//
