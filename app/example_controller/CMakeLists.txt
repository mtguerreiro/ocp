cmake_minimum_required(VERSION 3.10)

project(ocp_example)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(OCP_PATH "../../../ocp/")
add_subdirectory(${OCP_PATH} "${PROJECT_BINARY_DIR}/ocp")

set(OPIL_PATH "/home/marco/projects/opil/")
add_subdirectory(${OPIL_PATH} "${PROJECT_BINARY_DIR}/opil")

add_subdirectory(threads)
add_subdirectory(app)

add_executable(ocp_example main.c)

target_link_libraries(ocp_example PUBLIC ocp threads app)

target_include_directories(ocp_example PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${OCP_PATH}"
                           "${OPIL_PATH}"
)
