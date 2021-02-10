project(utils)

add_definitions(-DMODULE_NAME="${PROJECT_NAME}")
add_compile_options(-ffunction-sections -fdata-sections)

add_library(${PROJECT_NAME} STATIC
    json-cfg/src/json-cfg.c
    log/src/log.c
    common/src/que.c
    mm/src/physmap.c
    mm/src/virt2phys.c
    nnm/src/nnm_utils.c
    nnm/src/nnm_pipeline.c
    nnm/src/nnm_reqrep.c
    nnm/src/nnm_pubsub.c)

target_link_libraries(${PROJECT_NAME}
    -l${LIB_CJSON_NAME}
    -l${LIB_NANOMSG_NAME}
    -lrt -lpthread)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/json-cfg/inc
    PRIVATE ${FREAK_TOOLS_PATH}/jsonb-c/inc
    PUBLIC ${PROJECT_SOURCE_DIR}/common/inc
    PUBLIC ${PROJECT_SOURCE_DIR}/log/inc
    PUBLIC ${PROJECT_SOURCE_DIR}/mm/inc
    PUBLIC ${PROJECT_SOURCE_DIR}/nnm/inc)
add_dependencies(${PROJECT_NAME} ${LIB_CJSON_NAME})
add_dependencies(${PROJECT_NAME} ${LIB_UTHASH_NAME})
add_dependencies(${PROJECT_NAME} ${LIB_NANOMSG_NAME})

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})