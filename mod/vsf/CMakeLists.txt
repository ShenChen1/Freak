project(vsf)

add_definitions(-DMODULE_NAME="${PROJECT_NAME}")
add_executable(${PROJECT_NAME}
    src/main.c
    src/cfg.c
    src/msgbox/msgbox.c
    src/msgbox/msgbox_vsf.c
    src/hal/vi.c
    src/hal/vpss.c
    src/hal/venc.c
    cfg/cfg.c)

target_link_libraries(${PROJECT_NAME}
    ${LIB_CJSON_NAME}
    ${LIB_NANOMSG_NAME}
    utils
    proto)
include_directories(${PROJECT_SOURCE_DIR})

add_custom_target(cfg-${PROJECT_NAME}
    COMMAND python ${FREAK_TOOLS_PATH}/jsonb-c/src/jsonb_c.py --source=${PROJECT_SOURCE_DIR}/cfg/cfg.jsonb --installdir=${PROJECT_SOURCE_DIR}/cfg)
include_directories(${FREAK_TOOLS_PATH}/jsonb-c/inc/)
add_dependencies(${PROJECT_NAME} cfg-${PROJECT_NAME})