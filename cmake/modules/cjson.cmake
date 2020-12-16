set(LIB_CJSON_NAME cjson)

################################################################

include_directories(${CJSON_HEADER_DIR})
link_directories(${CJSON_LINK_DIR})
add_link_options(-l${LIB_CJSON_NAME})

add_library(${LIB_CJSON_NAME} SHARED IMPORTED)
set_target_properties(${LIB_UFIFO_NAME} PROPERTIES
    IMPORTED_LOCATION ${CJSON_LINK_DIR}/libcjson.so)

################################################################

add_dependencies(${LIB_CJSON_NAME} build-${LIB_CJSON_NAME})