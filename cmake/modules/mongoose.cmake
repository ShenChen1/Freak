set(LIB_MONGOOSE_NAME mongoose)

################################################################

include_directories(${MONGOOSE_HEADER_DIR})
link_directories(${MONGOOSE_LINK_DIR})

################################################################

add_custom_target(${LIB_MONGOOSE_NAME})
add_dependencies(${LIB_MONGOOSE_NAME} build-${LIB_MONGOOSE_NAME})
