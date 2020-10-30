include(cjson)
include(nanomsg)
include(st)
include(mongoose)
include(media-server)
include(uthash)
include(ufifo)

add_compile_options(-g -Os -Wall -Werror)
add_subdirectory(utils)
add_subdirectory(proto)
add_subdirectory(vsf-${CMAKE_CHIP_NAME}) # special vsf

link_libraries(-lrt -lpthread)
add_subdirectory(bsp)
add_subdirectory(vsf)
add_subdirectory(rtsp)