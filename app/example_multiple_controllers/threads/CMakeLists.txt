cmake_minimum_required(VERSION 3.12)

add_library(threads ocpIfThread.c ocpOpilThread.c)

target_link_libraries(threads ocp opil opil_comm opil_ctlrif)

target_include_directories(threads PRIVATE
                           "${PROJECT_SOURCE_DIR}"
                           "../${OCP_PATH}"
                           "../${OPIL_PATH}"
)
