cmake_minimum_required(VERSION 3.18)

project(ocp_example)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(OCP_PATH "../../../ocp/")
add_subdirectory(${OCP_PATH} "${PROJECT_BINARY_DIR}/ocp")

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_executable(ocp_example main.c app.c appController.c appHw.c ocpIfLinuxThread.c)
    target_link_libraries(ocp_example PUBLIC ocp)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_executable(ocp_example main.c app.c appController.c appHw.c ocpIfWinThread.c)
    target_link_libraries(ocp_example PUBLIC ocp ws2_32)
else ()
    message("-- THREADS: platform ${CMAKE_SYSTEM_NAME} not supported for this example.")
endif ()

target_link_libraries(
    ocp_example
    PRIVATE
    ocp
)

