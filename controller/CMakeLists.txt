cmake_minimum_required(VERSION 3.12)

add_library(controller controller.c controllerIf.c)

target_include_directories(controller PRIVATE
                           "${PROJECT_SOURCE_DIR}"
)

target_link_libraries(controller rp)