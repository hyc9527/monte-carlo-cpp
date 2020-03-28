[![PyPI license](https://img.shields.io/pypi/l/ansicolortags.svg)](https://pypi.python.org/pypi/ansicolortags/) [![GitHub pull-requests](https://img.shields.io/github/issues-pr/Naereen/StrapDown.js.svg)](https://GitHub.com/Naereen/StrapDown.js/pull/) [![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](https://github.com/dwyl/esta/issues) 

# monte-carlo-cpp
A high-speed financial Monte Carlo simulation library for C++

# Contact
Questions? Feel free to reach out via my LinkedIn on my profile page. I'm also seeking employment in data science/finance!

# Dependencies

* [GCC](https://gcc.gnu.org/)
* [Boost](https://www.boost.org/)
* [Nlohmann Json](https://github.com/nlohmann/json) (Included)
* [CxxPolyFit](https://github.com/LLNL/CxxPolyFit) (Must be statically compiled for use with compile-me)
* [Python 3](https://www.python.org/)
* [Matplotlib](https://github.com/matplotlib/matplotlib)
* [Pandas](https://github.com/pandas-dev/pandas)
* [compile-me](https://github.com/cnaimo/compile-me) (Included)

# Theory
## Assets
Price timeseries for assets are typically modelled with the following stochastic differential equation (SDE):

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

The value of a European option at some time t is as follows:

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

The equation for valuing European options can be applied to Asian options as well, taking into account differing payoff formulae:

<p align="center">
  <img src="https://latex.codecogs.com/gif.latex?\LARGE&space;V(S(t),t)&space;=&space;\mathbb{E}^{Q}\left&space;[&space;h(S(t,T),T))exp\left&space;(-&space;\int_{t}^{T}r(s)ds\right&space;)&space;\right&space;]">
</p>
<p align="center">
  <img src="https://latex.codecogs.com/gif.latex?\large&space;h(S(t,T),T)&space;=&space;\left\{\begin{matrix}&space;max\left&space;[&space;\frac{1}{T}(&space;\int_{0}^{T}S(t)dt)-K,&space;0\right&space;],&space;&&space;for\,fixed\,strike\,arithmatic\,mean\,call\,option&space;\\&space;max\left&space;[K&space;-&space;\frac{1}{T}(&space;\int_{0}^{T}S(t)dt),&space;0\right&space;],&space;&&space;for\,fixed\,strike\,arithmatic\,mean\,put\,option&space;\\&space;max\left&space;[&space;S(T)-\frac{k}{T}(&space;\int_{0}^{T}S(t)dt),0&space;\right&space;],&space;&&space;for\,floating\,strike\,arithmatic\,mean\,call\,option\\&space;max\left&space;[\frac{k}{T}(&space;\int_{0}^{T}S(t)dt)-S(T),0&space;\right&space;],&space;&&space;for\,floating\,strike\,arithmatic\,mean\,put\,option&space;\end{matrix}\right.">
</p>

Where:
- k = some weighting, typically 1

## Derivatives: Valuing Asian & European Options

Pricing Asian and European options via Monte Carlo is accomplished by:
- Perfoming a Monte Carlo simulation on the underlying asset through the option maturity date
- Determining the option payout for each Monte Carlo iteration
- Averaging payouts between iterations
- Discounting backward using the current risk free rate and the number of days until option maturity

## Derivatives: Valuing American Options via Longstaff Schwartz

When an American option is exercised only at maturity, its value would be the same as a European option. However, American options give the option holder the right to exercise the option at any time whereas European options can only be exercised at maturity. In order to value American options accurately, the time of ideal option excersize must be determined. One solution proposed by [Longstaff & Schwartz](https://people.math.ethz.ch/~hjfurrer/teaching/LongstaffSchwartzAmericanOptionsLeastSquareMonteCarlo.pdf) is to perform a polynomial regression to estimate the value of continuing to hold an option on each day until maturity and comparing with the value of exercising that option on each day until maturity. The ideal time for exercise is found by locating the first point where immediate exercise is more profitable than the discounted value of continuation. The value of the option is then estimated by averaging all discouted cash flows from ideal option exercise over each underlying Monte Carlo iteration.

Using this method, the value of continuation is as follows:
<p align="center">
  <img src="https://latex.codecogs.com/gif.latex?\LARGE&space;F(\omega&space;;t_{_k})&space;=&space;\mathbb{E}^{Q}\left&space;[&space;\sum_{j=k&plus;1}^{K}&space;exp\left&space;(-&space;\int_{t_{k}}^{t_{j}}r(\omega,s)ds&space;\right)C(\omega&space;,t_{j};t_{k},T)|\mathfrak{F}_{\textup{tk}}\right&space;]">
</p>

Where:
- F(ω;t) = Value of continuation
- E<sup>Q</sup> = Expectation with risk neutral measure (Q)
- r(ω,t) = Riskless discount rate
- C(ω,s;t,T) = Path of option cash flows using using optimal stopping and not exercising before time t
- 𝕱<sub>tk</sub> = Given information set

# Usage

A pre-compiled library file is included for convenience (Note: compiled for Linux via Ubuntu 18.04, may not work on Mac/Windows). To begin using the library in your own project, copy the files monte_carlo.a and monte_carlo.h to your project directory. In your C++ project main file, add the library header file:

```c++
#include "monte_carlo.h"
```
When compiling, link the library file & dependencies
```
g++ my_project.cpp monte_carlo.a -lcxxlsfit -lblas -llapack
```

## Functions

```c++
void monte_carlo(std::vector<double> &data, std::vector<std::vector<double> > &sim_vec, int sim_len, const int iterations)
```
- Performs a Monte Carlo simulation using ```data``` as the historical asset price timeseries and ```sim_vec``` which will contain the resulting simulation asset paths.


```c++
double monte_carlo_fixed_strike_arithmatic_avg_asian_call(std::vector<double> &data_underlying, const double strike, const double risk_free_rate, const int days_to_exp, const int iterations=100000)
```
- Returns the estimated value of a fixed strike arithmatic average Asian call option. Lookback period for the average price is fixed from the first date of the Monte Carlo simulation through maturity.

```c++
double monte_carlo_fixed_strike_arithmatic_avg_asian_put(std::vector<double> &data_underlying, const double strike, const double risk_free_rate, const int days_to_exp, const int iterations=100000)
```
- Returns the estimated value of a fixed strike arithmatic average Asian put option. Lookback period for the average price is fixed from the first date of the Monte Carlo simulation through maturity.


```c++
double monte_carlo_floating_strike_arithmatic_avg_asian_call(std::vector<double> &data_underlying, const double strike, const double risk_free_rate, const int days_to_exp, const int iterations=100000)
```
- Returns the estimated value of a floating strike arithmatic average Asian call option. Lookback period for the average price is fixed from the first date of the Monte Carlo simulation through maturity.


```c++
double monte_carlo_floating_strike_arithmatic_avg_asian_put(std::vector<double> &data_underlying, const double strike, const double risk_free_rate, const int days_to_exp, const int iterations=100000)
```
- Returns the estimated value of a floating strike arithmatic average Asian put option. Lookback period for the average price is fixed from the first date of the Monte Carlo simulation through maturity.

```c++
double american_put_longstaff_schwartz(std::vector<double> &data_underlying, const double strike, const double risk_free_rate, const int days_to_exp, const int iterations=100000)
```
- Returns the estimated value of an American put via Longstaff Schwartz

```c++
double american_call_longstaff_schwartz(std::vector<double> &data_underlying, const double strike, const double risk_free_rate, const int days_to_exp, const int iterations=100000)
```
- Returns the estimated value of an American call via Longstaff Schwartz


# Example
A sample C++ project has been included. Run compile.py and press enter to compile and run example.cpp:

```
Python3 compile.py
```

# Todo
- Add European options
- Add adjustable average lookback period for Asian options


# References
- [Valuing American Options by Simulation: A Simple Least-Squares Approach](https://people.math.ethz.ch/~hjfurrer/teaching/LongstaffSchwartzAmericanOptionsLeastSquareMonteCarlo.pdf)
- [Pricing American Options With Monte Carlo Methods](https://www.maths.ox.ac.uk/system/files/attachments/TT18_dissertation_1000246.pdf)
- [Asian Options](https://en.wikipedia.org/wiki/Asian_option)
- [Geometric Brownian Motions](https://www.quantstart.com/articles/Geometric-Brownian-Motion/)

