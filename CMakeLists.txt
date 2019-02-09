cmake_minimum_required (VERSION 3.9)

# Project name
project(
    OS_SCHEDULER
    VERSION 0.1.1
    DESCRIPTION     "Scheduler for ARM Cortex M3 embadded OS"
    HOMEPAGE_URL    "https://github.com/Zeke133/osStudy"
    LANGUAGES
        ASM     # startup procedure
        C
        CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_VERBOSE_MAKEFILE ON)

# ------------------------------------------------------------
# Unit tests
# enable_testing()
# add_subdirectory(unitTests)
# add_test(
#     NAME utils
#     COMMAND unitTests
# )

# ------------------------------------------------------------
# Startup procedure
add_library(startup OBJECT)
target_sources(
        startup
    PRIVATE platformInitARMCortexM3/startup.s
)
set_property(
    TARGET
        startup
    APPEND
    PROPERTY
        COMPILE_OPTIONS
            -mthumb
            -mlittle-endian
            -march=armv7-m
            -mcpu=cortex-m3
            -g
)

# ------------------------------------------------------------
# Modules
add_subdirectory(sources)
set_property(
    TARGET
        modules
    APPEND
    PROPERTY
        COMPILE_OPTIONS
            -mthumb
            -mlittle-endian
            -march=armv7-m
            -mcpu=cortex-m3
            -g                      # debug mode
            -Wall
            -Os                     # optimisation size
            -ffreestanding
            --specs=nano.specs      # To use newlib-nano
            -ffunction-sections     # which split functions and data into their own ELF sections.
            -fdata-sections         # This allows the linker to eliminate
                                    # additional unused code when passed --gc-sections
            -fno-exceptions         # in libs
            -fno-non-call-exceptions
            -fno-rtti
            -fno-use-cxa-atexit     # Teardown code (including global destructors) can be omitted
)
set_property(
    TARGET
        modules
    PROPERTY
        COMPILE_FEATURES
            cxx_std_17
)

# ------------------------------------------------------------
# Link firmware
add_executable(os.elf)
target_link_libraries(os.elf PRIVATE startup modules)

set_target_properties(
        os.elf
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
)

# ---
# Link properties
set(MAP_FILE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/os.map)
set(LINKER_SCRIPT_PATH ${PROJECT_SOURCE_DIR}/platformInitARMCortexM3/linkerScript.ld)
set(USER_LINK_FLAG          "-Map=${MAP_FILE_OUTPUT_PATH} -cref --gc-sections")
# set(USER_LINK_FLAG          "-Os -g -Map=${MAP_FILE_OUTPUT_PATH} -cref --gc-sections")
set(PLATFORM_LINK_FLAGS     "")
# set(PLATFORM_LINK_FLAGS     "-mcpu=cortex-m3 -mthumb")
set(EMBEDDED_LINK_FLAGS     "")
# set(EMBEDDED_LINK_FLAGS     "--specs=nano.specs --specs=nosys.specs -nostartfiles")
# -nostdlib same as passing -nodefaultlibs and -nostartfiles
set_property(
    TARGET os.elf
    PROPERTY
        LINK_FLAGS
            "${PLATFORM_LINK_FLAGS}  ${EMBEDDED_LINK_FLAGS} ${USER_LINK_FLAG} -T${LINKER_SCRIPT_PATH}"
            # CMake syntax let flags be defined only in one string
)

# ------------------------------------------------------------
# Post build tasks
# set(HEX_FILE_OUT ${PROJECT_SOURCE_DIR}/bin/os.hex)
set(BIN_FILE_OUT ${PROJECT_SOURCE_DIR}/bin/os.bin)
set(SYM_FILE_OUT ${PROJECT_SOURCE_DIR}/bin/os.sym)
add_custom_command(
    TARGET os.elf POST_BUILD
    # COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:os.elf> ${HEX_FILE_OUT}
    COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:os.elf> ${BIN_FILE_OUT}
    COMMAND ${CMAKE_NM} -n $<TARGET_FILE:os.elf> > ${SYM_FILE_OUT}
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:os.elf>
    COMMENT "Building ${HEX_FILE_OUT} \nBuilding ${BIN_FILE_OUT}"
)
