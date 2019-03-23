# ---
# BoSs kernel abstraction layer modules
# ---
add_library(nuclearAbstractModules OBJECT)

set(nuclearAbstractModulesPath ${nuclearPath}/modules)
set(nuclearAbstractModulesHeadersPath ${nuclearPath}/headers)

target_include_directories(
        nuclearAbstractModules
    PUBLIC ${BOOST_DIR}
    PUBLIC ${nuclearAbstractModulesHeadersPath}
)

target_sources(
        nuclearAbstractModules
    PRIVATE thread
    PRIVATE scheduler
)

add_subdirectory(tests)
