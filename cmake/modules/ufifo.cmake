set(LIB_UFIFO_NAME ufifo)

################################################################

include_directories(${UFIFO_HEADER_DIR})
link_directories(${UFIFO_LINK_DIR})
add_link_options(-l${LIB_UFIFO_NAME})

add_library(${LIB_UFIFO_NAME} SHARED IMPORTED)
set_target_properties(${LIB_UFIFO_NAME} PROPERTIES
    IMPORTED_LOCATION ${UFIFO_LINK_DIR}/libufifo.so)

################################################################

add_dependencies(${LIB_UFIFO_NAME} build-${LIB_UFIFO_NAME})