cmake_minimum_required(VERSION 3.15)

add_library(app app.c appController.c appHw.c appOpil.c appControllerSfb.c appControllerCascaded.c)

target_link_libraries(app ocp opil controller)

target_include_directories(app PRIVATE
                           "${PROJECT_SOURCE_DIR}"
                           "../${OCP_PATH}"
                           "${OPIL_PATH}"
)
