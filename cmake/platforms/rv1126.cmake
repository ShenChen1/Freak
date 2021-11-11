set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_CHIP_NAME rv1126)

set(CMAKE_CROSS_PREFIX  "arm-linux-gnueabihf")
set(CMAKE_C_COMPILER    "arm-linux-gnueabihf-gcc")
set(CMAKE_CXX_COMPILER  "arm-linux-gnueabihf-g++")
set(CMAKE_AR            arm-linux-gnueabihf-ar CACHE FILEPATH "Archiver")
set(CMAKE_LINER         "arm-linux-gnueabihf-g++")
set(CMAKE_LD            "arm-linux-gnueabihf-ld")
#set(CMAKE_SYSROOT /home/cjyvm2/3516rootfs/rootfs)
# This crosstool doesn't define SO_REUSEPORT
add_compile_options(-Wno-unused-function)
#add_compile_options(-std=c++11)
# This chip provides the floating-point operation unit and NEON.
# The libraries in the file system are compiled with hard floating point and neon compatible with the soft floating point call interface.
#add_compile_options(-mcpu=cortex-a7 -mfloat-abi=softfp -mfpu=neon-vfpv4 -fno-aggressive-loop-optimizations)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
