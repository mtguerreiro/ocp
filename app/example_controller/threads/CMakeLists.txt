cmake_minimum_required(VERSION 3.12)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_library(threads ocpIfLinuxThread.c ocpOpilThread.c)
    target_link_libraries(threads ocp opil opil_comm opil_ctlrif)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_library(threads ocpIfWinThread.c ocpOpilThread.c)
    target_link_libraries(threads ocp opil opil_comm opil_ctlrif ws2_32)
else ()
    message("-- THREADS: platform ${CMAKE_SYSTEM_NAME} not supported for this example.")
endif ()

target_include_directories(threads PRIVATE
                           "${PROJECT_SOURCE_DIR}"
                           "../${OCP_PATH}"
                           "../${OPIL_PATH}"
)
