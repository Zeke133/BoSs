# ---
# Compile OS modules to object file
# ---
add_library(modules OBJECT)
# target_link_libraries(modules PRIVATE startup stm_ll)

set(CORE_DIR            core)
set(UTILS_DIR           utils)
set(CODE_STYLE_DIR      examples/codeStyle)

set(H_DIR               headers)
set(SRC_DIR             modules)

target_include_directories(
    modules
    PRIVATE ${CODE_STYLE_DIR}
    PRIVATE ${CORE_DIR}/${H_DIR}
    PRIVATE ${UTILS_DIR}/${H_DIR}
)

target_sources(
    modules
    PRIVATE ${CODE_STYLE_DIR}/sourceCodeTemplate
    PRIVATE ${CORE_DIR}/${SRC_DIR}/core
    PRIVATE ${UTILS_DIR}/${SRC_DIR}/convertation
)
