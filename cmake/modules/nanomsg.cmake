set(LIB_NANOMSG_NAME nanomsg)

################################################################

include_directories(${NANOMSG_HEADER_DIR})
link_directories(${NANOMSG_LINK_DIR})
add_link_options(-l${LIB_NANOMSG_NAME})

add_library(${LIB_NANOMSG_NAME} SHARED IMPORTED)
set_target_properties(${LIB_NANOMSG_NAME} PROPERTIES
    IMPORTED_LOCATION ${NANOMSG_LINK_DIR}/libnanomsg.so)

################################################################

add_dependencies(${LIB_NANOMSG_NAME} build-${LIB_NANOMSG_NAME})
