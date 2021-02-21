project(freetype)

ExternalProject_Add(build-${PROJECT_NAME}
    URL ${PROJECT_SOURCE_DIR}/src/freetype-2.10.1.tar.gz
    CONFIGURE_COMMAND ./configure --host=${CMAKE_CROSS_PREFIX} --with-zlib=no --with-png=no
    BUILD_COMMAND make
    INSTALL_COMMAND make install DESTDIR=${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-prefix/src/build-${PROJECT_NAME}-build
    BUILD_IN_SOURCE 1)

# set search path
set(FREETYPE_HEADER_DIR "${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-prefix/src/build-${PROJECT_NAME}-build/usr/local/include/freetype2"
    CACHE PATH INTERNAL FORCE)
set(FREETYPE_LINK_DIR "${PROJECT_BINARY_DIR}/build-${PROJECT_NAME}-prefix/src/build-${PROJECT_NAME}-build/usr/local/lib"
    CACHE PATH INTERNAL FORCE)

# install lib.so
install(DIRECTORY "${FREETYPE_LINK_DIR}/" DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FILES_MATCHING PATTERN "libfreetype.so*")