project(rtsp)

add_definitions(-DMODULE_NAME="${PROJECT_NAME}")
add_definitions(-DOS_LINUX)
add_executable(${PROJECT_NAME}
    src/main.c
    src/cfg.c
    src/msgbox/msgbox.c
    src/msgbox/msgbox_rtsp.c
    src/rtsp/rtp-media.c
    src/rtsp/rtp-receiver-tcp.c
    src/rtsp/rtp-receiver-udp.c
    src/rtsp/rtp-transport-tcp.c
    src/rtsp/rtp-transport-udp.c
    src/rtsp/rtsp-server.c
    src/interface/svr_mgr.c
    cfg/cfg.c)

target_link_libraries(${PROJECT_NAME}
    -l${LIB_MEDIA_SRV_AIO_NAME}
    -l${LIB_MEDIA_SRV_UTILS_NAME}
    ${LIB_MEDIA_SRV_NAME}::rtsp
    ${LIB_MEDIA_SRV_NAME}::http
    ${LIB_MEDIA_SRV_NAME}::rtp
    utils
    proto
    media)
include_directories(${PROJECT_SOURCE_DIR})
add_dependencies(${PROJECT_NAME} ${LIB_MEDIA_SRV_NAME})

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
jsonb(${PROJECT_NAME} cfg ${PROJECT_SOURCE_DIR}/cfg/cfg.jsonb ${PROJECT_SOURCE_DIR}/cfg)