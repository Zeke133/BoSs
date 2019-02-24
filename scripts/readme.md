# Folder with automation scripts for external tools

Some usefull scripts for building and work on project.
On first step with project call: "./scripts/sh/genBuild.bat" to generate makefiles and build the project.

Scripts must be executed from **${projectRoot}** directory.

Used tools:

- **arm-none-eabi** GCC based toolchain for project compilation and linkage.
- **GCC** Compilers collection used for UnitTests building.
- **make** Buid utility.
- **cmake/ctest** Build generation system. Building and testing automation.
- **googleTest** Unit testing framework.
- **OpenOCD** Real hardware debug and uploading/downloading firmaware onto developement board.
- **stm32flash** Communication with default stm32 bootloader.

**OpenOCD** and **stm32flash** must be placed in same directory like this project.

    ...     someDir / ${projectRoot} / scripts / bat / ...
                                               / sh / ...
                                               / ~~googleTest~~ / ...
                                               / stm32flash / ...
                                               / OpenOCD / ...

**arm-none-eabi**,**gcc**,**make**,**cmake/ctest** must be added to **PATH**
