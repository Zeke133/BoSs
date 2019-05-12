# ---
# BoSs framework
# ---
set(frameworkPath ${PROJECT_SOURCE_DIR}/sources/framework)
set(frameworkInterfacesPath ${frameworkPath}/interfaces)

add_subdirectory(peripheral)        # internal peripheral devices drivers
add_subdirectory(utils)             # hardware independent utils
