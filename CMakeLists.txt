cmake_minimum_required(VERSION 3.18)

# Install git hooks
execute_process(COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/scripts/install-git-hooks)

# Set CMake modules path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/utils")

project(FREAK VERSION 1.0.0 LANGUAGES C CXX)
set(FREAK_HOME_PATH ${PROJECT_SOURCE_DIR})
set(FREAK_BUILD_PATH ${PROJECT_SOURCE_DIR}/build)
set(FREAK_SYSROOT_PATH ${PROJECT_SOURCE_DIR}/sysroot)
set(FREAK_TOOLS_PATH ${PROJECT_SOURCE_DIR}/tools)
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/package)

# tool
include(jsonb)
include(GNUInstallDirs)

add_subdirectory(fw)
add_subdirectory(sdk)
add_subdirectory(mod)