set(LIB_FREETYPE_NAME freetype)

################################################################

include_directories(${FREETYPE_HEADER_DIR})
link_directories(${FREETYPE_LINK_DIR})

################################################################

add_custom_target(${LIB_FREETYPE_NAME})
add_dependencies(${LIB_FREETYPE_NAME} build-${LIB_FREETYPE_NAME})