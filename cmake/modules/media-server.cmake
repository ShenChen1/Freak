set(LIB_MEDIA_SRV_NAME media-server)

add_library(${LIB_MEDIA_SRV_NAME}::flv STATIC IMPORTED)
set_target_properties(${LIB_MEDIA_SRV_NAME}::flv PROPERTIES
    IMPORTED_LOCATION ${HIVIEW_SYSROOT_PATH}/usr/local/lib/libflv.a)

add_library(${LIB_MEDIA_SRV_NAME}::mov STATIC IMPORTED)
set_target_properties(${LIB_MEDIA_SRV_NAME}::mov PROPERTIES
    IMPORTED_LOCATION ${HIVIEW_SYSROOT_PATH}/usr/local/lib/libmov.a)

add_library(${LIB_MEDIA_SRV_NAME}::rtmp STATIC IMPORTED)
set_target_properties(${LIB_MEDIA_SRV_NAME}::rtmp PROPERTIES
    IMPORTED_LOCATION ${HIVIEW_SYSROOT_PATH}/usr/local/lib/librtmp.a)

add_library(${LIB_MEDIA_SRV_NAME}::rtp STATIC IMPORTED)
set_target_properties(${LIB_MEDIA_SRV_NAME}::rtp PROPERTIES
    IMPORTED_LOCATION ${HIVIEW_SYSROOT_PATH}/usr/local/lib/librtp.a)

add_library(${LIB_MEDIA_SRV_NAME}::rtsp STATIC IMPORTED)
set_target_properties(${LIB_MEDIA_SRV_NAME}::rtsp PROPERTIES
    IMPORTED_LOCATION ${HIVIEW_SYSROOT_PATH}/usr/local/lib/librtsp.a)

add_library(${LIB_MEDIA_SRV_NAME}::sip STATIC IMPORTED)
set_target_properties(${LIB_MEDIA_SRV_NAME}::sip PROPERTIES
    IMPORTED_LOCATION ${HIVIEW_SYSROOT_PATH}/usr/local/lib/libsip.a)

add_library(${LIB_MEDIA_SRV_NAME}::http STATIC IMPORTED)
set_target_properties(${LIB_MEDIA_SRV_NAME}::http PROPERTIES
    IMPORTED_LOCATION ${HIVIEW_SYSROOT_PATH}/usr/local/lib/libhttp.a)

add_library(${LIB_MEDIA_SRV_NAME}::aio SHARED IMPORTED)
set_target_properties(${LIB_MEDIA_SRV_NAME}::aio PROPERTIES
    IMPORTED_LOCATION ${HIVIEW_SYSROOT_PATH}/usr/local/lib/libaio.so)

add_library(${LIB_MEDIA_SRV_NAME}::utils SHARED IMPORTED)
set_target_properties(${LIB_MEDIA_SRV_NAME}::utils PROPERTIES
    IMPORTED_LOCATION ${HIVIEW_SYSROOT_PATH}/usr/local/lib/libutils.so)

add_custom_target(${LIB_MEDIA_SRV_NAME})
include_directories(${HIVIEW_SYSROOT_PATH}/usr/local/include/media-server)
add_dependencies(${LIB_MEDIA_SRV_NAME} build-${LIB_MEDIA_SRV_NAME})
