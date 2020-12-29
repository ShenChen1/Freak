project(app)
include(sdk-${CMAKE_CHIP_NAME})
add_definitions(-DMODULE_NAME="${PROJECT_NAME}")
add_executable(${PROJECT_NAME}
    src/main.c
    src/cfg.c
    src/msgbox/msgbox.c
    src/msgbox/msgbox_alg.c
    src/hal/appalg.c
    src/hal/hsFace.c
    src/interface/alg_mgr.c
    cfg/cfg.c)

target_link_libraries(${PROJECT_NAME}
    -l${LIB_CJSON_NAME}
    -l${LIB_NANOMSG_NAME}
    -l${LIB_UFIFO_NAME}
    utils
    proto
    ${FREAK_HOME_PATH}/sdk/${CMAKE_CHIP_NAME}/libnnieface.so
    ${FREAK_HOME_PATH}/sdk/${CMAKE_CHIP_NAME}/libtracker.so
    -lstdc++)
include_directories(${PROJECT_SOURCE_DIR})

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
jsonb(${PROJECT_NAME} cfg ${PROJECT_SOURCE_DIR}/cfg/cfg.jsonb ${PROJECT_SOURCE_DIR}/cfg)
