# ---
# BoSs user application modules
# ---
set(applicationPath ${PROJECT_SOURCE_DIR}/sources/application)
set(applicationModulesPath ${applicationPath}/modules)
set(applicationHeadersPath ${applicationPath}/headers)

add_library(application OBJECT)

target_link_libraries(
        application
    PUBLIC frameworkUtils
    PUBLIC frameworkPeripheral
)

target_include_directories(
        application
    PUBLIC ${applicationHeadersPath}
)

target_sources(
        application
    PRIVATE ${applicationModulesPath}/application
)
