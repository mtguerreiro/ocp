# Introduction

This example shows how to use `ocp` to manage several controllers under the same control system. The example demonstrates how to implement e compare the performance of a cascaded and a state feedback controler for the buck converter.

In this example, we use Python to set the parameters of the controllers, to enable the controllers, and to switch between them.  The main goal of this example is to show some of the flexibility of combining Python and the controllers running C code. 

In the previous example (`example_controller`), we implemented a state feedback controller, and configure its parameters through Python. Moreover, we were able to retrieve the trace data recorded by the controller and check the transient response of the output voltage. 

However, as the last example shows, it is necessary to run execute many steps before we can get the system running, such as packing and unpacking binary data, figuring out the size of the trace in bytes, etc. Here, we show how to build a Python module tailored for an application such that we don't have to deal with the low-level `ocp` functions. Furthermore, we also provide a structure to handle multiple controllers in the application.

Before going over this example, it is recommend to have a look at the single controller example (`example_controller`). 


# File structure



When there are multiple controllers, the number of files can start increasi
Now that there are multiple controllers, the number of files 

This example is structured in the following way:

- `main.c:` This file contains the `main` function, used to initialize `ocp`, the controller, and to interface with the simulation software.
- `ocpConfig.h`: Defines the number of traces and control systems in the system.
- `stypes.h`: Configuration file for the simulation, containing a description of the input and output signals.
- `buck` folder: contains a Python module to handle the low-level interaction with `ocp` and provide us with a higher level interface to the controller.
- `app` folder: contains the source files for the controller.
- `plecs` folder: contains the simulation of the converter.
- `python_ex_controllers.py`: Python script for this example.

### `app` folder

The `app` folder contains the source files for the controller, and differs significantly for the previous two examples (`example_intro`, `example_controller`).

Now, we wish to implement multiple controllers, so we need a way to manage them, i.e. initialize them, individually set their parameters, disable, enable, etc. We'll manage this by using the `controller` C library included in `ocp`. The `controller` library will take care of initializing all the controllers, routing their data, and running them if they are enabled. The controllers of the application are defined in the `appController.c` file as an `enum` type, like so:
```c
typedef enum{
    APP_CONTROLLER_SFB,
    APP_CONTROLLER_CASCADED,
    APP_CONTROLLER_END
}appControllersEnum_t;
```
Each controller in the application needs to follow a template that is compatible with the `controller` library. For this example, two controllers are implemented:
- `appControllerSfb.c`
- `appControllerCascaded.c`
In the initialization of `appController.c`, these controllers are included in a callback table that is passed to the `controller` library:
```c
    controllerGetCbs_t ctlGetCbs[APP_CONTROLLER_END] = {0};
    ctlGetCbs[APP_CONTROLLER_SFB] = appControlSfbGetCallbacks;
    ctlGetCbs[APP_CONTROLLER_CASCADED] = appControlCascadedGetCallbacks;
```

Adding new controllers requires adding a new entry in the controllers `enum` type, and including the appropriate callback in the callback table, as shown in the snippets above.

### `buck` folder

The `buck` folder contains a Python module intended to provide a high-level interface between the user and the controller. Just as in the `example_controller` case, we define some functions where we can pass our desired control specifications (e.g. settling time and overshoot), and the function will determine the proper gains, convert them to binary data, and send it to the controller. We will also implement functions where we just define the number of samples we'd like the trace to have, and the trace will be set with the proper size in bytes, as well as getting the trace data already in our desired format.

We start by having a look at the  `controllers.py` file. This file defines a `Reference` class, which we use to set the setpoints of the controllers. This is based on a template provided by `pyocp`, and we just overwrite two methods: `_decode` and `_encode`, where we specify how the reference is interpreted. Since we just have one reference (the output voltage) as a floating point value, the encoding and decoding is simple.

Next, we define a `Controllers` class that includes the controllers we have available. The class is defined as:
```python
class Controllers:

    def __init__(self, ctl_if):

        self.sfb = _SFB(0, ctl_if)
        self.cascaded = _Cascaded(1, ctl_if)
```
Note that we create an object for each controller, and we give their corresponding ID. This ID is the corresponding number in the `enum` type defined back in the `appController.c` file, in the `app` folder.

The definition of the controllers come next. We'll just focus on the `_Cascaded` class for the cascaded controller. In the initialization of the class, it is important to provide the parameters of the controller in a `keys` tuple. They have to be in the same order as they are expected to be received by the controller. Then, the `_decode` and `_encode` methods will take care of packing and unpacking the data in binary format. As in the `example_controller` example, we define a `set_gains` method that takes the desired controller response as parameters, gets the gains and sends it to the controllers. Note that for this, a `set_params` method is used, which is defined in the `pyocp.controller.ControllerTemplate` class. The method takes a dictionary as argument, and packs the data in binary format and sends it to the controller. 

One thing to note is that the C controller always receives all parameters simultaneously. This means that, if the controller has 10 parameters but we only need to change 1, we have to write all 10 parameters together. This is embedded in the `set_params` method of the `pyocp.controller.ControllerTemplate` class. This method will first read and decode the data in the controller, update the fields with the new values passed to it as a dictionary, and then send all parameters to the controller again. In the template, it is assumed that all data in the controller is in floating point. If not the case, we can just overwrite the `set_params` method with a custom implementation.

Next, we have a look at the `trace.py` file. In this file, a class `Trace` is defined which uses the `pyocp.trace.TraceTemplate` class as template. Most of the trace functionalities are similar. The implementation assumes only floating point signals in the trace, and takes care of decoding the trace data and setting the appropriate size according to the number of samples we would like to record.

Last, we have an `iface.py` file, which is used to actually implement the interface with the controller. This class establishes the `pyocp` interface and contains the `Controllers`, `Reference`, and `trace` objects, which will be used to interface with the controller. The class also defines two methods, wich are used to enable and disable the controller.

# Running the example

This example uses PLECS to simulate a converter, and uses `opil` (get it [here](https://gitlab.rhrk.uni-kl.de/lrs/opil)) to interface PLECS with the controller. It is assumed that you have the `ocp` and `opil` folders in the same location, like so:

```
top_level_folder
└───ocp
│   │   CMakeLists.txt
│   │   .gitignore
│   └───app
│   └───controlsys
│       ....
└───opil
    │   CMakeLists.txt
    │   .gitignore
    │   opilhost.c
    │   ...
    └───comm
    └───ctlrif
        ....
```

## Starting the controller and interacting with it


