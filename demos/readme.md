## Introduction

This folder contains some examples to illustrate some of the features of `ocp`. 

## Installing `pyocp` 

To run the examples, it is assumed that you have the `pyocp` Python package installed in your Python environment. This package can be found in the main directory of the repository, inside the `python` folder. To install the package, navigate to the `python` folder with the command prompt, and run
```sh
python -m pip install -e .
``` 

## Building

To run the examples, it is assumed that you have CMake and a C compiler. The examples can be built and executed with the command line, or using, for example, Visual Studio Code. 

### Running on Windows

If you're running on Windows, we strongly recommend using [MSYS2](https://www.msys2.org/) to install a 64-bit GCC compiler and Make. You can follow the instructions on the main page to install MSYS2 and the GCC compiler. After that, you can install make with

```sh
pacman -S make
```

Don't forget to add the path to the compiler and `make` to your `PATH` environment variable. If you installed MSYS2 following the instructions on the website, then add the following paths to `PATH`, in this order:

- `C:\msys2\ucrt64\bin`
- `C:\msys2\usr\bin`

To install CMake, check out the instructions in their [download page](https://cmake.org/download/).
