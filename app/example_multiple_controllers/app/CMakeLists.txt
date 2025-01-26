
add_library(app)

target_sources(app
    PRIVATE
    app.c 
    appController.c
    appControllerCascaded.c
    appControllerSfb.c
    appHw.c
    appOpil.c
)

target_link_libraries(app
    PRIVATE
    ocp
    opil
)

target_include_directories(app
    PRIVATE
    "${PROJECT_SOURCE_DIR}"
)
