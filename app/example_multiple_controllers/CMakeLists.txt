cmake_minimum_required(VERSION 3.18)

project(ocp_example)

set(OCP_PATH "../../../ocp/")
set(OPIL_PATH "../../../opil/")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

add_compile_options(
    -Wall
    -Wextra
    -Wno-unused-parameter
    -O2
)

add_subdirectory(${OCP_PATH} "${PROJECT_BINARY_DIR}/ocp")
add_subdirectory(${OPIL_PATH} "${PROJECT_BINARY_DIR}/opil")

add_subdirectory(threads)
add_subdirectory(app)

add_executable(ocp_example main.c)

target_link_libraries(ocp_example
    PRIVATE
    opil
    ocp
    threads
    app
)
