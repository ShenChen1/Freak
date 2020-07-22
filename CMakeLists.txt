cmake_minimum_required(VERSION 3.5)

# Set CMake modules path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")

project(HIVIEW)
set(HIVIEW_SYSROOT_PATH ${PROJECT_SOURCE_DIR}/sysroot)

add_subdirectory(fw)
add_subdirectory(mod)