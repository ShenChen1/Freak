set(LIB_MONGOOSE_NAME mongoose)

add_library(${LIB_MONGOOSE_NAME} SHARED IMPORTED)
set_target_properties(${LIB_MONGOOSE_NAME} PROPERTIES
    IMPORTED_LOCATION ${FREAK_SYSROOT_PATH}/usr/local/lib/libmongoose.so)
include_directories(${FREAK_SYSROOT_PATH}/usr/local/include/mongoose)
add_dependencies(${LIB_MONGOOSE_NAME} build-${LIB_MONGOOSE_NAME})
