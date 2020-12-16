project(vsf)

# SDK
include(sdk-${CMAKE_CHIP_NAME})
include(cjson)
include(nanomsg)
include(ufifo)

add_definitions(-DMODULE_NAME="${PROJECT_NAME}")
add_executable(${PROJECT_NAME}
    src/main.c
    src/cfg.c
    src/msgbox/msgbox.c
    src/msgbox/msgbox_vsf_stream.c
    src/msgbox/msgbox_vsf_frame.c
    src/msgbox/msgbox_vsf_osd.c
    src/interface/stream_mgr.c
    src/interface/frame_mgr.c
    src/interface/chn_mgr.c
    src/interface/osd_mgr.c
    src/hal/sys.c
    src/hal/vi.c
    src/hal/vpss.c
    src/hal/venc.c
    src/hal/rgn.c
    cfg/cfg.c)

target_link_libraries(${PROJECT_NAME}
    ${LIB_SDK_NAME}
    -Wl,--whole-archive
    vsf-${CMAKE_CHIP_NAME}
    -Wl,--no-whole-archive
    utils
    proto)
include_directories(${PROJECT_SOURCE_DIR})
add_dependencies(${PROJECT_NAME} vsf-${CMAKE_CHIP_NAME})
add_dependencies(${PROJECT_NAME} ${LIB_UFIFO_NAME})

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
jsonb(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/cfg/cfg.jsonb ${PROJECT_SOURCE_DIR}/cfg)
