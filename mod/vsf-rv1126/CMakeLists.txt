project(vsf-rv1126)

# SDK
include(sdk-${CMAKE_CHIP_NAME})

add_definitions(-DMODULE_NAME="${PROJECT_NAME}")
#add_definitions(-DSENSOR0_TYPE=SONY_IMX327_2L_MIPI_2M_30FPS_12BIT)
#add_definitions(-DSENSOR1_TYPE=SONY_IMX327_2L_MIPI_2M_30FPS_12BIT)
add_compile_options(-ffunction-sections -fdata-sections)
add_link_options(-Wl,--gc-sections)

add_library(${PROJECT_NAME} STATIC
    src/hal/sys.c
    src/hal/vi.c
    src/hal/venc.c
    src/sdk_cfg.c
    cfg/sdk_cfg.c)

target_link_libraries(${PROJECT_NAME}
    utils
    media)
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/../vsf/)
add_dependencies(${PROJECT_NAME} sdk-${CMAKE_CHIP_NAME})

jsonb(${PROJECT_NAME} sdkcfg ${PROJECT_SOURCE_DIR}/cfg/sdk_cfg.jsonb ${PROJECT_SOURCE_DIR}/cfg)