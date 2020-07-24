project(bsp)

include(cjson)
include(st)
include(nanomsg)
include(media-server)

add_executable(${PROJECT_NAME} src/main.c)
target_link_libraries(${PROJECT_NAME}
    ${LIB_CJSON_NAME}
    ${LIB_ST_NAME}
    ${LIB_NANOMSG_NAME}
    ${LIB_MEDIA_SRV_NAME})