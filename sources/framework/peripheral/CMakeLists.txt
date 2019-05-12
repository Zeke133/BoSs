# ---
# BoSs framework peripheral drivers
# ---
# Framework
set(frameworkPeripheralModulesPath ${frameworkPath}/peripheral/modules)
set(frameworkPeripheralHeadersPath ${frameworkPath}/peripheral/headers)
set(frameworkUtilsHeadersPath ${frameworkPath}/utils/headers)

add_library(frameworkPeripheral OBJECT)

target_link_libraries(
        frameworkPeripheral
    PUBLIC cmsis
    PUBLIC stmLL
    PUBLIC nuclearAbstractModules
)

target_include_directories(
        frameworkPeripheral
    PUBLIC ${stmDriverHeadersPath}
    PUBLIC ${frameworkInterfacesPath}
    PUBLIC ${frameworkUtilsHeadersPath}
    PUBLIC ${frameworkPeripheralHeadersPath}
)

target_sources(
        frameworkPeripheral
    PRIVATE ${frameworkPeripheralModulesPath}/gpio
    PRIVATE ${frameworkPeripheralModulesPath}/led
    PRIVATE ${frameworkPeripheralModulesPath}/sysClock
    PRIVATE ${frameworkPeripheralModulesPath}/delay
    PRIVATE ${frameworkPeripheralModulesPath}/dma
    PRIVATE ${frameworkPeripheralModulesPath}/uart
    PRIVATE ${frameworkPeripheralModulesPath}/i2c
    PRIVATE ${frameworkPeripheralModulesPath}/rtc
)
