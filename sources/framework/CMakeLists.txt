# ---
# BoSs peripheral framework
# ---
# External Libs
set(stmDriverHeadersPath ${PROJECT_SOURCE_DIR}/libs/STM32F1xx_LL_Driver/inc)
# Framework
set(frameworkPath ${PROJECT_SOURCE_DIR}/sources/framework)
set(frameworkModulesPath ${frameworkPath}/modules)
set(frameworkHeadersPath ${frameworkPath}/headers)

add_library(framework OBJECT)

target_link_libraries(
        framework
    PUBLIC cmsis
    PUBLIC nuclearHardwareModules
    PUBLIC nuclearAbstractModules
)

target_include_directories(
        framework
    PUBLIC ${stmDriverHeadersPath}
    PUBLIC ${frameworkHeadersPath}
)

target_sources(
        framework
    PRIVATE ${frameworkModulesPath}/gpio
    PRIVATE ${frameworkModulesPath}/led
    PRIVATE ${frameworkModulesPath}/sysClock
)
