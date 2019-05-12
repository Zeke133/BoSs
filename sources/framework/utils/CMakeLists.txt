# ---
# BoSs framework utils
# ---
# Framework
set(frameworkUtilsModulesPath ${frameworkPath}/utils/modules)
set(frameworkUtilsHeadersPath ${frameworkPath}/utils/headers)

add_library(frameworkUtils OBJECT)

target_include_directories(
        frameworkUtils
    PUBLIC ${frameworkInterfacesPath}
    PUBLIC ${frameworkUtilsHeadersPath}
)

target_sources(
        frameworkUtils
    PRIVATE ${frameworkUtilsModulesPath}/convertation
    PRIVATE ${frameworkUtilsModulesPath}/crc
    PRIVATE ${frameworkUtilsModulesPath}/ostream
)

add_subdirectory(tests)
