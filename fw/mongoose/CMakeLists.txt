project(mongoose)

ExternalProject_Add(build-${PROJECT_NAME}
    URL ${PROJECT_SOURCE_DIR}/src/mongoose-6.18.tar.gz
    CONFIGURE_COMMAND true
    BUILD_COMMAND ${CMAKE_C_COMPILER} -shared -o libmongoose.so mongoose.c -Wall -Werror -Os -g -fPIC
    INSTALL_COMMAND true
    COMMAND mkdir -p ${FREAK_SYSROOT_PATH}/usr/include/mongoose
    COMMAND cp -av mongoose.h ${FREAK_SYSROOT_PATH}/usr/include/mongoose
    COMMAND mkdir -p ${FREAK_SYSROOT_PATH}/usr/lib/
    COMMAND cp -av libmongoose.so ${FREAK_SYSROOT_PATH}/usr/lib/
    BUILD_IN_SOURCE 1)