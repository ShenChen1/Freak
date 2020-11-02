project(vsf-host)

add_definitions(-DMODULE_NAME="${PROJECT_NAME}")
add_library(${PROJECT_NAME} STATIC
    src/dummy.c)