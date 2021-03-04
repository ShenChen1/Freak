project(media)

add_definitions(-DMODULE_NAME="${PROJECT_NAME}")
add_compile_options(-ffunction-sections -fdata-sections)

add_library(${PROJECT_NAME} STATIC
    src/media.c
    src/video_frame.c
    src/video_stream.c)

target_link_libraries(${PROJECT_NAME}
    -l${LIB_UFIFO_NAME}
    -lrt
    utils)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/inc)
add_dependencies(${PROJECT_NAME} ${LIB_UFIFO_NAME})