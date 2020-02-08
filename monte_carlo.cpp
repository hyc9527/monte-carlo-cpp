#include <random>
#include <algorithm>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>


std::vector<std::vector<double> > monte_carlo(std::vector<double> data, const int sim_len, const int iterations){
    double sigma;
    double mu;
    double drift;
    std::vector<double> log_returns;
    std::vector<double> sim;
    std::vector<std::vector<double> > sim_full;
    log_returns.reserve(data.size()-1);
    sim.reserve(sim_len);

    for(int i=1; i!=data.size(); ++i){
        log_returns.emplace_back(log(data[i] / data[i-1]));
    }

    // get standard deviation and mean
    using namespace boost::accumulators;
    accumulator_set<double, features<tag::mean, tag::variance>> acc;

    for(auto it=log_returns.begin(); it!=log_returns.end(); ++it){
        acc(*it);
    }

    sigma = std::sqrt(variance(acc));
    mu = mean(acc);
    drift = mu - (0.5 * pow(sigma, 2.0));

    std::random_device rand; 
    std::mt19937_64 gen(rand());
    std::normal_distribution<double> dist(0.0, sigma); 

    for(int i=0; i!=iterations; ++i){
        sim.emplace_back(data[data.size()-1] * exp(drift + dist(gen)));
        for(int x=1; x!=sim_len; ++x){
            sim.emplace_back(sim.back() * exp(drift + dist(gen)));
        }
        sim_full.push_back(sim);
        sim.clear();
    }
    return sim_full;
}

double monte_carlo_fixed_strike_arithmatic_avg_asian_call(std::vector<double> data_underlying, const double strike, const double risk_free_rate, const int days_to_exp, const int iterations){
    std::vector<double> average_prices;
    std::vector<double> payout;
    double sum;

    // perform Monte Carlo on underlying asset
    std::vector<std::vector<double> > vec = monte_carlo(data_underlying, days_to_exp, iterations);

    average_prices.reserve(iterations);
    payout.reserve(iterations);

    for(int i=0; i!=vec.size(); ++i){
        sum = 0.0;
        for(auto it=vec[i].begin(); it!=vec[i].end(); ++it){
            sum += *it;
        }
        average_prices.emplace_back(sum / double(days_to_exp));  
    }

    // determine average payout & discount back
    sum = 0.0;
    for(auto it=average_prices.begin(); it!=average_prices.end(); ++it){
        sum += std::max(*it - strike, 0.0);
    }
    return (sum / double(iterations)) * std::exp(-risk_free_rate * (double(days_to_exp) / 365.0));
}

double monte_carlo_fixed_strike_arithmatic_avg_asian_put(std::vector<double> data_underlying, const double strike, const double risk_free_rate, const int days_to_exp, const int iterations){
    std::vector<double> average_prices;
    std::vector<double> payout;
    double sum;

    // perform Monte Carlo on underlying asset
    std::vector<std::vector<double> > vec = monte_carlo(data_underlying, days_to_exp, iterations);

    average_prices.reserve(iterations);
    payout.reserve(iterations);

    for(int i=0; i!=vec.size(); ++i){
        sum = 0.0;
        for(auto it=vec[i].begin(); it!=vec[i].end(); ++it){
            sum += *it;
        }
        average_prices.emplace_back(sum / double(days_to_exp));  
    }

    // determine average payout & discount back
    sum = 0.0;
    for(auto it=average_prices.begin(); it!=average_prices.end(); ++it){
        sum += std::max(strike - *it, 0.0);
    }
    return (sum / double(iterations)) * std::exp(-risk_free_rate * (double(days_to_exp) / 365.0));
}

double monte_carlo_floating_strike_arithmatic_avg_asian_call(std::vector<double> data_underlying, const double k, const double risk_free_rate, const int days_to_exp, const int iterations){
    std::vector<double> average_prices;
    std::vector<double> payout;
    double maturity_price;
    double sum;

    // perform Monte Carlo on underlying asset
    std::vector<std::vector<double> > vec = monte_carlo(data_underlying, days_to_exp, iterations);

    average_prices.reserve(iterations);
    payout.reserve(iterations);

    for(int i=0; i!=vec.size(); ++i){
        sum = 0.0;
        for(auto it=vec[i].begin(); it!=vec[i].end(); ++it){
            sum += *it;
        }
        average_prices.emplace_back(sum / double(days_to_exp));  
    }

    // get average price at maturity
    sum = 0.0;
    for(int i=0; i!=vec.size(); ++i){
        sum += vec[i].back();
    }
    maturity_price = sum / double(iterations);

    // determine average payout & discount back
    sum = 0.0;
    for(auto it=average_prices.begin(); it!=average_prices.end(); ++it){
        sum += std::max(maturity_price - (*it * k), 0.0);
    }
    return (sum / double(iterations)) * std::exp(-risk_free_rate * (double(days_to_exp) / 365.0));
}

double monte_carlo_floating_strike_arithmatic_avg_asian_put(std::vector<double> data_underlying, const double k, const double risk_free_rate, const int days_to_exp, const int iterations){
    std::vector<double> average_prices;
    std::vector<double> payout;
    double maturity_price;
    double sum;

    // perform Monte Carlo on underlying asset
    std::vector<std::vector<double> > vec = monte_carlo(data_underlying, days_to_exp, iterations);

    average_prices.reserve(iterations);
    payout.reserve(iterations);

    for(int i=0; i!=vec.size(); ++i){
        sum = 0.0;
        for(auto it=vec[i].begin(); it!=vec[i].end(); ++it){
            sum += *it;
        }
        average_prices.emplace_back(sum / double(days_to_exp));  
    }

    // get average price at maturity
    sum = 0.0;
    for(int i=0; i!=vec.size(); ++i){
        sum += vec[i].back();
    }
    maturity_price = sum / double(iterations);

    // determine average payout & discount back
    sum = 0.0;
    for(auto it=average_prices.begin(); it!=average_prices.end(); ++it){
        sum += std::max((*it * k) - maturity_price, 0.0);
    }
    return (sum / double(iterations)) * std::exp(-risk_free_rate * (double(days_to_exp) / 365.0));
}
