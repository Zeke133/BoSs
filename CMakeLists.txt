cmake_minimum_required (VERSION 3.9)

# Project name
project(
    OS_SCHEDULER
    VERSION 0.1.1
    DESCRIPTION     "Scheduler for ARM Cortex M3 embadded OS"
    HOMEPAGE_URL    "https://github.com/Zeke133/osStudy"
    LANGUAGES
        ASM     # startup procedure
        C       # user project
        CXX     # user project
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Startup procedure
add_library(startup OBJECT)
target_sources(
        startup
    PRIVATE startup.s
)

# ------------------------------------------------------------
# Link firmware
add_executable(scheduler.elf)
target_link_libraries(scheduler.elf PRIVATE startup)

# target_include_directories(
#         scheduler.elf
#     PRIVATE ${CODE_STYLE_DIR}
# )

target_sources(
        scheduler.elf
    PRIVATE test
)

# ---
# Setup compilation properties
# ---
set_target_properties(
        scheduler.elf
    PROPERTIES
        COMPILE_FEATURES
            cxx_std_17
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
)
set_property(
    TARGET
        scheduler.elf
    APPEND
    PROPERTY
        COMPILE_OPTIONS
            # C++ only options
            -fno-exceptions         # in libs
            -fno-non-call-exceptions
            -fno-rtti
            -fno-use-cxa-atexit     # Teardown code (including global destructors) can be omitted
            # Common flags
            -mcpu=arm926ej-s
            # -mcpu=cortex-m3
            # -mthumb
            -g                      # debug mode
            # -Os                     # optimisation
            -Wall
            # Bare metal specific flags
            --specs=nano.specs      # To use newlib-nano
            -ffunction-sections     # which split functions and data into their own ELF sections.
            -fdata-sections         # This allows the linker to eliminate
                                    # additional unused code when passed --gc-sections
            -ffreestanding          # indicates that your program exists in an environment
                                    # where standard library facilities may be absent
                                    # and where your program may not begin at main()
            -finline-small-functions    # which inlines functions whenever the compiler thinks
                                        # the function body contains fewer instructions
                                        # than the code needed to call it.
            -findirect-inlining     # which runs additional inlining passes. For example,
                                    # if main() calls a() and a() calls b(), this compiler
                                    # could fold the body of a() into main(), then notice
                                    # that b() is also a good candidate for inlining and fold it in.
                                    # Serious improvements can be gained through such second-order effects.
)

# ---
# Link properties
set(MAP_FILE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin/scheduler.map)
set(LINKER_SCRIPT_PATH ${PROJECT_SOURCE_DIR}/test.ld)
set(PLATFORM_LINK_FLAGS     "-mcpu=arm926ej-s")
set(EMBEDDED_LINK_FLAGS     "--specs=nano.specs --specs=nosys.specs -nostartfiles")
set(USER_LINK_FLAG          "-Os -g -Wl,-Map=${MAP_FILE_OUTPUT_PATH} -Wl,-cref -Wl,--gc-sections")
set_property(
    TARGET scheduler.elf
    PROPERTY
        LINK_FLAGS
            # Stupid syntax let flags be defined only in one string
            "${PLATFORM_LINK_FLAGS}  ${EMBEDDED_LINK_FLAGS} ${USER_LINK_FLAG} -Wl,-T${LINKER_SCRIPT_PATH}"
)

# ------------------------------------------------------------
# Post build tasks
set(HEX_FILE_OUT ${PROJECT_SOURCE_DIR}/bin/scheduler.hex)
set(BIN_FILE_OUT ${PROJECT_SOURCE_DIR}/bin/scheduler.bin)
set(SYM_FILE_OUT ${PROJECT_SOURCE_DIR}/bin/scheduler.sym)
add_custom_command(
    TARGET scheduler.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:scheduler.elf> ${HEX_FILE_OUT}
    COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:scheduler.elf> ${BIN_FILE_OUT}
    COMMAND ${CMAKE_NM} -n $<TARGET_FILE:scheduler.elf> > ${SYM_FILE_OUT}
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:scheduler.elf>
    COMMENT "Building ${HEX_FILE_OUT} \nBuilding ${BIN_FILE_OUT}"
)
