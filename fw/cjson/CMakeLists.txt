project(cjson)

ExternalProject_Add(build-${PROJECT_NAME}
    URL ${PROJECT_SOURCE_DIR}/src/cjson-1.7.13.tar.gz
    CONFIGURE_COMMAND true
    BUILD_COMMAND make CC=${CMAKE_C_COMPILER}
    INSTALL_COMMAND make install DESTDIR=${FREAK_SYSROOT_PATH}
    BUILD_IN_SOURCE 1)