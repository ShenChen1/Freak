set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_CROSS_PREFIX  "arm-himix200-linux")
set(CMAKE_C_COMPILER    "arm-himix200-linux-gcc")
set(CMAKE_CXX_COMPILER  "arm-himix200-linux-g++")
set(CMAKE_AR            "arm-himix200-linux-ar")
set(CMAKE_LINER         "arm-himix200-linux-g++")
set(CMAKE_LD            "arm-himix200-linux-ld")

# This crosstool doesn't define SO_REUSEPORT
add_compile_options(-Wno-unused-function)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
