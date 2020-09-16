set(LIB_NANOMSG_NAME nanomsg)

add_library(${LIB_NANOMSG_NAME} SHARED IMPORTED)
set_target_properties(${LIB_NANOMSG_NAME} PROPERTIES
    IMPORTED_LOCATION ${FREAK_SYSROOT_PATH}/lib/libnanomsg.so)
include_directories(${FREAK_SYSROOT_PATH}/include/)
add_dependencies(${LIB_NANOMSG_NAME} build-${LIB_NANOMSG_NAME})
