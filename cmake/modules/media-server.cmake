set(LIB_MEDIA_SRV_NAME media-server)

################################################################

include_directories(${MEDIA_SRV_HEADER_DIR})
link_directories(${MEDIA_SRV_LINK_DIR})

add_library(${LIB_MEDIA_SRV_NAME}::flv STATIC IMPORTED)
set_target_properties(${LIB_MEDIA_SRV_NAME}::flv PROPERTIES
    IMPORTED_LOCATION ${MEDIA_SRV_LINK_DIR}/libflv.a)

add_library(${LIB_MEDIA_SRV_NAME}::mov STATIC IMPORTED)
set_target_properties(${LIB_MEDIA_SRV_NAME}::mov PROPERTIES
    IMPORTED_LOCATION ${MEDIA_SRV_LINK_DIR}/libmov.a)

add_library(${LIB_MEDIA_SRV_NAME}::rtmp STATIC IMPORTED)
set_target_properties(${LIB_MEDIA_SRV_NAME}::rtmp PROPERTIES
    IMPORTED_LOCATION ${MEDIA_SRV_LINK_DIR}/librtmp.a)

add_library(${LIB_MEDIA_SRV_NAME}::rtp STATIC IMPORTED)
set_target_properties(${LIB_MEDIA_SRV_NAME}::rtp PROPERTIES
    IMPORTED_LOCATION ${MEDIA_SRV_LINK_DIR}/librtp.a)

add_library(${LIB_MEDIA_SRV_NAME}::rtsp STATIC IMPORTED)
set_target_properties(${LIB_MEDIA_SRV_NAME}::rtsp PROPERTIES
    IMPORTED_LOCATION ${MEDIA_SRV_LINK_DIR}/librtsp.a)

add_library(${LIB_MEDIA_SRV_NAME}::sip STATIC IMPORTED)
set_target_properties(${LIB_MEDIA_SRV_NAME}::sip PROPERTIES
    IMPORTED_LOCATION ${MEDIA_SRV_LINK_DIR}/libsip.a)

################################################################

include_directories(${MEDIA_SRV_SDK_HEADER_DIR})
link_directories(${MEDIA_SRV_SDK_LINK_DIR})
add_link_options(-laio -lutils-media)

add_library(${LIB_MEDIA_SRV_NAME}::http STATIC IMPORTED)
set_target_properties(${LIB_MEDIA_SRV_NAME}::http PROPERTIES
    IMPORTED_LOCATION ${MEDIA_SRV_SDK_LINK_DIR}/libhttp.a)

add_library(${LIB_MEDIA_SRV_NAME}::aio SHARED IMPORTED)
set_target_properties(${LIB_MEDIA_SRV_NAME}::aio PROPERTIES
    IMPORTED_LOCATION ${MEDIA_SRV_SDK_LINK_DIR}/libaio.so)

add_library(${LIB_MEDIA_SRV_NAME}::utils SHARED IMPORTED)
set_target_properties(${LIB_MEDIA_SRV_NAME}::utils PROPERTIES
    IMPORTED_LOCATION ${MEDIA_SRV_SDK_LINK_DIR}/libutils-media.so)

################################################################

add_custom_target(${LIB_MEDIA_SRV_NAME})
add_dependencies(${LIB_MEDIA_SRV_NAME} build-${LIB_MEDIA_SRV_NAME})