cmake_minimum_required(VERSION 3.18)

# Install git hooks
execute_process(COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/scripts/install-git-hooks)

# Set CMake modules path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")

project(FREAK)
set(FREAK_HOME_PATH ${PROJECT_SOURCE_DIR})
set(FREAK_BUILD_PATH ${PROJECT_SOURCE_DIR}/build)
set(FREAK_SYSROOT_PATH ${PROJECT_SOURCE_DIR}/sysroot)
set(FREAK_TOOLS_PATH ${PROJECT_SOURCE_DIR}/tools)

add_subdirectory(fw)
add_subdirectory(sdk)
add_subdirectory(mod)