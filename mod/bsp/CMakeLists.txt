project(bsp)

include(cjson)
include(ufifo)

add_executable(${PROJECT_NAME}
    src/main.c
    cfg/cfg.c)

target_link_libraries(${PROJECT_NAME}
    ${LIB_CJSON_NAME}
    ${LIB_UFIFO_NAME})

add_custom_target(cfg-${PROJECT_NAME}
    COMMAND python ${HIVIEW_TOOLS_PATH}/jsonb-c/src/jsonb_c.py --source=${PROJECT_SOURCE_DIR}/cfg/cfg.jsonb --installdir=${PROJECT_SOURCE_DIR}/cfg)
include_directories(${HIVIEW_TOOLS_PATH}/jsonb-c/inc/)
add_dependencies(${PROJECT_NAME} cfg-${PROJECT_NAME})