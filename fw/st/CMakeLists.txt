project(st)

ExternalProject_Add(build-${PROJECT_NAME}
    URL ${PROJECT_SOURCE_DIR}/src/st-1.9.tar.gz
    CONFIGURE_COMMAND true
    COMMAND mkdir -p ${HIVIEW_SYSROOT_PATH}/usr/local/lib/
    COMMAND mkdir -p ${HIVIEW_SYSROOT_PATH}/usr/local/include/
    BUILD_COMMAND make linux-optimized CC=${CMAKE_C_COMPILER} LD=${CMAKE_LINER} TARGETDIR=${PROJECT_BINARY_DIR}/build EXAMPLES=
    INSTALL_COMMAND true
    COMMAND cp -av ${PROJECT_BINARY_DIR}/build/libst.so ${HIVIEW_SYSROOT_PATH}/usr/local/lib/
    COMMAND cp -av ${PROJECT_BINARY_DIR}/build/libst.so.1 ${HIVIEW_SYSROOT_PATH}/usr/local/lib/
    COMMAND cp -av ${PROJECT_BINARY_DIR}/build/libst.so.1.9 ${HIVIEW_SYSROOT_PATH}/usr/local/lib/
    COMMAND cp -av ${PROJECT_BINARY_DIR}/build/st.h ${HIVIEW_SYSROOT_PATH}/usr/local/include/st
    BUILD_IN_SOURCE 1)