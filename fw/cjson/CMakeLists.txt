project(cjson)

ExternalProject_Add(build-${PROJECT_NAME}
    URL ${PROJECT_SOURCE_DIR}/src/cjson-1.7.13.tar.gz
    CONFIGURE_COMMAND true
    BUILD_COMMAND make CC=${CMAKE_C_COMPILER}
    INSTALL_COMMAND make install DESTDIR=${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-prefix/src/build-${PROJECT_NAME}-build
    BUILD_IN_SOURCE 1)

# set search path
set(CJSON_HEADER_DIR "${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-prefix/src/build-${PROJECT_NAME}-build/usr/local/include/cjson"
    CACHE PATH INTERNAL FORCE)
set(CJSON_LINK_DIR "${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-prefix/src/build-${PROJECT_NAME}-build/usr/local/lib"
    CACHE PATH INTERNAL FORCE)

# install lib.so
install(DIRECTORY "${CJSON_LINK_DIR}/" DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FILES_MATCHING PATTERN "libcjson.so*")