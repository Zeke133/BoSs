# ---
# STM Low Level Driver library
# ---
set(stmDriverHeadersPath ${PROJECT_SOURCE_DIR}/libs/STM32F1xx_LL_Driver/inc)
set(stmDriverModulesPath ${PROJECT_SOURCE_DIR}/libs/STM32F1xx_LL_Driver/src)

add_library(stmLL OBJECT)

target_link_libraries(
        stmLL
    PUBLIC cmsis
)

target_include_directories(
        stmLL
    # PUBLIC ${PROJECT_SOURCE_DIR}/libs/cmsis/ARMCM3
    # PUBLIC ${PROJECT_SOURCE_DIR}/libs/cmsis/STM32F1xx
    PUBLIC ${stmDriverHeadersPath}
)

target_sources(
        stmLL
    PRIVATE ${stmDriverModulesPath}/stm32f1xx_ll_rcc
    PRIVATE ${stmDriverModulesPath}/stm32f1xx_ll_dma
    PRIVATE ${stmDriverModulesPath}/stm32f1xx_ll_usart
)

target_compile_definitions(
        stmLL
    PUBLIC USE_FULL_LL_DRIVER
    # PUBLIC STM32F103xB  ##!< STM32F103C8, STM32F103R8, STM32F103T8, STM32F103V8, STM32F103CB, STM32F103RB, STM32F103TB and STM32F103VB
)
