set(LIB_MONGOOSE_NAME mongoose)

################################################################

include_directories(${MONGOOSE_HEADER_DIR})
link_directories(${MONGOOSE_LINK_DIR})
add_link_options(-l${LIB_MONGOOSE_NAME})

add_library(${LIB_MONGOOSE_NAME} STATIC IMPORTED)
set_target_properties(${LIB_MONGOOSE_NAME} PROPERTIES
    IMPORTED_LOCATION ${MONGOOSE_LINK_DIR}/libmongoose.so)

################################################################

add_dependencies(${LIB_MONGOOSE_NAME} build-${LIB_MONGOOSE_NAME})
