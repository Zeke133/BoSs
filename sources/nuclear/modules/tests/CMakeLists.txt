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
    PRIVATE ${GOOGLE_TEST_DIR}/googletest/make/gtest_main.a
)

target_include_directories(
        nuclearAbstractModulesTests
    PRIVATE ${GOOGLE_TEST_DIR}/googletest/include
    PRIVATE ${nuclearAbstractModulesHeadersPath}
)

target_sources(
        nuclearAbstractModulesTests
    PRIVATE ${nuclearAbstractModulesPath}/thread
    PRIVATE ${nuclearAbstractModulesPath}/scheduler
    PRIVATE ${nuclearAbstractModulesPath}/timerQueue
    PRIVATE threadTest
    PRIVATE schedulerTest
    PRIVATE timerQueueTest
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
