#include <random>
#include <vector>
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


