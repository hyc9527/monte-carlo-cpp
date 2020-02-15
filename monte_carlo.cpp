#include <random>
#include <algorithm>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
#include <Polynomial.hh>

double discount(const double risk_free_rate, const int days){
    return std::exp(-risk_free_rate * (double(days) / 252.0));
}

void monte_carlo(std::vector<double> &data, std::vector<std::vector<double> > &sim_vec, int sim_len, const int iterations){
    double sigma;
    double mu;
    double drift;
    std::vector<double> log_returns;
    std::vector<double> sim;
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

    // generate simulation
    for(int i=0; i!=iterations; ++i){
        sim.emplace_back(data[data.size()-1] * exp(drift + dist(gen)));
        for(int x=1; x!=sim_len; ++x){
            sim.emplace_back(sim.back() * exp(drift + dist(gen)));
        }
        sim_vec.emplace_back(sim);
        sim.clear();
    }
}

double monte_carlo_fixed_strike_arithmatic_avg_asian_call(std::vector<double> &data_underlying, const double strike, const double risk_free_rate, const int days_to_exp, const int iterations){
    std::vector<double> average_prices;
    std::vector<double> payout;
    std::vector<std::vector<double> > vec;
    double sum;

    // perform Monte Carlo on underlying asset
    monte_carlo(data_underlying, vec, days_to_exp, iterations);

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
    return (sum / double(iterations)) * std::exp(-risk_free_rate * (double(days_to_exp) / 252.0));
}

double monte_carlo_fixed_strike_arithmatic_avg_asian_put(std::vector<double> &data_underlying, const double strike, const double risk_free_rate, const int days_to_exp, const int iterations){
    std::vector<double> average_prices;
    std::vector<double> payout;
    std::vector<std::vector<double> > vec;
    double sum;

    // perform Monte Carlo on underlying asset
    monte_carlo(data_underlying, vec, days_to_exp, iterations);

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
    return (sum / double(iterations)) * std::exp(-risk_free_rate * (double(days_to_exp) / 252.0));
}

double monte_carlo_floating_strike_arithmatic_avg_asian_call(std::vector<double> &data_underlying, const double strike, const double risk_free_rate, const int days_to_exp, const int iterations){
    std::vector<double> average_prices;
    std::vector<double> payout;
    double maturity_price;
    std::vector<std::vector<double> > vec;
    double sum;

    // perform Monte Carlo on underlying asset
    monte_carlo(data_underlying, vec, days_to_exp, iterations);

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
        sum += std::max(maturity_price - (*it * strike), 0.0);
    }
    return (sum / double(iterations)) * std::exp(-risk_free_rate * (double(days_to_exp) / 252.0));
}

double monte_carlo_floating_strike_arithmatic_avg_asian_put(std::vector<double> &data_underlying, const double strike, const double risk_free_rate, const int days_to_exp, const int iterations){
    std::vector<double> average_prices;
    std::vector<double> payout;
    double maturity_price;
    std::vector<std::vector<double> > vec;
    double sum;

    // perform Monte Carlo on underlying asset
    monte_carlo(data_underlying, vec, days_to_exp, iterations);

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
        sum += std::max((*it * strike) - maturity_price, 0.0);
    }
    return (sum / double(iterations)) * std::exp(-risk_free_rate * (double(days_to_exp) / 252.0));
}

double american_put_longstaff_schwartz(std::vector<double> &data_underlying, const double strike, const double risk_free_rate, const int days_to_exp, const int iterations){
    std::vector<double> price;
    double iter_cont;  // continuation value for this particular iteration
    std::vector<std::vector<double> > sim_vec;
    std::vector<std::vector<double> > cfm; // cash flow matrix
    std::vector<double> day_cash; // cash flow for one day
    day_cash.reserve(iterations);

    double one_day_discount = discount(risk_free_rate, 1);


    // run Monte Carlo on underlying asset
    monte_carlo(data_underlying, sim_vec, days_to_exp, iterations);

    // work backwards, starting with maturity date
    for(auto it=sim_vec.begin(); it!=sim_vec.end(); ++it){
        day_cash.emplace_back(std::max(it->back() - strike, 0.0));
    }
    cfm.emplace_back(day_cash);


    for(int i=days_to_exp-2; i!=-1; --i){
        // iterate backward
        for(int x=0; x!=iterations; ++x){
            day_cash[x] = std::max(sim_vec[x][i] - strike, 0.0);
        }

        std::vector<double> xs;
        std::vector<double> ys;
        std::vector<double> continuation;
        xs.reserve(iterations);
        ys.reserve(iterations);
        continuation.reserve(iterations);

        for(int x=0; x!=iterations; ++x){
            if(day_cash[x] > 0.0){
                xs.emplace_back(sim_vec[x][i]);
                ys.emplace_back(cfm[0][x] * one_day_discount);
            }
        }

        // get 2nd order polynomial
        Polynomial poly(xs, ys, 1, 2);

        // get continuation value
        for(int x=0; x!=iterations; ++x){
            if(day_cash[x] > 0.0){
                // option is in the money
                price = {sim_vec[x][i]};
                iter_cont = poly.eval(price) * one_day_discount;
                if(iter_cont > day_cash[x]){
                    // continuation value is greater than cash flow from exercising the option today
                    // set today's cash flow to zero
                    day_cash[x] = 0.0;
                }
                else{
                    // exercise option today!
                    // set future cash flows to zero
                    for(int q=0; q!=cfm.size(); ++q){
                        cfm[q][x] = 0.0;
                    }
                }
            }
            else{
                // option is out of the money, don't exercise today
                // set today's cash flow to zero
                day_cash[x] = 0.0;
            }
        }

        // exercise data to cash flow matrix
        auto insert_front = cfm.begin();
        cfm.insert(insert_front, day_cash);
    }

    double sum = 0.0;
    int count = 0;
    for(int i=0; i!=iterations; ++i){
        for(int x=0; x!=days_to_exp; ++x){
            if(cfm[x][i] > 0.0){
                sum += cfm[x][i] * discount(risk_free_rate, x+1);
                ++count;
                break;
            }
        }
    }
    // average discounted cash flows
    return sum/double(count);
}

double american_call_longstaff_schwartz(std::vector<double> &data_underlying, const double strike, const double risk_free_rate, const int days_to_exp, const int iterations){
    std::vector<double> price;
    double iter_cont;  // continuation value for this particular iteration
    std::vector<std::vector<double> > sim_vec;
    std::vector<std::vector<double> > cfm; // cash flow matrix
    std::vector<double> day_cash; // cash flow for one day
    day_cash.reserve(iterations);

    double one_day_discount = discount(risk_free_rate, 1);


    // run Monte Carlo on underlying asset
    monte_carlo(data_underlying, sim_vec, days_to_exp, iterations);

    // work backwards, starting with maturity date
    for(auto it=sim_vec.begin(); it!=sim_vec.end(); ++it){
        day_cash.emplace_back(std::max(strike - it->back(), 0.0));
    }
    cfm.emplace_back(day_cash);


    for(int i=days_to_exp-2; i!=-1; --i){
        // iterate backward
        for(int x=0; x!=iterations; ++x){
            day_cash[x] = std::max(strike - sim_vec[x][i], 0.0);
        }

        std::vector<double> xs;
        std::vector<double> ys;
        std::vector<double> continuation;
        xs.reserve(iterations);
        ys.reserve(iterations);
        continuation.reserve(iterations);

        for(int x=0; x!=iterations; ++x){
            if(day_cash[x] > 0.0){
                xs.emplace_back(sim_vec[x][i]);
                ys.emplace_back(cfm[0][x] * one_day_discount);
            }
        }

        // get 2nd order polynomial
        Polynomial poly(xs, ys, 1, 2);

        // get continuation value
        for(int x=0; x!=iterations; ++x){
            if(day_cash[x] > 0.0){
                // option is in the money
                price = {sim_vec[x][i]};
                iter_cont = poly.eval(price) * one_day_discount;
                if(iter_cont > day_cash[x]){
                    // continuation value is greater than cash flow from exercising the option today
                    // set today's cash flow to zero
                    day_cash[x] = 0.0;
                }
                else{
                    // exercise option today!
                    // set future cash flows to zero
                    for(int q=0; q!=cfm.size(); ++q){
                        cfm[q][x] = 0.0;
                    }
                }
            }
            else{
                // option is out of the money, don't exercise today
                // set today's cash flow to zero
                day_cash[x] = 0.0;
            }
        }

        // exercise data to cash flow matrix
        auto insert_front = cfm.begin();
        cfm.insert(insert_front, day_cash);
    }

    double sum = 0.0;
    int count = 0;
    for(int i=0; i!=iterations; ++i){
        for(int x=0; x!=days_to_exp; ++x){
            if(cfm[x][i] > 0.0){
                sum += cfm[x][i] * discount(risk_free_rate, x+1);
                ++count;
                break;
            }
        }
    }
    // average discounted cash flows
    return sum/double(count);
}
