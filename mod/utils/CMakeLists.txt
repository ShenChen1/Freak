project(utils)

add_compile_options(-ffunction-sections -fdata-sections)
add_link_options(-Wl,--gc-sections)

add_library(${PROJECT_NAME} SHARED
    json-cfg/src/json-cfg.c)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/json-cfg/inc)

include_directories(${HIVIEW_TOOLS_PATH}/jsonb-c/inc/)
add_dependencies(${PROJECT_NAME} ${LIB_CJSON_NAME})