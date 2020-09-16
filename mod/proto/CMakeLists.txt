project(proto)

add_definitions(-DMODULE_NAME="${PROJECT_NAME}")
add_compile_options(-ffunction-sections -fdata-sections)
add_link_options(-Wl,--gc-sections)

add_library(${PROJECT_NAME} STATIC
    jsonb/proto_bsp.c
    jsonb/proto_rtsp.c
    src/dummy.c)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/inc
    PUBLIC ${PROJECT_SOURCE_DIR}/jsonb)

add_dependencies(${PROJECT_NAME} ${LIB_CJSON_NAME})

add_custom_target(cfg-${PROJECT_NAME}
    COMMAND python ${FREAK_TOOLS_PATH}/jsonb-c/src/jsonb_c.py --source=${PROJECT_SOURCE_DIR}/jsonb/proto_bsp.jsonb --installdir=${PROJECT_SOURCE_DIR}/jsonb
    COMMAND python ${FREAK_TOOLS_PATH}/jsonb-c/src/jsonb_c.py --source=${PROJECT_SOURCE_DIR}/jsonb/proto_rtsp.jsonb --installdir=${PROJECT_SOURCE_DIR}/jsonb)
include_directories(${FREAK_TOOLS_PATH}/jsonb-c/inc/)
add_dependencies(${PROJECT_NAME} cfg-${PROJECT_NAME})