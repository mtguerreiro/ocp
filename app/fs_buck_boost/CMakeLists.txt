
add_library(fs_buck_boost)

target_sources(fs_buck_boost
    PRIVATE
    fsbuckboostOpil.c

    fsbuckboostController.c
    fsbuckboostControlIdle.c
    fsbuckboostControlRamp.c
    fsbuckboostControlBuckSfb.c
    fsbuckboostControlBoostEnergy.c

    fsbuckboostHwOpil.c
    fsbuckboostHwIf.c
)

target_link_libraries(fs_buck_boost
    PRIVATE
    ocp
    opil
)

target_include_directories(fs_buck_boost
    PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}"
)

target_include_directories(fs_buck_boost
    PRIVATE
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_BINARY_DIR}/app"
)
