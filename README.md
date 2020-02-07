# monte-carlo-cpp
A High speed financial Monte Carlo simulation library for C++

# Dependencies

* G++
* Boost
* Nlohmann Json (included)
* Python 3
* Matplotlib
* Pandas
* [compile-me](https://github.com/cnaimo/compile-me) (included)

# Usage

A pre-compiled library file is included for convenience. To begin using the library in your own project, copy the files monte_carlo.a and monte_carlo.h to your project directory. In your C++ project main file, add the library header file:

```c++
#include "monte_carlo.h"
```

When compiling, link the library file
```
g++ my_project.cpp monte_carlo.a
```

# Example
A sample C++ project has been included. Run compile.py and press enter to compile and run example.cpp:

```
Python3 compile.py
```

