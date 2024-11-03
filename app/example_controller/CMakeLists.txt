cmake_minimum_required(VERSION 3.10)

project(ocp_example)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(OCP_PATH "../../../ocp/")
add_subdirectory(${OCP_PATH} "${PROJECT_BINARY_DIR}/ocp")
add_subdirectory(threads)

add_executable(ocp_example main.c app.c appController.c appHw.c)

target_link_libraries(ocp_example PUBLIC ocp threads)

target_include_directories(ocp_example PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${OCP_PATH}"
)
