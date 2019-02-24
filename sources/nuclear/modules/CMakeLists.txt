# ---
# BoSs kernel abstraction layer modules
# ---
add_library(nuclearAbstractModules OBJECT)

set(nuclearAbstractModulesPath ${nuclearPath}/modules)
set(nuclearAbstractModulesHeadersPath ${nuclearPath}/headers)

target_include_directories(
        nuclearAbstractModules
    PRIVATE ${nuclearAbstractModulesHeadersPath}
)

target_sources(
        nuclearAbstractModules
    PRIVATE core
    PRIVATE thread
    PRIVATE scheduler
)

set_property(
    TARGET
        nuclearAbstractModules
    APPEND
    PROPERTY
        COMPILE_OPTIONS
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
        nuclearAbstractModules
    PROPERTY
        COMPILE_FEATURES
            cxx_std_17
)

add_subdirectory(tests)
