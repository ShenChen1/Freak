set(LIB_UTHASH_NAME uthash)

add_library(${LIB_UTHASH_NAME} MODULE IMPORTED)
include_directories(${HIVIEW_SYSROOT_PATH}/usr/local/include/uthash)
add_dependencies(${LIB_UTHASH_NAME} build-${LIB_UTHASH_NAME})