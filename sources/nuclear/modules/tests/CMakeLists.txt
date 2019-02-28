# ---
# BoSs kernel abstraction layer Unit tests
# ---

add_test(
    NAME    BoSsKernelUnitTests
    COMMAND nuclearAbstractModulesTests
)

# Set compiler and linker to build unit tests
# because arm-toolchain was set previously
# GoogleTest library use pthread
set(CMAKE_CXX_COMPILER  g++)
set(CMAKE_LINKER        g++)
set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_LINKER>  -pthread <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

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
    PRIVATE threadTest
    PRIVATE schedulerTest
    PRIVATE runAllTests
)
