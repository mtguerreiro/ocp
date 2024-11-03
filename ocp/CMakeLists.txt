cmake_minimum_required(VERSION 3.12)

add_library(ocp ocpCS.c ocpTrace.c ocpIf.c ocpOpil.c ocpPlatform.c)

target_include_directories(ocp PRIVATE
                           "${PROJECT_SOURCE_DIR}"
)

target_link_libraries(ocp controlsys ctrace rp)


