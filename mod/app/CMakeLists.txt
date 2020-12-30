project(app)

# SDK
include(sdk-${CMAKE_CHIP_NAME})

add_definitions(-DMODULE_NAME="${PROJECT_NAME}")
add_executable(${PROJECT_NAME}
    src/main.c
    src/cfg.c
    src/msgbox/msgbox.c
    src/msgbox/msgbox_alg.c
    src/interface/alg_mgr.c
    cfg/cfg.c)

target_link_libraries(${PROJECT_NAME}
    -l${LIB_CJSON_NAME}
    -l${LIB_NANOMSG_NAME}
    -l${LIB_UFIFO_NAME}
    ${LIB_SDK_NAME}
    utils
    proto
    -lstdc++)
include_directories(${PROJECT_SOURCE_DIR})
add_dependencies(${PROJECT_NAME} ${LIB_UFIFO_NAME})

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
jsonb(${PROJECT_NAME} cfg ${PROJECT_SOURCE_DIR}/cfg/cfg.jsonb ${PROJECT_SOURCE_DIR}/cfg)
