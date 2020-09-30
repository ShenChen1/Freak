project(rtsp)

add_definitions(-DMODULE_NAME="${PROJECT_NAME}")
add_definitions(-DOS_LINUX)
add_executable(${PROJECT_NAME}
    src/main.c
    src/cfg.c
    src/rtsp-ctrl.c
    src/msgbox/msgbox.c
    src/msgbox/msgbox_rtsp.c
    src/rtsp/rtp-media.c
    src/rtsp/rtp-receiver-tcp.c
    src/rtsp/rtp-receiver-udp.c
    src/rtsp/rtp-transport-tcp.c
    src/rtsp/rtp-transport-udp.c
    src/rtsp/rtsp-server.c
    src/rtsp/rtsp-client.c
    cfg/cfg.c)

target_link_libraries(${PROJECT_NAME}
    ${LIB_CJSON_NAME}
    ${LIB_NANOMSG_NAME}
    ${LIB_ST_NAME}
    ${LIB_UFIFO_NAME}
    ${LIB_MEDIA_SRV_NAME}::rtsp
    ${LIB_MEDIA_SRV_NAME}::http
    ${LIB_MEDIA_SRV_NAME}::rtp
    ${LIB_MEDIA_SRV_NAME}::aio
    ${LIB_MEDIA_SRV_NAME}::utils
    utils
    proto)
include_directories(${PROJECT_SOURCE_DIR})
add_dependencies(${PROJECT_NAME} ${LIB_UTHASH_NAME})
add_dependencies(${PROJECT_NAME} ${LIB_MEDIA_SRV_NAME})

add_custom_target(cfg-${PROJECT_NAME}
    COMMAND python ${FREAK_TOOLS_PATH}/jsonb-c/src/jsonb_c.py --source=${PROJECT_SOURCE_DIR}/cfg/cfg.jsonb --installdir=${PROJECT_SOURCE_DIR}/cfg)
include_directories(${FREAK_TOOLS_PATH}/jsonb-c/inc/)
add_dependencies(${PROJECT_NAME} cfg-${PROJECT_NAME})