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

# Utils
target_sources(ocp
    PRIVATE
    utils/dfilt.c
)

# IPC
target_sources(ocp
    PRIVATE
    ipc/ipcClient.c
)

# Zynq
target_sources(ocp
    PRIVATE
    cpu0/tasks/blink.c
    cpu0/tasks/sysinit.c
    cpu0/tasks/uiface.c
    hardware/zynq/opilZynqCpu0.c
    hardware/zynq/ocpZynqCpu0.c
    hardware/zynq/ipcClientZynq.c
)

target_include_directories(ocp
    PRIVATE
    ${PROJECT_SOURCE_DIR}/
    ./
    cpu0/
    cpu0/tasks/
    hardware/zynq/
)

target_include_directories(ocp
    PUBLIC
    ./
    cpu0/tasks/
    hardware/zynq/
)

