# ---
# ARM CMSIS library
# ---
add_library(cmsis OBJECT)

set(cmsisPath ${PROJECT_SOURCE_DIR}/libs/cmsis)

target_include_directories(
        cmsis
    PUBLIC ${cmsisPath}/ARMCM3
    PUBLIC ${cmsisPath}/STM32F1xx
)

target_sources(
        cmsis
    PRIVATE ${cmsisPath}/STM32F1xx/system_stm32f1xx
)

target_compile_definitions(
        cmsis
    PUBLIC STM32F103xB  ##!< STM32F103C8, STM32F103R8, STM32F103T8, STM32F103V8, STM32F103CB, STM32F103RB, STM32F103TB and STM32F103VB
)
