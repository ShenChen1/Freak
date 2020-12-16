set(LIB_UTHASH_NAME uthash)

################################################################

include_directories(${UTHASH_HEADER_DIR})

################################################################

add_custom_target(${LIB_UTHASH_NAME})
add_dependencies(${LIB_UTHASH_NAME} build-${LIB_UTHASH_NAME})