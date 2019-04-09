# ---
# BoSs kernel hardware dependent modules
# ---
add_library(nuclearHardwareModules OBJECT)

target_sources(
        nuclearHardwareModules
    PRIVATE startUp.s
            contextSwitch.s
            pendSvHandler.s
            svCallHandler.s
            svCallApi.s
)

if(NOT use_libg)
    target_sources(
            nuclearHardwareModules
        PRIVATE customLibgcc.s
    )
endif()
