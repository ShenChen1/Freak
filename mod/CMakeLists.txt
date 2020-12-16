add_compile_options(-g -Os -Wall -Werror)
add_subdirectory(utils)
add_subdirectory(proto)
add_subdirectory(vsf-${CMAKE_CHIP_NAME}) # special vsf

link_libraries(-lrt -lpthread)
add_subdirectory(bsp)
add_subdirectory(vsf)
add_subdirectory(rtsp)
add_subdirectory(web)