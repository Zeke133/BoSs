# BoSs

## Bare operation System solution

Project of OS core for ARM Cortex-M3 microcontrollers with emulation on Qemu.
Implements preemptive multitasking scheduller.

## Compilation tips

Two options to copmile the project are possible and handled with Cmake CACHE variable *use_libg*.

1. **Maximal lightweight project** doesn't use any *libc* or *libg*. So size of application scratch is **about 2kB only**. Link with *-nostdlib* flag. Compile with *-fno-exceptions -fno-non-call-exceptions -fno-rtti -fno-use-cxa-atexit* flags. *.ARM.extab* & *.ARM.exidx* sections have to be commented in *linkerSections.ld*.
2. **C++ 17 and STL full support** use *libg_nano* with semihosting turned OFF. Add modern C++ features like exceptions, RTTI, std::function and so on. Minimal size will grow to **about 10kB**. Compile with no additional flags. *.ARM.extab* & *.ARM.exidx* sections have to be uncommented in *linkerSections.ld*.

## To Do

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
- [x] **real hardware start**
- [x] ~~qemu DWT not implemented?~~
- [x] Context switch procedures profiling. Use DWT
- [x] lambda use
- [x] **Scheduler must also set nextThread pointer**
- [x] target for libC or not? use standart libC and exceptions? **write manual**
- [x] #pragma once use and delete uint32_t where is not needed
- [ ] SLEEPING threads queue
- [ ] Change to PSP and **unprivileged mode** check if works
- [ ] Make user treads to run in Unprivileged mode and one thread for kernel tasks. Run user main() from _init. before this go to PSP unprivileged mode.
- [ ] Thread SLEEP()
- [ ] Mutex and semafore
- [ ] Self test with semafore and poling systimer
- [ ] Stack Overflow detection by "endmarks"
- [ ] try to delete dependencies to LL lib .cpp and other trash
- [ ] delete from Framework headers #includes of STM LL and CMSIS. Make them pure interface headers. This will require some atribute types redefinition.
