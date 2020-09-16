project(ufifo)

ExternalProject_Add(build-${PROJECT_NAME}
    GIT_REPOSITORY "https://github.com/ShenChen1/ufifo.git"
    CONFIGURE_COMMAND true
    BUILD_COMMAND make all CROSS_COMPILE=${CMAKE_CROSS_PREFIX}
    INSTALL_COMMAND make install PREFIX=${FREAK_SYSROOT_PATH}
    BUILD_IN_SOURCE 1)
