project(bsp)

include(cjson)
add_executable(${PROJECT_NAME} src/main.c)
target_link_libraries(${PROJECT_NAME}
    ${LIB_CJSON_NAME}
)