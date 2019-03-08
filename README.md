# BoSs

## Bare operation System solution

Project of OS core (scheduler) for ARM Cortex-M3 microcontrollers with emulation on Qemu.

**To Do:**

- [x] Clean project from garbage
- [x] Create Linker script and normal startup with Vectors table to run on Qemu
- [x] Simple Threads primitive to Run
- [x] Write context switch procedure
- [x] Check if threads run
- [x] Create PendingSV handler on ASM - problem that C++ handler procedure saves some registers on stack each run
- [x] **Unit tests**
- [x] two configs for QEMU and for REAL HW
- [x] reafact added files. create few target configurations for qemuDebug and realHW
- [x] target for CMSIS, and **chose correct startup** procedures
- [ ] target for libC or not? use standart libC and exceptions?
- [x] **real hardware start**
- [x] ~~qemu DWT not implemented?~~
- [x] Context switch procedures profiling. Use DWT
- [ ] #pragma once use and delete uint32_t where is not needed
- [ ] lambda use
- [ ] Change to PSP and **unprivileged mode** check if works
- [ ] Make user treads to run in Unprivileged mode and one thread for kernel tasks. Run user main() from _init. before this go to PSP unprivileged mode.
- [ ] **Scheduler must also set nextThread pointer**
- [ ] Thread SLEEP()
- [ ] Mutex and semafore
- [ ] Self test with semafore and poling systimer
- [ ] Stack Overflow detection by "endmarks"
- [ ] try to delete dependencies to LL lib .cpp and other trash
- [ ] delete from Framework headers #includes of STM LL and CMSIS. Make them pure interface headers. This will require some atribute types redefinition.
