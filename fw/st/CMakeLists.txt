project(st)

ExternalProject_Add(build-${PROJECT_NAME}
    URL ${PROJECT_SOURCE_DIR}/src/st-1.9.tar.gz
    CONFIGURE_COMMAND true
    BUILD_COMMAND make linux-optimized CC=${CMAKE_C_COMPILER} LD=${CMAKE_LD} TARGETDIR=${PROJECT_BINARY_DIR}/build EXAMPLES=
    INSTALL_COMMAND true
    COMMAND mkdir -p ${FREAK_SYSROOT_PATH}/usr/local/lib/
    COMMAND cp -av ${PROJECT_BINARY_DIR}/build/libst.a ${FREAK_SYSROOT_PATH}/usr/local/lib/
    COMMAND mkdir -p ${FREAK_SYSROOT_PATH}/usr/local/include/${PROJECT_NAME}
    COMMAND cp -av ${PROJECT_BINARY_DIR}/build/st.h ${FREAK_SYSROOT_PATH}/usr/local/include/${PROJECT_NAME}
    BUILD_IN_SOURCE 1)