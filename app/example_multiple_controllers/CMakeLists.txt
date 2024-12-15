cmake_minimum_required(VERSION 3.15)

project(ocp_example)

set(OCP_PATH "../../../ocp/")
set(OPIL_PATH "../../../opil/")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
        -O1
        )

add_subdirectory(${OCP_PATH} "${PROJECT_BINARY_DIR}/ocp")
add_subdirectory(${OPIL_PATH} "${PROJECT_BINARY_DIR}/opil")

add_subdirectory(threads)
add_subdirectory(app)

add_executable(ocp_example main.c)

target_link_libraries(ocp_example PUBLIC opil ocp threads app controller)

target_include_directories(ocp_example PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${OCP_PATH}"
                           "${OPIL_PATH}"
)
