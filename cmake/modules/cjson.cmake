set(LIB_CJSON_NAME cjson)

add_library(${LIB_CJSON_NAME} SHARED IMPORTED)
set_target_properties(${LIB_CJSON_NAME} PROPERTIES
    IMPORTED_LOCATION ${FREAK_SYSROOT_PATH}/usr/local/lib/libcjson.so)
include_directories(${FREAK_SYSROOT_PATH}/usr/local/include/cjson)
add_dependencies(${LIB_CJSON_NAME} build-${LIB_CJSON_NAME})
