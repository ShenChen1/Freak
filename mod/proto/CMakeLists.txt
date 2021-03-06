project(proto)

add_definitions(-DMODULE_NAME="${PROJECT_NAME}")
add_compile_options(-ffunction-sections -fdata-sections)

add_library(${PROJECT_NAME} STATIC
    jsonb/proto_app.c
    jsonb/proto_base.c
    jsonb/proto_bsp.c
    jsonb/proto_hdr.c
    jsonb/proto_rtsp.c
    jsonb/proto_vsf.c
    jsonb/proto_web.c
    interface/bsp/dummy.c
    interface/vsf/chn_mgr.c
    interface/vsf/frame_mgr.c
    interface/vsf/osd_mgr.c
    interface/vsf/stream_mgr.c
    interface/rtsp/svr_mgr.c
    interface/web/svr_mgr.c
    interface/app/alg_mgr.c
    msgbox/msgbox.c)

target_link_libraries(${PROJECT_NAME}
    utils)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/inc
    PUBLIC ${PROJECT_SOURCE_DIR}/jsonb
    PUBLIC ${PROJECT_SOURCE_DIR}/interface)

jsonb(${PROJECT_NAME} base ${PROJECT_SOURCE_DIR}/jsonb/proto_base.jsonb ${PROJECT_SOURCE_DIR}/jsonb)
jsonb(${PROJECT_NAME} hdr ${PROJECT_SOURCE_DIR}/jsonb/proto_hdr.jsonb ${PROJECT_SOURCE_DIR}/jsonb)
jsonb(${PROJECT_NAME} bsp ${PROJECT_SOURCE_DIR}/jsonb/proto_bsp.jsonb ${PROJECT_SOURCE_DIR}/jsonb)
jsonb(${PROJECT_NAME} rtsp ${PROJECT_SOURCE_DIR}/jsonb/proto_rtsp.jsonb ${PROJECT_SOURCE_DIR}/jsonb)
jsonb(${PROJECT_NAME} vsf ${PROJECT_SOURCE_DIR}/jsonb/proto_vsf.jsonb ${PROJECT_SOURCE_DIR}/jsonb)
jsonb(${PROJECT_NAME} web ${PROJECT_SOURCE_DIR}/jsonb/proto_web.jsonb ${PROJECT_SOURCE_DIR}/jsonb)
jsonb(${PROJECT_NAME} app ${PROJECT_SOURCE_DIR}/jsonb/proto_app.jsonb ${PROJECT_SOURCE_DIR}/jsonb)
