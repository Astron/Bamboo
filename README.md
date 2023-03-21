# Bamboo
A library and tools for object-oriented network protocols. [Work in progress]

----------------

## Build Instructions for GNU Linux/Make (Python bindings)

- Compile the Bamboo C++ source and tell CMake to build the Bamboo shared library.
```bash
$ cmake . -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=1 && make
```

- A `libbamboo.so` shared object file should have been generated in the current directory.
- Copy the shared object file to your `/lib` directory. The next step requires this library to be installed.
```bash
$ sudo cp libbamboo.so /lib
```

- Install the `PyBindGen` pip module to the python environment of your choice. (required to run setup.py)
- Build the Bamboo python module using the `setup.py` script in the `bindings/python` directory.
```bash
$ cd bindings/python
$ python2 -m pip install pybindgen
$ python2 setup.py build
```

- Setup and install the bamboo module in your Python environment.
- **NOTE:** If you're using a virtual environment, replace 'python2' with the path to your venv python binary.
```bash
$ python2 setup.py install
```