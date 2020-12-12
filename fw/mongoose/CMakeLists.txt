project(mongoose)

ExternalProject_Add(build-${PROJECT_NAME}
    URL ${PROJECT_SOURCE_DIR}/src/mongoose-6.18.tar.gz
    CONFIGURE_COMMAND true
    BUILD_COMMAND true
    COMMAND ${CMAKE_C_COMPILER} -o mongoose.o -c mongoose.c -Wall -Werror -Os -g
    COMMAND ${CMAKE_AR} -src libmongoose.a mongoose.o
    INSTALL_COMMAND true
    COMMAND mkdir -p ${FREAK_SYSROOT_PATH}/include/mongoose
    COMMAND cp -av mongoose.h ${FREAK_SYSROOT_PATH}/include/mongoose
    COMMAND mkdir -p ${FREAK_SYSROOT_PATH}/lib/
    COMMAND cp -av libmongoose.a ${FREAK_SYSROOT_PATH}/lib/
    BUILD_IN_SOURCE 1)