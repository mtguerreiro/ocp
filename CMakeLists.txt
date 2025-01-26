cmake_minimum_required(VERSION 3.18)

add_library(ocp)

# OCP core
target_sources(ocp
    PRIVATE
    ocp/ocpCS.c
    ocp/ocpTrace.c 
    ocp/ocpIf.c 
    ocp/ocpOpil.c 
    ocp/ocpPlatform.c
)

# Trace
target_sources(ocp
    PRIVATE
    trace/ctrace.c
    trace/ctrace_circBuf.c 
    trace/ctrace_mem.c
)

# RP
target_sources(ocp
    PRIVATE
    rp/rp.c
)

# Control sys
target_sources(ocp
    PRIVATE
    controlsys/controlsys.c
)

# Controller
target_sources(ocp
    PRIVATE
    controller/controller.c
    controller/controllerIf.c
)

target_include_directories(ocp
    PRIVATE
    ${PROJECT_SOURCE_DIR}/
    ./
)

target_include_directories(ocp
    PUBLIC
    ./
)

