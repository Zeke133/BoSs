cmake_minimum_required(VERSION 3.9)
# ------------------------------------------------------------
project(
    BOSS            # Bare Operating System Solution
    VERSION         0.1.2
    DESCRIPTION     "ARM Cortex-M3 minimal preemptive multitasking OS"
    HOMEPAGE_URL    "https://github.com/Zeke133/BoSs"
    LANGUAGES
        ASM
        C
        CXX
)
# ------------------------------------------------------------
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)   # generate file for IntelliSense
# set(CMAKE_VERBOSE_MAKEFILE ON)        # print cmake traces
# ------------------------------------------------------------
# Unit tests
enable_testing()
# Path to GoogleTest Framework - change for your location! Find doesn't work great with GTest.
set(GOOGLE_TEST_DIR ${PROJECT_SOURCE_DIR}/../googleTest)
# ------------------------------------------------------------
set(CMAKE_ASM_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
# ---
set(CMAKE_ASM_FLAGS_RELEASE "")
set(CMAKE_C_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
# ---
set(CMAKE_ASM_FLAGS_MINSIZEREL "")
set(CMAKE_C_FLAGS_MINSIZEREL "-Os")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os")
# ---
set(CMAKE_ASM_FLAGS_RELWITHDEBINFO "-g")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-g -Os")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -Os")
# ------------------------------------------------------------
add_subdirectory(libs/cmsis)
add_subdirectory(sources/nuclear)
add_subdirectory(sources/framework)
add_subdirectory(sources/application)
add_subdirectory(sources)   # compile options
# ------------------------------------------------------------
# Link final firmware binary
add_executable(boss.elf)
target_link_libraries(
        boss.elf
    PRIVATE cmsis
    PRIVATE nuclearHardwareModules
    PRIVATE nuclearAbstractModules
    PRIVATE framework
    PRIVATE application
)
set_target_properties(
        boss.elf
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
)
# ---
# Link properties
set(MAP_FILE_OUTPUT_PATH    ${PROJECT_SOURCE_DIR}/bin/boss.map)
# -cref                     Include Cross Reference Table to .map file
# --gc-sections             Throw out unused sections from output binary
# -Map=fileName             Generate .map file
set(USER_LINK_FLAGS         "-Map=${MAP_FILE_OUTPUT_PATH} -cref --gc-sections")
set(LINKER_MEMORY_QEMU      ${PROJECT_SOURCE_DIR}/sources/nuclear/linker/linkerMemoryQemu.ld)
set(LINKER_MEMORY_STM32     ${PROJECT_SOURCE_DIR}/sources/nuclear/linker/linkerMemoryStm32f103c8t6.ld)
set(LINKER_SECTIONS_SCRIPT  ${PROJECT_SOURCE_DIR}/sources/nuclear/linker/linkerSections.ld)
# set(PLATFORM_LINK_FLAGS     "-mcpu=cortex-m3 -mthumb")
# set(EMBEDDED_LINK_FLAGS     "--specs=nano.specs --specs=nosys.specs -nostartfiles")
# -nostdlib same as passing -nodefaultlibs and -nostartfiles
set_target_properties(
        boss.elf
    PROPERTIES
        LINK_FLAGS "${USER_LINK_FLAGS}"
        LINK_FLAGS_DEBUG "-T${LINKER_MEMORY_QEMU} -T${LINKER_SECTIONS_SCRIPT}"
        LINK_FLAGS_RELEASE "-T${LINKER_MEMORY_STM32} -T${LINKER_SECTIONS_SCRIPT}"
        LINK_FLAGS_RELWITHDEBINFO "-T${LINKER_MEMORY_STM32} -T${LINKER_SECTIONS_SCRIPT}"
        LINK_FLAGS_MINSIZEREL "-T${LINKER_MEMORY_QEMU} -T${LINKER_SECTIONS_SCRIPT}"
)
# ------------------------------------------------------------
# Post build tasks
set(HEX_FILE_OUT ${PROJECT_SOURCE_DIR}/bin/boss.hex)
set(BIN_FILE_OUT ${PROJECT_SOURCE_DIR}/bin/boss.bin)
set(SYM_FILE_OUT ${PROJECT_SOURCE_DIR}/bin/boss.sym)
add_custom_command(
    TARGET boss.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:boss.elf> ${HEX_FILE_OUT}
    COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:boss.elf> ${BIN_FILE_OUT}
    COMMAND ${CMAKE_NM} -n $<TARGET_FILE:boss.elf> > ${SYM_FILE_OUT}
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:boss.elf>
    COMMENT "Building ${HEX_FILE_OUT} \nBuilding ${BIN_FILE_OUT}"
)
