# Introduction

This example shows how to use `ocp` for closed-loop control and data logging. To demonstrate closed-loop control, a buck converter is simulated with a software, which exchanges measurements and control signals with the controller. 

In this example, we use Python to set the parameters of the controller (for example, gains and reference signal), to enable the controller, and retrieve logged data. The main goal of this example is to show some of the interesting possibilities when using Python to interact with the C controller. Refer to the `example_multiple_controllers` for an example handling multiple controllers under the same control system.

Before going over this example, it is recommend to take a look at the introductory example (`example_intro`), since this example skips some of the initialization details. 

# File structure

This example is structured in the following way:

- `main.c:` This file contains the `main` function, used to initialize `ocp`, the controller, and to interface with the simulation software.
- `ocpConfig.h`: Defines the number of traces and control systems in the system.
- `stypes.h`: Configuration file for the simulation, containing a description of the input and output signals.
- `buck.py` and `python_ex_controller.py`: Python files for this example. `buck.py` has definitions that are specific to the controller, and `python_ex_controller.py` shows how to interact with the controller.
- `app` folder: contains the source files for the controller.
- `plecs` folder: contains the simulation of the converter.

The structure of the files in the `app` folder is discussed in the introductory example, and thus this won't be discussed here again.

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

First, we can build and run the C program, which will initialize the controller and be ready to exchange data with the simulation. To do so, build and run this example (manually or with Visual Studio Code).

Once the program is running, we can use the Python interface to query the controller. Next, run the `python_ex_controller.py` from within a Python interpreter (we use IDLE for this), and now the controller should be accessible from the Python interpreter. For example, we can query the number and name of the controllers in the application:

```python
>>> ocp.cs_get_number_controllers()
(0, 1)
>>> ocp.cs_get_controllers_names()
(0, [b'App controller'])
```

In `ocp`, `traces` are used to log data. These traces can be used to log measurements, control signals and debug signals. In the same way that we can query the controllers in the application, we can query the traces:

```python
>>> ocp.trace_get_number_traces()
(0, 1)
>>> ocp.trace_get_traces_names()
(0, [b'App trace'])
```

In this example, there is a single trace and a single controller in the system. They are defined in the `ocpConfig.h` file, and their names are defined during the application's initialization.

We can address individual traces (as well as controllers) by specifying it during a query. For example, we can find how many signals a trace contains and their labels, as follows:

```python
>>> ocp.trace_get_number_signals(0)
(0, 4)
>>> ocp.trace_get_signals_names(0)
(0, [b'Inductor current', b'Output voltage', b'Duty-cycle', b'Reference'])
```

Note that we passed `0` as the argument to  `ocp.trace_get_number_signals` and `ocp.trace_get_signals_names`, because  traces are addressed by an integer (their ID) starting from zero. In this case, there is only one trace, so its ID is `0`. From the output of the command, we can see that the trace has 4 signals, with the indicated labels.  

The size of the trace, e.g. how many bytes it can store, can be set dynamically. Initially, its size is set to its maximum possible size. If we query the size of the trace, we get:

```python
>>> ocp.trace_get_size(0)
(0, 10485760)
```

In this case, the trace can store up to 10 MB of data. All signals in a single trace are stored simultaneously at an event defined by the user in the C code of the application. A common case is to set the trace so that it stores its signals every time the controller is executed. 

The size of the trace can be dynamically set (up to its maximum size). For example, let's say we want to record the trace signals for 4000 control events. Each trace is saved as a float in the controller's memory, which takes 4 bytes. Thus, if we have 4 signals and want to store 4000 samples of each, we need to set the trace to have a size of `4*4*4000`:

```python
>>> ocp.trace_set_size(0, 4 * 4 * 4000)
(0,)
>>> ocp.trace_get_size(0)
(0, 64000)
```

## Setting controller parameters

The Python interface allows setting parameters of the controller dynamically. Each controller has a "controller interface" function that can be used to exchange data with the controller. In this example, the implementation only sends data to the controller, but  `example_multiple_controllers` shows how to send and receive data.

If we check the `app/appController.c`, we see two important definitions. At the top of the file, we see:
```c
typedef struct{
    float ki;
    float kv;
    float k_ev;
    float v_ref;
}params_t;
```
and
```c
static params_t params = {
    .ki = 0.0530530303030496f, .kv = -0.04430925432587546f, .k_ev = -98.62715371027674f,
    .v_ref = 6.0f
};
```

We use the `params_t` structure, and a `params` variable, to define the parameters of the controller we wish to change dynamically. Then, in the  `appControllerIf`, we'll see the following code:
```c
int32_t appControllerIf(void *in, uint32_t insize, void **out, uint32_t maxoutsize){

    (void)out;
    (void)maxoutsize;

    if( insize != sizeof(params_t) ) return -1;
    memcpy( (void *)&params, in, sizeof(params_t) );

    return 0;
}
```

This functions takes the data in the `in` buffer and copies it to the `params` variable that was declared earlier. 

In the Python side, there is an `ocp.cs_controller_if` function, which takes as argument a data packet to be delivered to the `appControllerIf` function in C. Because we know that the C function will copy the binary data to the `params` variable, we can build the data packet in Python so that we can set the parameters in the controller.

In Python, we can create a function to assemble this data packet as follows (defined in the `buck.py` file):

```python
def format_params(ki, kv, k_ev, v_ref):

    data_bin = struct.pack('<ffff', ki, kv, k_ev, v_ref)

    return data_bin
```

This function takes the parameters of the controller as arguments, and packs them in a binary format so that they can be sent to the controller. 

Ideally, we don't want to set the gains of the controller directly. We'd like to specify some requirements such as settling time and overshoot, and then obtain the gains of the controller. For a state feedback controller, this is possible by using pole placement. 

Although the equations to design a state feedback controller are not complicated, implementing them in Python is straightforward, while coding them in C would require a lot more work. 

Since we can transmit data to the controller using the controller's interface, we can design the controller in Python, and simply transmit the parameters to the controller. In this way, all the complexity of the design method can be written in Python, which is much easier to debug, test, and integrate with external libraries. The C code is only responsible for executing the control law and setting the parameters of the controller, leading to a smaller and more robust  C code that is easier to maintain.

In this example, we implement the function to design the controller and pack its parameters in binary format in the `buck.py` file. This file has two functions: `get_gains`, to get the gains of the controller according to settling time and overshoot specifications; and `format_gains` (shown above), to pack all parameters in the binary format expected by the controller.

Now, we can set our controller. First, we obtain its gains, then we format these gains along with the reference in a binary data packet, and finally send the packet to the controller:

```python
>>> ki, kv, k_ev = buck.get_gains(ts=1e-3, os=5)
>>> ctlr_bin_data = buck.format_params(ki, kv, k_ev, 6)
>>> ocp.cs_controller_if(0, ctlr_bin_data)
(0, b'')
```

## Running the controller

Now that the controller is set, we can run a closed-loop simulation and retrieve data stored by the controller. To do so, we use PLECS to simulate the buck converter. Open the PLECS model in the `plecs` folder and start the simulation. In `ocp`, the controller is initially disabled, and must be enabled. In the Python shell, run:

```python
>>> ocp.trace_reset(0); ocp.cs_enable(0)
(0,)
```

This resets the trace (so it starts recording data again), and enables the controller. Now, if you look at PLECS' scope, you should see that the voltage is regulated to 6 V, the reference value we had previously set in the controller.

Next, we can check the trace signals to see the data the controller recorded. We can retrieve the data from the controller as follows:

```python
>>> status, trace_data = ocp.trace_read(0)
```

`ocp.trace_read` returns the data stored in the controller's memory in binary format. If you check, `trace_data` has the size that we had previously set:

```python
>>> len(trace_data)
64000
```

Because we know that we have four float signals in the trace, we can convert this binary data to a numpy array. In `python_ex_controller.py`, we wrote a function to do exactly that. To convert the data, run: 

```python
>>> data = trace_data_to_np_array(trace_data, 4)
```

The second argument of this function is the number of signals in the trace, which is 4. Now, if we check the shape of `data`, we get:

```python
>>> data.shape
(4000, 4)
```

This means each signal has 4000 samples, and there are four signals, exactly as we've configured the trace previously. Now that we have the data, we can for example compare the reference signal and the output voltage as follows:

```python
>>> plt.plot(data[:, 1])
>>> plt.plot(data[:, 3])
```

## Next
Now that we have the flexibility of Python to design and parametrize the controller, testing the controller with different parameters is just a matter of creating additional Python code to loop through some parameters and configure the controller.

Here, we are directly interacting with the `ocp` Python module, which offers the low level interface to the controller. Because of this, it is a bit awkward to set and read the trace, and set the parameters of the converter.

However, it is possible to use the primitives of the `ocp` Python module to build a higher level module that is much easier to interact with. Moreover, it is also possible to design, set and run multiple controllers, so that they can be compared. This is expanded in the `example_multiple_controllers` example.
