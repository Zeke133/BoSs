# BoSs

## Bare operating System solution

Project of OS core for ARM Cortex-M3 microcontrollers with emulation on Qemu.

Implements preemptive multitasking scheduller.
Also provides lightweight object oriented hardware abstraction framework.

Aimed for quick and easy development of efficient applications on resource restricted MCUs.

Currently supported:

- STM32F103 series

## Project structure

- **bin** - binary output files
- **build** - cmake build directory; contains *compile_commands.json* used for IntelliSense support
- **docs** - usefull documentation and books
- **libs** - 3rd party libraries
- **scripts** - workflow automation scripts
- **sources** - source code

## Project variants

### Standart Cpp libraries support

There are two variants to copmile the project possible and handled with Cmake CACHE variable *use_libg*.

1. **Maximal lightweight project** - Set *use_libg OFF*. Doesn't use any *libc* or *libg*. So size of application scratch is **about 2kB only**. Link with *-nostdlib* flag. Compile with *-fno-exceptions -fno-non-call-exceptions -fno-rtti -fno-use-cxa-atexit* flags. *.ARM.extab* & *.ARM.exidx* sections have to be discarded, so special linker script *linkerSectionsNoExc.ld* is used.
2. **C++ 17 and STL full support** - Set *use_libg ON*. Use *libg_nano* with semihosting turned OFF. Add modern C++ features like exceptions, RTTI, std::function and so on. Minimal size will grow to **about 10kB**. Compile with no additional flags. *.ARM.extab* & *.ARM.exidx* sections have to be included to binary, so *linkerSections.ld* script is used.

### Destination target

There are two targets to copmile the project possible and handled with Cmake *CMAKE_BUILD_TYPE* and *use_qemu_dummies* variables.

1. **QEMU** - Set *CMAKE_BUILD_TYPE Debug*. Option *use_qemu_dummies* handles usage of special code for QEMU and turns *ON* for Debug target automaticaly. Otherwise has to be turned *ON* in Cmake cache manually.
2. **STM32 real hardware** - Set *CMAKE_BUILD_TYPE* to any other type except *Debug*. Option *use_qemu_dummies* has to be turned *OFF*.

## Used tools

- *arm-none-eabi* - GCC based toolchain for project compilation and linkage.
- *GCC* - Compilers collection used for UnitTests building.
- *make* - Buid utility.
- *cmake/ctest* - Build generation system. Building and testing automation.
- *googleTest* - Unit testing framework.
- *OpenOCD* - Real hardware debug and uploading/downloading firmaware onto developement board.
- *stm32flash* - Uploading/downloading firmaware onto developement board. Communication with default stm32 bootloader.
- *GDB* - Debug.

## Last builds size

- STM32 bare metal without std libs: Flash - 2024 bytes, RAM - 8+20 bytes
- STM32 bare metal with std libg_nano: Flash - 14280 bytes, RAM - 124+88 bytes

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
- [x] look like wrong thread SP in PendSV- problem was in std::function
- [x] Refactoring of PendSV - integrate pause and ctxt save to switchThread()
- [x] write unit tests of scheduler
- [x] **update time on delete from TimersQueue**
- [ ] try to **simplify sleepSysCall**
- [x] **rename svCallMock and proxy** classes
- [x] SLEEPING threads queue
- [ ] create vscode cmake build configuration
- [ ] update doks
- [ ] embed 3rd party libs using cmake
- [ ] Context switch optimisation for 1 thread case
- [ ] Thread SLEEP()
- [ ] Divide to namespaces
- [ ] Mutex and semafore
- [ ] cppCheck static analisis
- [ ] Self test with semafore and poling systimer
- [ ] Make user treads to run in Unprivileged mode and one thread for kernel tasks. Run user main() from _init. before this go to PSP unprivileged mode.
- [ ] Stack Overflow detection by "endmarks"
- [ ] ~~try to delete dependencies to LL lib .cpp and other trash~~
- [ ] ~~delete from Framework headers #includes of STM LL and CMSIS. Make them pure interface headers. This will require some atribute types redefinition.~~
