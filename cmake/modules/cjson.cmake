set(LIB_CJSON_NAME cjson)

################################################################

include_directories(${CJSON_HEADER_DIR})
link_directories(${CJSON_LINK_DIR})

################################################################

add_custom_target(${LIB_CJSON_NAME})
add_dependencies(${LIB_CJSON_NAME} build-${LIB_CJSON_NAME})