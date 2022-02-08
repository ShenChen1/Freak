project(alg-rv1126)

# SDK
include(sdk-${CMAKE_CHIP_NAME})

add_definitions(-DMODULE_NAME="${PROJECT_NAME}")
add_compile_options(-ffunction-sections -fdata-sections)
add_link_options(-Wl,--gc-sections)

add_library(${PROJECT_NAME} STATIC
    src/hal/face.c)

target_link_libraries(${PROJECT_NAME}
    utils
    proto
    media
    -lstdc++
    -lNceFace
    -lnce_alg
    -lrknn_api
    -lm
    -lsqlite3
    -ldl)
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${FREAK_TOOLS_PATH}/jsonb-c/inc/)
include_directories(${PROJECT_SOURCE_DIR}/../app/)
include_directories(${PROJECT_SOURCE_DIR}/inc/)
add_dependencies(${PROJECT_NAME} sdk-${CMAKE_CHIP_NAME})
