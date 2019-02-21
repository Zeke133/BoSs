# ---
# Compile OS modules to object file
# ---
add_library(modules OBJECT)

set(KERNEL_DIR          kernel)
set(UTILS_DIR           utils)
# set(CMSIS_LIB_DIR       ../libs/cmsis/ARMCM3)

set(H_DIR               headers)
set(SRC_DIR             modules)

target_include_directories(
    modules
    # PRIVATE ${CMSIS_LIB_DIR}
    PRIVATE ${KERNEL_DIR}/${H_DIR}
    PRIVATE ${UTILS_DIR}/${H_DIR}
)

target_sources(
    modules
    PRIVATE ${KERNEL_DIR}/${SRC_DIR}/core
    PRIVATE ${KERNEL_DIR}/${SRC_DIR}/thread
    PRIVATE ${KERNEL_DIR}/${SRC_DIR}/scheduler
)
