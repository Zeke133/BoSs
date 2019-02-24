cmake_minimum_required(VERSION 3.9)

project(
    BOSS            # Bare Operating System Solution
    VERSION         0.1.2
    DESCRIPTION     "ARM Cortex-M3 minimal preemptive multitasking OS"
    HOMEPAGE_URL    "https://github.com/Zeke133/BoSs"
    LANGUAGES
        ASM
        CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)   # generate file for IntelliSense
# set(CMAKE_VERBOSE_MAKEFILE ON)        # print cmake traces

# ------------------------------------------------------------
# Unit tests
enable_testing()
# Path to GoogleTest Framework - change for your location!
set(GOOGLE_TEST_DIR ${PROJECT_SOURCE_DIR}/../googleTest)

# ------------------------------------------------------------
# Kernel
add_subdirectory(sources/nuclear)

# ------------------------------------------------------------
# Link firmware
add_executable(boss.elf)

target_link_libraries(
        boss.elf
    PRIVATE nuclearHardwareModules
    PRIVATE nuclearAbstractModules
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
set(LINKER_MEMORY_SCRIPT    ${PROJECT_SOURCE_DIR}/sources/nuclear/linker/linkerMemoryQemu.ld)
set(LINKER_SECTIONS_SCRIPT  ${PROJECT_SOURCE_DIR}/sources/nuclear/linker/linkerSections.ld)
# set(PLATFORM_LINK_FLAGS     "-mcpu=cortex-m3 -mthumb")
# set(EMBEDDED_LINK_FLAGS     "--specs=nano.specs --specs=nosys.specs -nostartfiles")
# -nostdlib same as passing -nodefaultlibs and -nostartfiles
set_property(
    TARGET boss.elf
    PROPERTY
        LINK_FLAGS
            # CMake syntax let flags be defined only in one string
            "${USER_LINK_FLAGS} -T${LINKER_MEMORY_SCRIPT} -T${LINKER_SECTIONS_SCRIPT}"
)

# ------------------------------------------------------------
# Post build tasks
# set(HEX_FILE_OUT ${PROJECT_SOURCE_DIR}/bin/boss.hex)
set(BIN_FILE_OUT ${PROJECT_SOURCE_DIR}/bin/boss.bin)
set(SYM_FILE_OUT ${PROJECT_SOURCE_DIR}/bin/boss.sym)
add_custom_command(
    TARGET boss.elf POST_BUILD
    # COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:boss.elf> ${HEX_FILE_OUT}
    COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:boss.elf> ${BIN_FILE_OUT}
    COMMAND ${CMAKE_NM} -n $<TARGET_FILE:boss.elf> > ${SYM_FILE_OUT}
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:boss.elf>
    COMMENT "Building ${HEX_FILE_OUT} \nBuilding ${BIN_FILE_OUT}"
)
