set(LIB_NANOMSG_NAME nanomsg)

################################################################

include_directories(${NANOMSG_HEADER_DIR})
link_directories(${NANOMSG_LINK_DIR})

################################################################

add_custom_target(${LIB_NANOMSG_NAME})
add_dependencies(${LIB_NANOMSG_NAME} build-${LIB_NANOMSG_NAME})
