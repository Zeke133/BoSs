# Folder with automation scripts for external tools

Some usefull scripts for building and work on project.
On first step with project call: **./scripts/sh/generateBuildTestSTM32.sh** to generate makefiles and build the project for STM32 real hardware target.

Other scripts:

- **generateBuildTestQemu.sh** - Generate Cmake project. Build. Run unit-tests. For Qemu target.
- **build.sh** - Build already generated Cmake project.
- **run.sh** - Run executable in QEMU.
- **runDebug.sh** - Run executable in QEMU and wait for GDB connection.
- **openOCDConnect.sh** - Connect to MCU using openOCD and ST-link.
- **openOCDFlash.sh** - Flash program to MCU usig openOCD and ST-link.

Scripts must be executed from **${projectRoot}** directory.

Used tools:

- **arm-none-eabi** - GCC based toolchain for project compilation and linkage.
- **GCC** - Compilers collection used for UnitTests building.
- **make** - Buid utility.
- **cmake/ctest** - Build generation system. Building and testing automation.
- **googleTest** - Unit testing framework.
- **OpenOCD** - Real hardware debug and uploading/downloading firmaware onto developement board.
- **stm32flash** - Uploading/downloading firmaware onto developement board. Communication with default stm32 bootloader.
- **GDB** - Debug.

**OpenOCD** and **stm32flash** must be placed in same directory like this project.

    ...     someDir / ${projectRoot} / scripts / bat / ...
                                               / sh / ...
                    / googleTest / ...
                    / stm32flash / ...
                    / OpenOCD / ...

**arm-none-eabi**, **gcc**, **make**, **cmake/ctest** must be added to **PATH**
