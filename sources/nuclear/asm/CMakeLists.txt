# ---
# BoSs kernel hardware dependent modules
# ---
add_library(nuclearHardwareModules OBJECT)

target_sources(
        nuclearHardwareModules
    PRIVATE startUp.s
    PRIVATE contextSwitch.s
    PRIVATE pendSvHandler.s
)
