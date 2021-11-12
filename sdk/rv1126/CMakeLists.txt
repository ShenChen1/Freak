project(sdk-rv1126)

ExternalProject_Add(${PROJECT_NAME}
    URL ${PROJECT_SOURCE_DIR}/sdk_rv1126.tar.gz
    CONFIGURE_COMMAND true
    BUILD_COMMAND true
    INSTALL_COMMAND true
    COMMAND mkdir -p ${FREAK_HOME_PATH}/mod/vsf-rv1126/sdk_inc/
    COMMAND cmake -E copy_directory ${PROJECT_BINARY_DIR}/${PROJECT_NAME}-prefix/src/${PROJECT_NAME}/include/ ${FREAK_HOME_PATH}/mod/vsf-rv1126/sdk_inc/
    COMMAND cp -rf ${PROJECT_BINARY_DIR}/${PROJECT_NAME}-prefix/src/${PROJECT_NAME}/isp_include/ ${FREAK_HOME_PATH}/mod/vsf-rv1126/
    #COMMAND cp -av ${PROJECT_SOURCE_DIR}/libtracker.so ${PROJECT_BINARY_DIR}/${PROJECT_NAME}-prefix/src/${PROJECT_NAME}/lib/
    BUILD_IN_SOURCE 1)

# set search path
set(MEDIA_SDK_HEADER_DIR "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-prefix/src/${PROJECT_NAME}/include"
    CACHE PATH INTERNAL FORCE)
set(MEDIA_SDK_LINK_DIR "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-prefix/src/${PROJECT_NAME}/lib"
    CACHE PATH INTERNAL FORCE)


# set so and ko
install(DIRECTORY "${MEDIA_SDK_LINK_DIR}" DESTINATION ${CMAKE_INSTALL_PREFIX}/usr/local
    FILES_MATCHING PATTERN "*.so")