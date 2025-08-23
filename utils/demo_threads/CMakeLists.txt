
add_library(ocp_demo_threads)

if( OCP_DEMO_THREADS_OPIL )
    target_sources(
        ocp_demo_threads
        PRIVATE
        ocpOpilThread.c
    )
    target_link_libraries(
        ocp_demo_threads
        PRIVATE
        opil
    )
endif()

target_link_libraries(
    ocp_demo_threads
    PRIVATE
    ocp
)

if(OCP_DEMO_THREADS_OCP)
    if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
        target_sources(ocp_demo_threads PRIVATE ocpIfLinuxThread.c)
    elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
        target_sources(ocp_demo_threads PRIVATE ocpIfWinThread.c)
        target_link_libraries(ocp_demo_threads PRIVATE ws2_32)
    else ()
        message("-- THREADS: platform ${CMAKE_SYSTEM_NAME} not supported for this example.")
    endif ()
endif()

target_include_directories(
    ocp_demo_threads
    PRIVATE
    "${PROJECT_SOURCE_DIR}"
)
