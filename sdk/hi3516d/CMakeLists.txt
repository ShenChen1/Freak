project(sdk-hi3516d)

ExternalProject_Add(${PROJECT_NAME}
    URL ${PROJECT_SOURCE_DIR}/mpp_smp_linux.tgz
    CONFIGURE_COMMAND true
    BUILD_COMMAND true
    INSTALL_COMMAND true
    COMMAND mkdir -p ${FREAK_SYSROOT_PATH}/usr/local/lib/mpp/
    COMMAND cmake -E copy_directory ${PROJECT_BINARY_DIR}/${PROJECT_NAME}-prefix/src/${PROJECT_NAME}/lib/ ${FREAK_SYSROOT_PATH}/usr/local/lib/mpp/
    COMMAND mkdir -p ${FREAK_SYSROOT_PATH}/usr/local/include/mpp/
    COMMAND cmake -E copy_directory ${PROJECT_BINARY_DIR}/${PROJECT_NAME}-prefix/src/${PROJECT_NAME}/include/ ${FREAK_SYSROOT_PATH}/usr/local/include/mpp/
    COMMAND mkdir -p ${FREAK_SYSROOT_PATH}/usr/local/ko/
    COMMAND cmake -E copy_directory ${PROJECT_BINARY_DIR}/${PROJECT_NAME}-prefix/src/${PROJECT_NAME}/ko/ ${FREAK_SYSROOT_PATH}/usr/local/ko/
    COMMAND mkdir -p ${FREAK_HOME_PATH}/mod/vsf-hi3516d/sample/
    COMMAND cmake -E copy_directory ${PROJECT_BINARY_DIR}/${PROJECT_NAME}-prefix/src/${PROJECT_NAME}/sample/common ${FREAK_HOME_PATH}/mod/vsf-hi3516d/sample/
    BUILD_IN_SOURCE 1)