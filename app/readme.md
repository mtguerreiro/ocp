# Introduction

This folder contains some examples to illustrate some of the features of `ocp`. 

To run the examples, it is assumed that you have CMake and a C compiler. The examples can be built and executed with the command line, or using, for example, Visual Studio Code.

Moreover, it is assumed to you have the `pyocp` Python package installed in your Python environment. This package can be found in the main directory of the repository, inside the `python` folder. To install the package, navigate to the `python` folder with the command prompt, and run
```sh
python -m pip install -e .
``` 

# Examples

- `example_controller`: Illustrates the principles behind using the controller interface to set controller parameters, and basic usage of the trace for recording data. 
