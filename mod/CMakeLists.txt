include(cjson)
include(freetype)
include(media-server)
include(mongoose)
include(nanomsg)
include(uthash)
include(ufifo)

# build type, by default to release (with optimisations)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)
    message(STATUS "Setting build type to '${CMAKE_BUILD_TYPE}' as none was specified.")
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()
add_compile_options(-Wall -Werror)

add_subdirectory(utils)
add_subdirectory(proto)
add_subdirectory(media)
add_subdirectory(vsf-${CMAKE_CHIP_NAME}) # special vsf
add_subdirectory(alg-${CMAKE_CHIP_NAME}) # special alg

add_link_options(-Wl,--gc-sections)
add_subdirectory(bsp)
add_subdirectory(vsf)
add_subdirectory(rtsp)
add_subdirectory(web)
add_subdirectory(app)
