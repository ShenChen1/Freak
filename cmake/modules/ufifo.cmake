set(LIB_UFIFO_NAME ufifo)

################################################################

include_directories(${UFIFO_HEADER_DIR})
link_directories(${UFIFO_LINK_DIR})

################################################################

add_custom_target(${LIB_UFIFO_NAME})
add_dependencies(${LIB_UFIFO_NAME} build-${LIB_UFIFO_NAME})