project(alg-rk1808)

add_definitions(-DMODULE_NAME="${PROJECT_NAME}")
add_library(${PROJECT_NAME} STATIC
    src/dummy.c)
include_directories(${PROJECT_SOURCE_DIR})