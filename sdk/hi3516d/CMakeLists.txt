project(sdk-hi3516d)

ExternalProject_Add(build-${PROJECT_NAME}
    URL ${PROJECT_SOURCE_DIR}/../Hi3516CV500_SDK_V2.0.2.0.tgz
    CONFIGURE_COMMAND true
    BUILD_COMMAND ./sdk.unpack
    INSTALL_COMMAND true
    COMMAND mkdir -p ${FREAK_SYSROOT_PATH}/usr/local/lib/
    COMMAND cmake -E copy_directory ${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-prefix/src/build-${PROJECT_NAME}/smp/a7_linux/mpp/lib/ ${FREAK_SYSROOT_PATH}/usr/local/lib/
    COMMAND mkdir -p ${FREAK_SYSROOT_PATH}/usr/local/include/mpp/
    COMMAND cmake -E copy_directory ${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-prefix/src/build-${PROJECT_NAME}/smp/a7_linux/mpp/include/ ${FREAK_SYSROOT_PATH}/usr/local/include/mpp/
    BUILD_IN_SOURCE 1)