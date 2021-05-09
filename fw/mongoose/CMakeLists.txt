project(mongoose)

ExternalProject_Add(build-${PROJECT_NAME}
    URL ${PROJECT_SOURCE_DIR}/src/mongoose-6.18.tar.gz
    CONFIGURE_COMMAND true
    BUILD_COMMAND true
    COMMAND ${CMAKE_C_COMPILER} -shared -o libmongoose.so mongoose.c -Os -g -fPIC
    INSTALL_COMMAND true
    BUILD_IN_SOURCE 1)

# set search path
set(MONGOOSE_HEADER_DIR "${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-prefix/src/build-${PROJECT_NAME}"
    CACHE PATH INTERNAL FORCE)
set(MONGOOSE_LINK_DIR "${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-prefix/src/build-${PROJECT_NAME}"
    CACHE PATH INTERNAL FORCE)

# install lib.so
install(FILES "${MONGOOSE_LINK_DIR}/libmongoose.so" DESTINATION ${CMAKE_INSTALL_LIBDIR})