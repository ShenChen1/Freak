project(proto)

add_definitions(-DMODULE_NAME="${PROJECT_NAME}")
add_compile_options(-ffunction-sections -fdata-sections)
add_link_options(-Wl,--gc-sections)

add_library(${PROJECT_NAME} SHARED
    jsonb/proto_base.c
    jsonb/proto_bsp.c
    jsonb/proto_rtsp.c
    jsonb/proto_vsf.c
    jsonb/proto_web.c
    src/dummy.c)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/inc
    PUBLIC ${PROJECT_SOURCE_DIR}/media
    PUBLIC ${PROJECT_SOURCE_DIR}/jsonb)
add_dependencies(${PROJECT_NAME} ${LIB_CJSON_NAME})

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})
jsonb(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/jsonb/proto_base.jsonb ${PROJECT_SOURCE_DIR}/jsonb)
jsonb(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/jsonb/proto_bsp.jsonb ${PROJECT_SOURCE_DIR}/jsonb)
jsonb(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/jsonb/proto_rtsp.jsonb ${PROJECT_SOURCE_DIR}/jsonb)
jsonb(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/jsonb/proto_vsf.jsonb ${PROJECT_SOURCE_DIR}/jsonb)
jsonb(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/jsonb/proto_web.jsonb ${PROJECT_SOURCE_DIR}/jsonb)