## Introduction

`ocp` is a framework that combines a Python interface with a C framework for control systems. The Python side provides a flexible environment to configure controllers, set parameters, and analyze logged data, while the C side provides the actual controller implementation that can be run either in simulation or deployed directly on hardware.

The framework is organized around three main ideas:

- Interface: a lightweight interface between Python and the controller, which allows Python to configure, run, and query the system. Communication is done through TCP sockets, so the controller can be running either on hardware or in simulation.

- Traces: a flexible logging mechanism to record measurements, control signals, and debug signals directly from the controller. Traces can be configured dynamically, and can also operate in trigger mode, where data recording only starts after a defined event. This makes it possible to capture specific transients and directly compare the response of different controllers or parameter sets.

- Controllers: A small controller library written in C to manage multiple controllers within the same control system. The library takes care of initialization, routing data, and executing the enabled controller. This makes it straightforward to implement and test different control strategies side by side, and to switch between them at run time.

## Getting started

The easiest way to get started is to have a look at the examples provided in the `demos` folder:

- Introductory example (`example_intro`): shows the structure of a minimal application with `ocp`, how to initialize traces and controllers, and how to interact with them from Python.

- Single controller example (`example_controller`): demonstrates a closed-loop simulation of a buck converter, how to design and set controller parameters from Python, and how to log and retrieve data from the controller.

- Multiple controllers example (`example_multiple_controllers`): extends the previous case to multiple controllers, showing how to manage them under the same control system, how to switch between them, and how to compare their performance using the trace in trigger mode.
