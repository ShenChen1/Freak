include(cjson)
include(media-server)
include(mongoose)
include(nanomsg)
include(uthash)
include(ufifo)

add_compile_options(-g -Os -Wall -Werror)
add_subdirectory(utils)
add_subdirectory(proto)
add_subdirectory(vsf-${CMAKE_CHIP_NAME}) # special vsf
add_subdirectory(alg-${CMAKE_CHIP_NAME}) # special alg

add_link_options(-Wl,--gc-sections)
add_subdirectory(bsp)
add_subdirectory(vsf)
add_subdirectory(rtsp)
add_subdirectory(web)
add_subdirectory(app)
