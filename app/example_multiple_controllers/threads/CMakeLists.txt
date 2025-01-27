
add_library(threads)

target_sources(threads
    PRIVATE
    ocpOpilThread.c
)

target_link_libraries(threads
    PRIVATE
    ocp
    opil
)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_sources(threads PRIVATE ocpIfLinuxThread.c)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_sources(threads PRIVATE ocpIfWinThread.c)
    target_link_libraries(threads PRIVATE ws2_32)
else ()
    message("-- THREADS: platform ${CMAKE_SYSTEM_NAME} not supported for this example.")
endif ()

target_include_directories(threads
    PRIVATE
    "${PROJECT_SOURCE_DIR}"
)
