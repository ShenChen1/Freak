project(media-server)

ExternalProject_Add(build-${PROJECT_NAME}
    GIT_REPOSITORY "https://github.com/ireader/media-server.git"
    CONFIGURE_COMMAND true
    BUILD_COMMAND true
    COMMAND make CROSS=${CMAKE_CROSS_PREFIX}- RELEASE=1 OUTTYPE=1 OUTPATH=${PROJECT_BINARY_DIR}/build OUTFILE=libflv.so -C libflv
    COMMAND make CROSS=${CMAKE_CROSS_PREFIX}- RELEASE=1 OUTTYPE=1 OUTPATH=${PROJECT_BINARY_DIR}/build OUTFILE=libmov.so -C libmov
    COMMAND make CROSS=${CMAKE_CROSS_PREFIX}- RELEASE=1 OUTTYPE=1 OUTPATH=${PROJECT_BINARY_DIR}/build OUTFILE=librtmp.so -C librtmp
    COMMAND make CROSS=${CMAKE_CROSS_PREFIX}- RELEASE=1 OUTTYPE=1 OUTPATH=${PROJECT_BINARY_DIR}/build OUTFILE=librtp.so -C librtp
    COMMAND make CROSS=${CMAKE_CROSS_PREFIX}- RELEASE=1 OUTTYPE=1 OUTPATH=${PROJECT_BINARY_DIR}/build OUTFILE=librtsp.so ROOT=${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-sdk-prefix/src/build-${PROJECT_NAME}-sdk/ -C librtsp
    COMMAND make CROSS=${CMAKE_CROSS_PREFIX}- RELEASE=1 OUTTYPE=1 OUTPATH=${PROJECT_BINARY_DIR}/build OUTFILE=libsip.so ROOT=${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-sdk-prefix/src/build-${PROJECT_NAME}-sdk/ -C libsip
    INSTALL_COMMAND true
    COMMAND mkdir -p ${HIVIEW_SYSROOT_PATH}/usr/local/lib/
    COMMAND cp -av ${PROJECT_BINARY_DIR}/build/libflv.so ${HIVIEW_SYSROOT_PATH}/usr/local/lib/
    COMMAND cp -av ${PROJECT_BINARY_DIR}/build/libmov.so ${HIVIEW_SYSROOT_PATH}/usr/local/lib/
    COMMAND cp -av ${PROJECT_BINARY_DIR}/build/librtmp.so ${HIVIEW_SYSROOT_PATH}/usr/local/lib/
    COMMAND cp -av ${PROJECT_BINARY_DIR}/build/librtp.so ${HIVIEW_SYSROOT_PATH}/usr/local/lib/
    COMMAND cp -av ${PROJECT_BINARY_DIR}/build/librtsp.so ${HIVIEW_SYSROOT_PATH}/usr/local/lib/
    COMMAND cp -av ${PROJECT_BINARY_DIR}/build/libsip.so ${HIVIEW_SYSROOT_PATH}/usr/local/lib/
    COMMAND mkdir -p ${HIVIEW_SYSROOT_PATH}/usr/local/include/${PROJECT_NAME}
    COMMAND cmake -E copy_directory ${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-prefix/src/build-${PROJECT_NAME}/libflv/include/ ${HIVIEW_SYSROOT_PATH}/usr/local/include/${PROJECT_NAME}
    COMMAND cmake -E copy_directory ${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-prefix/src/build-${PROJECT_NAME}/libmov/include/ ${HIVIEW_SYSROOT_PATH}/usr/local/include/${PROJECT_NAME}
    COMMAND cmake -E copy_directory ${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-prefix/src/build-${PROJECT_NAME}/librtmp/include/ ${HIVIEW_SYSROOT_PATH}/usr/local/include/${PROJECT_NAME}
    COMMAND cmake -E copy_directory ${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-prefix/src/build-${PROJECT_NAME}/librtp/include/ ${HIVIEW_SYSROOT_PATH}/usr/local/include/${PROJECT_NAME}
    COMMAND cmake -E copy_directory ${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-prefix/src/build-${PROJECT_NAME}/librtsp/include/ ${HIVIEW_SYSROOT_PATH}/usr/local/include/${PROJECT_NAME}
    COMMAND cmake -E copy_directory ${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-prefix/src/build-${PROJECT_NAME}/libsip/include/ ${HIVIEW_SYSROOT_PATH}/usr/local/include/${PROJECT_NAME}
    BUILD_IN_SOURCE 1)

ExternalProject_Add(build-${PROJECT_NAME}-sdk
    GIT_REPOSITORY "https://github.com/ireader/sdk.git"
    CONFIGURE_COMMAND true
    BUILD_COMMAND true
    COMMAND make CROSS=${CMAKE_CROSS_PREFIX}- RELEASE=1 OUTTYPE=1 OUTPATH=${PROJECT_BINARY_DIR}/build OUTFILE=libhttp.so -C libhttp
    INSTALL_COMMAND true
    COMMAND mkdir -p ${HIVIEW_SYSROOT_PATH}/usr/local/lib/
    COMMAND cp -av ${PROJECT_BINARY_DIR}/build/libhttp.so ${HIVIEW_SYSROOT_PATH}/usr/local/lib/
    COMMAND mkdir -p ${HIVIEW_SYSROOT_PATH}/usr/local/include/${PROJECT_NAME}
    COMMAND cmake -E copy_directory ${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-sdk-prefix/src/build-${PROJECT_NAME}-sdk/libhttp/include/ ${HIVIEW_SYSROOT_PATH}/usr/local/include/${PROJECT_NAME}
    BUILD_IN_SOURCE 1)
add_dependencies(build-${PROJECT_NAME} build-${PROJECT_NAME}-sdk)
