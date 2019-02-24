# ---
# BoSs nuclear
# ---
set(nuclearPath ${PROJECT_SOURCE_DIR}/sources/nuclear)

add_subdirectory(asm)       # nuclearHardwareModules
add_subdirectory(modules)   # nuclearAbstractModules

set_property(               # common part of COMPILE_OPTIONS
    TARGET
        nuclearHardwareModules
        nuclearAbstractModules
    APPEND
    PROPERTY
        COMPILE_OPTIONS
            -mthumb
            -mlittle-endian
            -march=armv7-m
            -mcpu=cortex-m3
            -g
)
