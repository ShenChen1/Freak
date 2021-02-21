project(vsf)

# SDK
include(sdk-${CMAKE_CHIP_NAME})

add_definitions(-DMODULE_NAME="${PROJECT_NAME}")
add_executable(${PROJECT_NAME}
    src/main.c
    src/cfg.c
    src/msgbox/msgbox.c
    src/msgbox/msgbox_stream.c
    src/msgbox/msgbox_frame.c
    src/msgbox/msgbox_osd.c
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
    -l${LIB_UFIFO_NAME}
    -l${LIB_FREETYPE_NAME}
    ${LIB_VSF_SDK_NAME}
    -Wl,--whole-archive
    vsf-${CMAKE_CHIP_NAME}
    -Wl,--no-whole-archive
    utils
    proto)
include_directories(${PROJECT_SOURCE_DIR})
add_dependencies(${PROJECT_NAME} vsf-${CMAKE_CHIP_NAME})
add_dependencies(${PROJECT_NAME} ${LIB_UFIFO_NAME})
add_dependencies(${PROJECT_NAME} ${LIB_FREETYPE_NAME})

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
jsonb(${PROJECT_NAME} cfg ${PROJECT_SOURCE_DIR}/cfg/cfg.jsonb ${PROJECT_SOURCE_DIR}/cfg)
