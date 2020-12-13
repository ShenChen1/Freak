project(vsf-hi3516d)

add_definitions(-DMODULE_NAME="${PROJECT_NAME}")
add_definitions(-DSENSOR0_TYPE=SONY_IMX327_2L_MIPI_2M_30FPS_12BIT)
add_definitions(-DSENSOR1_TYPE=SONY_IMX327_2L_MIPI_2M_30FPS_12BIT)
add_compile_options(-ffunction-sections -fdata-sections)
add_link_options(-Wl,--gc-sections)

add_library(${PROJECT_NAME} STATIC
    src/hal/sys.c
    src/hal/vi.c
    src/hal/vpss.c
    src/hal/venc.c
    src/hal/rgn.c
    src/sdk_cfg.c
    cfg/sdk_cfg.c
    sample/sample_comm_sys.c
    sample/sample_comm_vi.c
    sample/sample_comm_vo.c
    sample/sample_comm_vpss.c
    sample/sample_comm_venc.c
    sample/sample_comm_isp.c
    sample/sample_comm_region.c
    sample/loadbmp.c)

target_link_libraries(${PROJECT_NAME}
    utils
    proto)
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/../vsf/)
include_directories(${FREAK_SYSROOT_PATH}/usr/local/include/mpp)
add_dependencies(${PROJECT_NAME} sdk-${CMAKE_CHIP_NAME})

jsonb(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/cfg/sdk_cfg.jsonb ${PROJECT_SOURCE_DIR}/cfg)