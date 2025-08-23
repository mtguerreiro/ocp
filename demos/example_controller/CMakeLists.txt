cmake_minimum_required(VERSION 3.18)

project(ocp_example)

set(OCP_PATH "../../../ocp/")

set(OPIL_PATH "../../../opil/")
set(OPIL_TARGET_IP "127.0.0.1" CACHE STRING "IP of the OPiL target")
set(OPIL_TARGET_PORT 8090 CACHE STRING "Port opened by the OPiL target")

set(OCP_DEMO_THREADS_OCP  ON)
set(OCP_DEMO_THREADS_OPIL ON)

set(PLECS_BUILD_DIR "${CMAKE_SOURCE_DIR}/plecs/build/")
add_custom_target(build_plecs ALL
    COMMAND make -f "${OPIL_PATH}/sw/plecs/make_opil_plecs.mk" BUILD_DIR="${PLECS_BUILD_DIR}" OPIL="${OPIL_PATH}" HOST_COMM_SOCK_SERVER_IP="${OPIL_TARGET_IP}" HOST_COMM_SOCK_SERVER_PORT=${OPIL_TARGET_PORT} STYPES_DIR="${PROJECT_SOURCE_DIR}/"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Building OPiL/PLECS DLL using external Makefile"
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

add_compile_options( -Wall -Wextra -O2 )

add_subdirectory(${OCP_PATH} "${PROJECT_BINARY_DIR}/ocp")
add_subdirectory(${OPIL_PATH} "${PROJECT_BINARY_DIR}/opil")
target_compile_definitions(
    opil
    PRIVATE
    TARGET_COMM_SOCK_SERVER_PORT=${OPIL_TARGET_PORT}
)

add_subdirectory(app)

add_executable(ocp_example main.c)

target_link_libraries(ocp_example
    PRIVATE
    opil
    ocp
    ocp_demo_threads
    app
)

