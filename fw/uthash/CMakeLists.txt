project(uthash)

ExternalProject_Add(build-${PROJECT_NAME}
    GIT_REPOSITORY "https://github.com/troydhanson/uthash"
    CONFIGURE_COMMAND true
    BUILD_COMMAND true
    INSTALL_COMMAND mkdir -p ${FREAK_SYSROOT_PATH}/usr/local/include/${PROJECT_NAME}
    COMMAND cmake -E copy_directory ${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-prefix/src/build-${PROJECT_NAME}/include/ ${FREAK_SYSROOT_PATH}/usr/local/include/${PROJECT_NAME}
    BUILD_IN_SOURCE 1)
