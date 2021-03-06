cmake_minimum_required(VERSION 3.9)
# ------------------------------------------------------------
project(
    BOSS            # Bare Operating System Solution
    VERSION         0.0.3
    DESCRIPTION     "ARM Cortex-M3 minimal preemptive multitasking OS"
    HOMEPAGE_URL    "https://github.com/Zeke133/BoSs"
    LANGUAGES       ASM C CXX
)
# ------------------------------------------------------------
enable_testing()                        # Unit tests
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)   # generate 'compile_commands.json' file for IntelliSense support
set(CMAKE_VERBOSE_MAKEFILE OFF)         # compilation commands console output
# ------------------------------------------------------------
# Download and unpack googletest at configure time
configure_file(googletest-download/CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()
# Prevent overriding the parent project's compiler/linker settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# Add googletest directly to our build. This defines the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)
# ------------------------------------------------------------
# Path to Boost header only libraries
find_package(Boost REQUIRED)
if(Boost_FOUND)
    set(BOOST_DIR ${Boost_INCLUDE_DIRS})
endif()
# ------------------------------------------------------------
set(use_libg TRUE CACHE BOOL "Use C++ standart library")
if(use_libg)
    message("C++ standart library support enabled. Largest executable size.")
else()
    message("C++ standart library support disabled. Smallest executable size.")
endif()
# ------------------------------------------------------------
message("Build type: ${CMAKE_BUILD_TYPE}")
if(CMAKE_BUILD_TYPE STREQUAL Debug)
    option(use_qemu_dummies "Use dummy implenetations to run project on QEMU" TRUE)
    if(NOT use_qemu_dummies)
        message(SEND_ERROR "QEMU dummies deactivated for Debug build type!")
    endif()
else()
    if(use_qemu_dummies)
        message(SEND_ERROR "QEMU dummies activated for non Debug build type!")
    endif()
endif()
# ------------------------------------------------------------
set(CMAKE_ASM_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
# ---
set(CMAKE_ASM_FLAGS_RELEASE "")
set(CMAKE_C_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
# ---
set(CMAKE_ASM_FLAGS_MINSIZEREL "")
set(CMAKE_C_FLAGS_MINSIZEREL "-Os")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os")
# ---
set(CMAKE_ASM_FLAGS_RELWITHDEBINFO "-g")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-g -Os")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -Os")
# ------------------------------------------------------------
add_subdirectory(libs/cmsis)
add_subdirectory(libs/STM32F1xx_LL_Driver)
add_subdirectory(sources/nuclear)
add_subdirectory(sources/framework)
add_subdirectory(sources/application)
add_subdirectory(sources)   # compile options
# ------------------------------------------------------------
# Link final firmware binary
add_executable(boss.elf)
target_link_libraries(
        boss.elf
    PRIVATE nuclearHardwareModules
    PRIVATE nuclearAbstractModules
    PRIVATE frameworkPeripheral
    PRIVATE frameworkUtils
    PRIVATE application
)
if(NOT use_qemu_dummies)
    target_link_libraries(
            boss.elf
        PRIVATE cmsis
        PRIVATE stmLL
    )
endif()
set_target_properties(
        boss.elf
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
)
# ---
# Link properties
set(PLATFORM_LINK_FLAGS     "-mcpu=cortex-m3 -mthumb")
set(MAP_FILE_OUTPUT_PATH    ${PROJECT_SOURCE_DIR}/bin/boss.map)
# -cref                     Include Cross Reference Table to .map file
# --gc-sections             Throw out unused sections from output binary
# -Map=fileName             Generate .map file
set(USER_LINK_FLAGS_GCC_LD  "-Wl,-Map=${MAP_FILE_OUTPUT_PATH} -Wl,-cref -Wl,--gc-sections")
set(LINKER_MEMORY_QEMU      ${PROJECT_SOURCE_DIR}/sources/nuclear/linker/linkerMemoryQemu.ld)
set(LINKER_MEMORY_STM32     ${PROJECT_SOURCE_DIR}/sources/nuclear/linker/linkerMemoryStm32f103c8t6.ld)
if(use_libg)
    # --specs=nano.specs    Link with 'libg_nano' special embedded optimized 'libg' implementation
    # --specs=nosys.specs   Link with semihosting turned OFF
    set(LIBS_LINK_FLAGS "--specs=nano.specs --specs=nosys.specs")
    set(LINKER_SECTIONS_SCRIPT  ${PROJECT_SOURCE_DIR}/sources/nuclear/linker/linkerSections.ld)
else()
    # -nostdlib             -nodefaultlibs & -nostartfiles: standart library is not linked
    # -nodefaultlibs        Exceptions unwinding and RTTI for example will be disabled
    set(LIBS_LINK_FLAGS "-nostdlib")
    # without .ARM.exidx section
    set(LINKER_SECTIONS_SCRIPT  ${PROJECT_SOURCE_DIR}/sources/nuclear/linker/linkerSectionsNoExc.ld)
endif()
set_target_properties(
        boss.elf
    PROPERTIES
        LINK_FLAGS "${PLATFORM_LINK_FLAGS} ${LIBS_LINK_FLAGS} ${USER_LINK_FLAGS_GCC_LD}"
        LINK_FLAGS_DEBUG "-T${LINKER_MEMORY_QEMU} -T${LINKER_SECTIONS_SCRIPT}"
        LINK_FLAGS_RELEASE "-T${LINKER_MEMORY_STM32} -T${LINKER_SECTIONS_SCRIPT}"
        LINK_FLAGS_RELWITHDEBINFO "-T${LINKER_MEMORY_STM32} -T${LINKER_SECTIONS_SCRIPT}"
        LINK_FLAGS_MINSIZEREL "-T${LINKER_MEMORY_STM32} -T${LINKER_SECTIONS_SCRIPT}"
)
# ------------------------------------------------------------
# Post build tasks
set(HEX_FILE_OUT ${PROJECT_SOURCE_DIR}/bin/boss.hex)
set(BIN_FILE_OUT ${PROJECT_SOURCE_DIR}/bin/boss.bin)
set(SYM_FILE_OUT ${PROJECT_SOURCE_DIR}/bin/boss.sym)
add_custom_command(
    TARGET boss.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:boss.elf> ${HEX_FILE_OUT}
    COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:boss.elf> ${BIN_FILE_OUT}
    COMMAND ${CMAKE_NM} -n $<TARGET_FILE:boss.elf> > ${SYM_FILE_OUT}
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:boss.elf>
    COMMENT "Building ${HEX_FILE_OUT} \nBuilding ${BIN_FILE_OUT}"
)
