set(LIB_UFIFO_NAME ufifo)

add_library(${LIB_UFIFO_NAME} STATIC IMPORTED)
set_target_properties(${LIB_UFIFO_NAME} PROPERTIES
    IMPORTED_LOCATION ${HIVIEW_SYSROOT_PATH}/usr/local/lib/libufifo.so)
include_directories(${HIVIEW_SYSROOT_PATH}/usr/local/include/ufifo)
add_dependencies(${LIB_UFIFO_NAME} build-${LIB_UFIFO_NAME})
