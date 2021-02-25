project(media-server)

ExternalProject_Add(build-${PROJECT_NAME}
    GIT_REPOSITORY "https://github.com/ireader/media-server.git"
    CONFIGURE_COMMAND true
    BUILD_COMMAND true
    COMMAND make CROSS=${CMAKE_CROSS_PREFIX}- RELEASE=0 OUTPATH=${PROJECT_BINARY_DIR}/build -C libflv
    COMMAND make CROSS=${CMAKE_CROSS_PREFIX}- RELEASE=0 OUTPATH=${PROJECT_BINARY_DIR}/build -C libmov
    COMMAND make CROSS=${CMAKE_CROSS_PREFIX}- RELEASE=0 OUTPATH=${PROJECT_BINARY_DIR}/build -C librtmp
    COMMAND make CROSS=${CMAKE_CROSS_PREFIX}- RELEASE=0 OUTPATH=${PROJECT_BINARY_DIR}/build -C librtp
    COMMAND make CROSS=${CMAKE_CROSS_PREFIX}- RELEASE=0 OUTPATH=${PROJECT_BINARY_DIR}/build ROOT=${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-sdk-prefix/src/build-${PROJECT_NAME}-sdk/ -C librtsp
    COMMAND make CROSS=${CMAKE_CROSS_PREFIX}- RELEASE=0 OUTPATH=${PROJECT_BINARY_DIR}/build ROOT=${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-sdk-prefix/src/build-${PROJECT_NAME}-sdk/ -C libsip
    INSTALL_COMMAND true
    COMMAND cmake -E copy_directory ${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-prefix/src/build-${PROJECT_NAME}/libflv/include/ ${PROJECT_BINARY_DIR}/build/include/libflv
    COMMAND cmake -E copy_directory ${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-prefix/src/build-${PROJECT_NAME}/libmov/include/ ${PROJECT_BINARY_DIR}/build/include/libmov
    COMMAND cmake -E copy_directory ${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-prefix/src/build-${PROJECT_NAME}/librtmp/include/ ${PROJECT_BINARY_DIR}/build/include/librtmp
    COMMAND cmake -E copy_directory ${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-prefix/src/build-${PROJECT_NAME}/librtp/include/ ${PROJECT_BINARY_DIR}/build/include/librtp
    COMMAND cmake -E copy_directory ${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-prefix/src/build-${PROJECT_NAME}/librtsp/include/ ${PROJECT_BINARY_DIR}/build/include/librtsp
    COMMAND cmake -E copy_directory ${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-prefix/src/build-${PROJECT_NAME}/libsip/include/ ${PROJECT_BINARY_DIR}/build/include/libsip
    BUILD_IN_SOURCE 1)

# apply patch
ExternalProject_Add_Step(build-${PROJECT_NAME} apply-patch
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-prefix/src/build-${PROJECT_NAME}
    COMMENT "Apply patches for build-${PROJECT_NAME}"
    COMMAND git apply ${PROJECT_SOURCE_DIR}/0001-fw-patch-for-build-media-server.patch
    DEPENDEES download)

# set search path
set(MEDIA_SRV_HEADER_DIR "${PROJECT_BINARY_DIR}/build/include/"
    CACHE PATH INTERNAL FORCE)
set(MEDIA_SRV_LINK_DIR "${PROJECT_BINARY_DIR}/build"
    CACHE PATH INTERNAL FORCE)

ExternalProject_Add(build-${PROJECT_NAME}-sdk
    GIT_REPOSITORY "https://github.com/ireader/sdk.git"
    CONFIGURE_COMMAND true
    BUILD_COMMAND true
    COMMAND make CROSS=${CMAKE_CROSS_PREFIX}- RELEASE=0 OUTPATH=${PROJECT_BINARY_DIR}/build -C libhttp
    COMMAND make CROSS=${CMAKE_CROSS_PREFIX}- RELEASE=0 OUTPATH=${PROJECT_BINARY_DIR}/build -C libaio
    COMMAND make CROSS=${CMAKE_CROSS_PREFIX}- RELEASE=0 OUTPATH=${PROJECT_BINARY_DIR}/build -C source
    INSTALL_COMMAND true
    COMMAND mv ${PROJECT_BINARY_DIR}/build/libutils.so ${PROJECT_BINARY_DIR}/build/libutils-media.so
    COMMAND cmake -E copy_directory ${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-sdk-prefix/src/build-${PROJECT_NAME}-sdk/libhttp/include/ ${PROJECT_BINARY_DIR}/build/include/libhttp
    COMMAND cmake -E copy_directory ${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-sdk-prefix/src/build-${PROJECT_NAME}-sdk/libaio/include/ ${PROJECT_BINARY_DIR}/build/include/libaio
    COMMAND cmake -E copy_directory ${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-sdk-prefix/src/build-${PROJECT_NAME}-sdk/include/ ${PROJECT_BINARY_DIR}/build/include
    BUILD_IN_SOURCE 1)
add_dependencies(build-${PROJECT_NAME} build-${PROJECT_NAME}-sdk)

# apply patch
ExternalProject_Add_Step(build-${PROJECT_NAME}-sdk apply-patch
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-sdk-prefix/src/build-${PROJECT_NAME}-sdk
    COMMENT "Apply patches for build-${PROJECT_NAME}-sdk"
    COMMAND git apply ${PROJECT_SOURCE_DIR}/0001-fw-patch-for-build-sdk.patch
    DEPENDEES download)

# set search path
set(MEDIA_SRV_SDK_HEADER_DIR "${PROJECT_BINARY_DIR}/build/include"
    CACHE PATH INTERNAL FORCE)
set(MEDIA_SRV_SDK_LINK_DIR "${PROJECT_BINARY_DIR}/build"
    CACHE PATH INTERNAL FORCE)

# set lib.so
install(FILES "${MEDIA_SRV_SDK_LINK_DIR}/libaio.so" DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES "${MEDIA_SRV_SDK_LINK_DIR}/libutils-media.so" DESTINATION ${CMAKE_INSTALL_LIBDIR})