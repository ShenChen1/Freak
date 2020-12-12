project(media-server)

ExternalProject_Add(build-${PROJECT_NAME}
    GIT_REPOSITORY "https://github.com/ShenChen1/media-server.git"
    CONFIGURE_COMMAND true
    BUILD_COMMAND true
    COMMAND make CROSS=${CMAKE_CROSS_PREFIX}- RELEASE=0 OUTPATH=${PROJECT_BINARY_DIR}/build -C libflv
    COMMAND make CROSS=${CMAKE_CROSS_PREFIX}- RELEASE=0 OUTPATH=${PROJECT_BINARY_DIR}/build -C libmov
    COMMAND make CROSS=${CMAKE_CROSS_PREFIX}- RELEASE=0 OUTPATH=${PROJECT_BINARY_DIR}/build -C librtmp
    COMMAND make CROSS=${CMAKE_CROSS_PREFIX}- RELEASE=0 OUTPATH=${PROJECT_BINARY_DIR}/build -C librtp
    COMMAND make CROSS=${CMAKE_CROSS_PREFIX}- RELEASE=0 OUTPATH=${PROJECT_BINARY_DIR}/build ROOT=${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-sdk-prefix/src/build-${PROJECT_NAME}-sdk/ -C librtsp
    COMMAND make CROSS=${CMAKE_CROSS_PREFIX}- RELEASE=0 OUTPATH=${PROJECT_BINARY_DIR}/build ROOT=${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-sdk-prefix/src/build-${PROJECT_NAME}-sdk/ -C libsip
    INSTALL_COMMAND true
    COMMAND mkdir -p ${FREAK_SYSROOT_PATH}/usr/local/lib/
    COMMAND cp -av ${PROJECT_BINARY_DIR}/build/libflv.a ${FREAK_SYSROOT_PATH}/usr/local/lib/
    COMMAND cp -av ${PROJECT_BINARY_DIR}/build/libmov.a ${FREAK_SYSROOT_PATH}/usr/local/lib/
    COMMAND cp -av ${PROJECT_BINARY_DIR}/build/librtmp.a ${FREAK_SYSROOT_PATH}/usr/local/lib/
    COMMAND cp -av ${PROJECT_BINARY_DIR}/build/librtp.a ${FREAK_SYSROOT_PATH}/usr/local/lib/
    COMMAND cp -av ${PROJECT_BINARY_DIR}/build/librtsp.a ${FREAK_SYSROOT_PATH}/usr/local/lib/
    COMMAND cp -av ${PROJECT_BINARY_DIR}/build/libsip.a ${FREAK_SYSROOT_PATH}/usr/local/lib/
    COMMAND mkdir -p ${FREAK_SYSROOT_PATH}/usr/local/include/${PROJECT_NAME}
    COMMAND cmake -E copy_directory ${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-prefix/src/build-${PROJECT_NAME}/libflv/include/ ${FREAK_SYSROOT_PATH}/usr/local/include/${PROJECT_NAME}/libflv
    COMMAND cmake -E copy_directory ${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-prefix/src/build-${PROJECT_NAME}/libmov/include/ ${FREAK_SYSROOT_PATH}/usr/local/include/${PROJECT_NAME}/libmov
    COMMAND cmake -E copy_directory ${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-prefix/src/build-${PROJECT_NAME}/librtmp/include/ ${FREAK_SYSROOT_PATH}/usr/local/include/${PROJECT_NAME}/librtmp
    COMMAND cmake -E copy_directory ${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-prefix/src/build-${PROJECT_NAME}/librtp/include/ ${FREAK_SYSROOT_PATH}/usr/local/include/${PROJECT_NAME}/librtp
    COMMAND cmake -E copy_directory ${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-prefix/src/build-${PROJECT_NAME}/librtsp/include/ ${FREAK_SYSROOT_PATH}/usr/local/include/${PROJECT_NAME}/librtsp
    COMMAND cmake -E copy_directory ${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-prefix/src/build-${PROJECT_NAME}/libsip/include/ ${FREAK_SYSROOT_PATH}/usr/local/include/${PROJECT_NAME}/libsip
    BUILD_IN_SOURCE 1)

ExternalProject_Add(build-${PROJECT_NAME}-sdk
    GIT_REPOSITORY "https://github.com/ShenChen1/sdk.git"
    CONFIGURE_COMMAND true
    BUILD_COMMAND true
    COMMAND make CROSS=${CMAKE_CROSS_PREFIX}- RELEASE=0 OUTPATH=${PROJECT_BINARY_DIR}/build -C libhttp
    COMMAND make CROSS=${CMAKE_CROSS_PREFIX}- RELEASE=0 OUTPATH=${PROJECT_BINARY_DIR}/build -C libaio
    COMMAND make CROSS=${CMAKE_CROSS_PREFIX}- RELEASE=0 OUTPATH=${PROJECT_BINARY_DIR}/build -C source
    INSTALL_COMMAND true
    COMMAND mkdir -p ${FREAK_SYSROOT_PATH}/usr/local/lib/
    COMMAND cp -av ${PROJECT_BINARY_DIR}/build/libhttp.a ${FREAK_SYSROOT_PATH}/usr/local/lib/
    COMMAND cp -av ${PROJECT_BINARY_DIR}/build/libaio.so ${FREAK_SYSROOT_PATH}/usr/local/lib/
    COMMAND cp -av ${PROJECT_BINARY_DIR}/build/libutils.so ${FREAK_SYSROOT_PATH}/usr/local/lib/libutils-media.so
    COMMAND mkdir -p ${FREAK_SYSROOT_PATH}/usr/local/include/${PROJECT_NAME}
    COMMAND cmake -E copy_directory ${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-sdk-prefix/src/build-${PROJECT_NAME}-sdk/libhttp/include/ ${FREAK_SYSROOT_PATH}/usr/local/include/${PROJECT_NAME}/libhttp
    COMMAND cmake -E copy_directory ${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-sdk-prefix/src/build-${PROJECT_NAME}-sdk/libaio/include/ ${FREAK_SYSROOT_PATH}/usr/local/include/${PROJECT_NAME}/libaio
    COMMAND cmake -E copy_directory ${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-sdk-prefix/src/build-${PROJECT_NAME}-sdk/include/ ${FREAK_SYSROOT_PATH}/usr/local/include/${PROJECT_NAME}
    BUILD_IN_SOURCE 1)
add_dependencies(build-${PROJECT_NAME} build-${PROJECT_NAME}-sdk)
