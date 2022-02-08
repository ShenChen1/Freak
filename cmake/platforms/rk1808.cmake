set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_CHIP_NAME rk1808)

set(CMAKE_CROSS_PREFIX  "aarch64-linux-gnu")
set(CMAKE_C_COMPILER    "aarch64-linux-gnu-gcc")
set(CMAKE_CXX_COMPILER  "aarch64-linux-gnu-g++")
set(CMAKE_AR            "aarch64-linux-gnu-ar")
set(CMAKE_LINER         "aarch64-linux-gnu-g++")
set(CMAKE_LD            "aarch64-linux-gnu-ld")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)