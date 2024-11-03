cmake_minimum_required(VERSION 3.12)

add_library(threads ocpIfThread.c)

target_link_libraries(threads ocp)

target_include_directories(threads PRIVATE
                           "${PROJECT_SOURCE_DIR}"
                           "../${OCP_PATH}"
)
