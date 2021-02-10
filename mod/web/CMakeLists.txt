project(web)

add_definitions(-DMODULE_NAME="${PROJECT_NAME}")
add_definitions(-DOS_LINUX)
add_executable(${PROJECT_NAME}
    src/main.c
    src/cfg.c
    src/web/web-http.c
    src/web/web-media.c
    src/msgbox/msgbox.c
    src/msgbox/msgbox_http.c
    src/interface/svr_mgr.c
    cfg/cfg.c)

target_link_libraries(${PROJECT_NAME}
    -l${LIB_MONGOOSE_NAME}
    -l${LIB_MEDIA_SRV_UTILS_NAME}
    -l${LIB_UFIFO_NAME}
    ${LIB_MEDIA_SRV_NAME}::flv
    utils
    proto)
include_directories(${PROJECT_SOURCE_DIR})
add_dependencies(${PROJECT_NAME} ${LIB_MONGOOSE_NAME})
add_dependencies(${PROJECT_NAME} ${LIB_UFIFO_NAME})
add_dependencies(${PROJECT_NAME} ${LIB_MEDIA_SRV_NAME})

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
jsonb(${PROJECT_NAME} cfg ${PROJECT_SOURCE_DIR}/cfg/cfg.jsonb ${PROJECT_SOURCE_DIR}/cfg)