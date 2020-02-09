[![PyPI license](https://img.shields.io/pypi/l/ansicolortags.svg)](https://pypi.python.org/pypi/ansicolortags/) [![GitHub pull-requests](https://img.shields.io/github/issues-pr/Naereen/StrapDown.js.svg)](https://GitHub.com/Naereen/StrapDown.js/pull/) [![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](https://github.com/dwyl/esta/issues) 

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
## Assets
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

A full derivation is available [here](https://www.quantstart.com/articles/Geometric-Brownian-Motion/). This equation is used to perform the Monte Carlo simulation.

## Derivatives: European Options

The value of a european option at some time t is as follows:

<p align="center">
  <img src="https://latex.codecogs.com/gif.latex?\LARGE&space;V(S(t),t)&space;=&space;\mathbb{E}^{Q}\left&space;[&space;h(S(T),T))exp\left&space;(-&space;\int_{t}^{T}r(s)ds\right&space;)&space;\right&space;]">
</p>

Where:
- V = Option Value
- S = Underlying Asset Price
- h = Option Payoff
- r = Risk Free Rate
- T = Time of Option Maturity
- E<sup>Q</sup> = Expectation with risk neutral measure (Q)

The value of option payoff at maturity varies between option types:

<p align="center">
  <img src="https://latex.codecogs.com/gif.latex?\LARGE&space;h(S(T),T)&space;=&space;\left\{\begin{matrix}&space;max\left&space;[&space;S(T)-K,0&space;\right&space;],&space;&&space;for\,call\,option&space;\\&space;max\left&space;[&space;K-S(T),0&space;\right&space;],&space;&&space;for\,put\,option&space;\end{matrix}\right.">
</p>

Where:
- K = Strike Price

## Derivatives: Asian Options

The equation for valuing european options can be applied to asian options as well, taking into account differing payoff formulae:

<p align="center">
  <img src="https://latex.codecogs.com/gif.latex?\LARGE&space;V(S(t),t)&space;=&space;\mathbb{E}^{Q}\left&space;[&space;h(S(t,T),T))exp\left&space;(-&space;\int_{t}^{T}r(s)ds\right&space;)&space;\right&space;]">
</p>
<p align="center">
  <img src="https://latex.codecogs.com/gif.latex?h(S(t,T),T)&space;=&space;\left\{\begin{matrix}&space;max\left&space;[&space;\frac{1}{T}(&space;\int_{0}^{T}S(t)dt)-K,&space;0\right&space;],&space;&&space;for\,fixed\,strike\,arithmatic\,mean\,call\,option&space;\\&space;max\left&space;[K&space;-&space;\frac{1}{T}(&space;\int_{0}^{T}S(t)dt),&space;0\right&space;],&space;&&space;for\,fixed\,strike\,arithmatic\,mean\,put\,option&space;\\&space;max\left&space;[&space;S(T)-\frac{k}{T}(&space;\int_{0}^{T}S(t)dt),0&space;\right&space;],&space;&&space;for\,floating\,strike\,arithmatic\,mean\,call\,option\\&space;max\left&space;[\frac{k}{T}(&space;\int_{0}^{T}S(t)dt)-S(T),0&space;\right&space;],&space;&&space;for\,floating\,strike\,arithmatic\,mean\,put\,option&space;\end{matrix}\right.">
</p>

Where:
- k = some weighting, typically 1

## Derivatives: Valuing via Monte Carlo

Pricing asian and european options via Monte Carlo is accomplished by:
- Perfoming a Monte Carlo simulation on the underlying asset through the option maturity date
- Determining the option payout for each Monte Carlo iteration
- Averaging payouts between iterations
- Discounting backward using the current risk free rate and the number of days until option maturity

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
# References
- [Pricing American Options With Monte Carlo Methods](https://www.maths.ox.ac.uk/system/files/attachments/TT18_dissertation_1000246.pdf)
- [Asian Options](https://en.wikipedia.org/wiki/Asian_option)
- [Geometric Brownian Motions](https://www.quantstart.com/articles/Geometric-Brownian-Motion/)

