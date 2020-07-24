set(LIB_MEDIA_SRV_NAME media-server)

add_library(${LIB_MEDIA_SRV_NAME} STATIC IMPORTED)
set_target_properties(${LIB_MEDIA_SRV_NAME} PROPERTIES
    IMPORTED_LOCATION
        ${HIVIEW_SYSROOT_PATH}/usr/local/lib/libflv.so
        ${HIVIEW_SYSROOT_PATH}/usr/local/lib/libmov.so
        ${HIVIEW_SYSROOT_PATH}/usr/local/lib/librtmp.so
        ${HIVIEW_SYSROOT_PATH}/usr/local/lib/librtp.so
        ${HIVIEW_SYSROOT_PATH}/usr/local/lib/librtsp.so
        ${HIVIEW_SYSROOT_PATH}/usr/local/lib/libsip.so
        ${HIVIEW_SYSROOT_PATH}/usr/local/lib/libhttp.so)
include_directories(${HIVIEW_SYSROOT_PATH}/usr/local/include/media-server)
add_dependencies(${LIB_MEDIA_SRV_NAME} build-${LIB_MEDIA_SRV_NAME})
