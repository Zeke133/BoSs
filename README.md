# BOSS

## Bare Operating System Solution

Project of OS core (scheduler) for ARM Cortex M3 microcontrollers with emulation on Qemu.

**To Do:**

- [x] Clean project from garbage
- [x] Create Linker script and normal startup with Vectors table to run on Qemu
- [x] Simple Threads primitive to Run
- [x] Write context switch procedure
- [x] Check if threads run
- [x] Create PendingSV handler on ASM - problem that C++ handler procedure saves some registers on stack each run
- [ ] Change to PSP and **unprivileged mode** check if works
- [ ] Make user treads to run in Unprivileged mode and one thread for kernel tasks. Run user main() from _init. before this go to PSP unprivileged mode.
- [ ] **Scheduler must also set nextThread pointer**
- [ ] Context switch procedures profiling
- [x] Unit tests
- [ ] Thread SLEEP()
- [ ] Mutex and semafore
- [ ] Self test with semafore and poling systimer
- [ ] Stack Overflow detection by "endmarks"
