# monte-carlo-cpp
A high speed financial Monte Carlo simulation library for C++

# Dependencies

* [G++](https://gcc.gnu.org/)
* [Boost](https://www.boost.org/)
* [Nlohmann Json](https://github.com/nlohmann/json) (included)
* [Python 3](https://www.python.org/)
* [Matplotlib](https://github.com/matplotlib/matplotlib)
* [Pandas](https://github.com/pandas-dev/pandas)
* [compile-me](https://github.com/cnaimo/compile-me) (included)

# Theory
Price timeseries for simple assets such as commodities and equities are typically modelled with the following stochastic differential equation (SDE):

<p align="center">
  <img src="https://latex.codecogs.com/gif.latex?\LARGE&space;dS(t)&space;=&space;\mu&space;S(t)dt&space;&plus;&space;\sigma&space;S(t)B(t)">
</p>

Where:
- S(t) = Asset Price
- μ = Drift
- σ = Volatility
- B(t) = Geometric Brownian Motion

This SDE can be solved by applying Ito's Lemma, resulting in the following equation:

<p align="center">
  <img src="https://latex.codecogs.com/gif.latex?\LARGE&space;S(t)&space;=&space;S(0)e^{(\mu&space;-&space;\frac{1}{2}\sigma&space;^{2})t&space;&plus;&space;\sigma&space;B(t)}">
</p>

A full derivation is available [here](https://www.quantstart.com/articles/Geometric-Brownian-Motion/). This final equation is used to perform the Monte Carlo simulation.

# Usage

A pre-compiled library file is included for convenience (Note: compiled for Linux via Ubuntu 18.04, may not work on Mac/Windows). To begin using the library in your own project, copy the files monte_carlo.a and monte_carlo.h to your project directory. In your C++ project main file, add the library header file:

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

