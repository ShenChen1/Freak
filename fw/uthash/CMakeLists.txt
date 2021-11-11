project(uthash)

ExternalProject_Add(build-${PROJECT_NAME}
    #GIT_REPOSITORY "https://github.com/troydhanson/uthash"
    URL ${PROJECT_SOURCE_DIR}/src/uthash.tar.gz
    CONFIGURE_COMMAND true
    BUILD_COMMAND true
    INSTALL_COMMAND true
    BUILD_IN_SOURCE 1)

# set search path
set(UTHASH_HEADER_DIR "${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-prefix/src/build-${PROJECT_NAME}/include"
    CACHE PATH INTERNAL FORCE)