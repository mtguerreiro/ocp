# Introduction

This example shows how to use `ocp`. 

This is a basic template to create new applications with `ocp`. 

# Initialization

Initializing `ocp` requires initializing its interface, the traces, and the control system. 

Initializing the interface should be done first. The interface does not depend on the application, so its initialization can be done before initializing the application.

Next, the traces should be initialized. Since the traces are predefined in the `ocpConfig.h` file, each application can initialize its own trace. 

Initializing the trace just requires its maximum size and a buffer. To initialize a control system, you need to at least provide the following functions:

- Hardware interface
- Hardware status
- Get measurements
- Update control
- Run control
- Controller interface
- Controller status

These functions can be divided in two types: hardware functions and controller functions.

## File structure 

Thus, one way to organize your application is to create four source files: `main.c`, `app.c`, `appHw.c`, and `appController.c`.

`main.c` contains the `main` function, and can be used to initialize `ocp`'s interface, and the application. `app.c` can be used to initialize and connect everything together, while `appHw.c` and `appController.c` will contain the specific hardware and controller functions.

This file is used to initialize the hardware, the controller, and link them to `ocp`. If you look at the initialization function of `app.c`, you'll see that it first initializes the trace. This is important because during hardware and controller initialization, they might add signals to the trace.

After initializing the trace, the hardware and the controller are initialized. Last, `ocp`'s control system is initialized, which links the hardware and controller functions to `ocp`.

Note that the trace and the control system can be initialized with names. In this template, their are initialized with "App trace" and "App controller", respectively. 

## Running

In this example, all the controller and hardware files are empty. However, it is already possible to interact with your control system.

Build the files and run the target (here, we're assuming you've opened this folder in Visual Studio Code).  

Then, run the Python script `python_ex_template.py` with IDLE within the Python interpreter.

Now, run `ocp.cs_get_number_controllers()` in the interpreter. The result should look like the following
```python
>>> ocp.cs_get_number_controllers()
(0, 1)
```

With this command, we queried our controller about how many controllers it has. We can also query the name of the controllers:
```python
>>> ocp.cs_get_controllers_names()
(0, [b'App controller'])
```
Note that when we initialized our control system in `app.c`, we gave this as the name of the controller.

We can also interact with the trace. For example, we can query the name of the traces in the controller:
```python
>>> ocp.trace_get_traces_names()
(0, [b'App trace'])
```

We can also get and set the size of the trace:
```python
>>> ocp.trace_get_size(0)
(0, 104857600)
>>> ocp.trace_set_size(0, 1024)
(0,) 
>>> ocp.trace_get_size(0)
(0, 1024)
```

Note that to query and set the size of the trace, we have to give the ID of the trace that we want to query. This ID must match the one used to initialize the trace, as we did in `app.c`.

# Notes

- It is assumed that the python package `lrssoc` is installed in your system. This package can be found in the main directory of the repository, inside the `python` folder. To install the package, navigate to the `python` folder with the command prompt, and run
```sh
python -m pip install -e .
``` 

- It is assumed that you are using Visual Studio Code, have CMake installed, and the CMake extension for Visual Studio is also installed.