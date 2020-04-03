# ---
# BoSs kernel abstraction layer Unit tests
# ---

add_test(
    NAME    BoSsKernelUnitTests
    COMMAND nuclearAbstractModulesTests
)

# Set usual GCC compiler, because other targets use toolchain configuration.
set(CMAKE_CXX_COMPILER  g++)

add_executable(nuclearAbstractModulesTests)

target_link_libraries(
        nuclearAbstractModulesTests
    PRIVATE gtest_main
)

target_include_directories(
        nuclearAbstractModulesTests
    PRIVATE ${BOOST_DIR}
    PRIVATE ${nuclearAbstractModulesHeadersPath}
)

target_sources(
        nuclearAbstractModulesTests
    PRIVATE ${nuclearAbstractModulesPath}/thread
    PRIVATE ${nuclearAbstractModulesPath}/timerQueue
    PRIVATE ${nuclearAbstractModulesPath}/scheduler
    PRIVATE threadTest
    PRIVATE timerQueueTest
    PRIVATE schedulerTest
    PRIVATE runAllTests
)

# GoogleTest library use -pthread
target_compile_options(
        nuclearAbstractModulesTests
    PRIVATE -pthread
)

target_link_options(
        nuclearAbstractModulesTests
    PRIVATE -pthread
)
