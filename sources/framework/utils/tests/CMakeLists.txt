# ---
# BoSs framework utils Unit tests
# ---

add_test(
    NAME    BoSsFrameworkUtilsUnitTests
    COMMAND frameworkUtilsTests
)

# Set usual GCC compiler, because other targets use toolchain configuration.
set(CMAKE_CXX_COMPILER  g++)

add_executable(frameworkUtilsTests)

target_link_libraries(
        frameworkUtilsTests
    PRIVATE gtest_main
)

target_include_directories(
        frameworkUtilsTests
    PRIVATE ${frameworkUtilsHeadersPath}
)

target_sources(
        frameworkUtilsTests
    PRIVATE ${frameworkUtilsModulesPath}/convertation
    PRIVATE ${frameworkUtilsModulesPath}/crc
    PRIVATE convertationTest
    PRIVATE crcTest
    PRIVATE runAllTests
)

# GoogleTest library use -pthread
target_compile_options(
        frameworkUtilsTests
    PRIVATE -pthread
)

target_link_options(
        frameworkUtilsTests
    PRIVATE -pthread
)
