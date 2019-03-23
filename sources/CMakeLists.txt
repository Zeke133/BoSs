# ---
# Compile options of targets
# ---
# Targets:
#   boss.elf
# - cmsis
# - nuclearHardwareModules
# - nuclearAbstractModules
# - framework
# - application

# Compile options common for all targets ASM/C/C++
set_property(
    TARGET
        nuclearHardwareModules cmsis nuclearAbstractModules framework application
    PROPERTY
        COMPILE_OPTIONS -march=armv7-m -mcpu=cortex-m3 -mthumb -mlittle-endian
)

# Compile options common for C and C++ targets
if(NOT use_libg)
    set(COMPILE_DEFINES NO_USE_LIBG)
endif()
if(use_qemu_dummies)
    set(COMPILE_DEFINES "${COMPILE_DEFINES};QEMU_DUMMY")
endif()

set_property(
    TARGET
        cmsis nuclearAbstractModules framework application
    APPEND
    PROPERTY
        COMPILE_OPTIONS -Wall
                        -ffreestanding
                        --specs=nano.specs
                        -ffunction-sections
                        -fdata-sections
                        -finline-small-functions
                        -findirect-inlining
)
set_property(
    TARGET
        nuclearAbstractModules framework application
    APPEND
    PROPERTY
        COMPILE_DEFINITIONS ${COMPILE_DEFINES}
)

# Compile options common for C++ targets
if(NOT use_libg)
    set(COMPILE_OPTIONS_FLAGS -fno-exceptions -fno-non-call-exceptions -fno-rtti -fno-use-cxa-atexit)
endif()

set_property(
    TARGET
        nuclearAbstractModules nuclearAbstractModulesTests framework application
    PROPERTY
        COMPILE_FEATURES cxx_std_17
)
set_property(
    TARGET
        nuclearAbstractModules framework application
    APPEND
    PROPERTY
        COMPILE_OPTIONS ${COMPILE_OPTIONS_FLAGS}
)
