set(LIB_ST_NAME st)

add_library(${LIB_ST_NAME} STATIC IMPORTED)
set_target_properties(${LIB_ST_NAME} PROPERTIES
    IMPORTED_LOCATION ${HIVIEW_SYSROOT_PATH}/usr/local/lib/libst.a)
include_directories(${HIVIEW_SYSROOT_PATH}/usr/local/include/st)
add_dependencies(${LIB_ST_NAME} build-${LIB_ST_NAME})
