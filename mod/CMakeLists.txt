include(cjson)
include(nanomsg)

add_compile_options(-g -Os -Wall -Werror)
add_subdirectory(utils)
add_subdirectory(proto)

link_libraries(-lrt -lpthread)
add_subdirectory(bsp)