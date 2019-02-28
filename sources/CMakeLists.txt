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
set_property(
    TARGET
        cmsis nuclearAbstractModules framework application
    APPEND
    PROPERTY
        COMPILE_OPTIONS -Wall -ffreestanding --specs=nano.specs -ffunction-sections -fdata-sections
)

# Compile options common for C++ targets
set_property(
    TARGET
        nuclearAbstractModules framework application
    PROPERTY
        COMPILE_FEATURES cxx_std_17
)
set_property(
    TARGET
        nuclearAbstractModules framework application
    APPEND
    PROPERTY
        COMPILE_OPTIONS -fno-exceptions -fno-non-call-exceptions -fno-rtti -fno-use-cxa-atexit
)
