set(LIB_UTHASH_NAME uthash)

add_custom_target(${LIB_UTHASH_NAME})
include_directories(${FREAK_SYSROOT_PATH}/usr/local/include/uthash)
add_dependencies(${LIB_UTHASH_NAME} build-${LIB_UTHASH_NAME})