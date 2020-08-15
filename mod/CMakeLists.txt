include(cjson)
include(nanomsg)

add_compile_options(-g -Os -Wall -Werror)
add_subdirectory(utils)

link_libraries(-lrt -lpthread)
add_subdirectory(bsp)