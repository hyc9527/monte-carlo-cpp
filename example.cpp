#include "monte_carlo.h"
#include "json/json_3.7.3.hpp"
#include <vector>
#include <iostream>
#include <fstream>

using json = nlohmann::json;

int main(){
	std::vector<double> your_data = {1.2, 1.2, 1.12, 1.23, 1.02, 1.2, 1.22, 1.225, 1.2, 1.25};

	// run 100 iterations with a length of 250
	std::vector<std::vector<double> > vec = monte_carlo(your_data, 250, 100);

	// save results to json file
	json j;
	for(int i=0; i!=vec.size(); ++i){
		j[std::to_string(i)] = json::array();
		for(int x=0; x!=vec[i].size(); ++x){
			j[std::to_string(i)].push_back(vec[i][x]);
		}
	}
	std::ofstream myfile;
	myfile.open("sample.json");
	myfile << j.dump(4);
	myfile.close();
	
	// plot via matplotlib
	int plot = system("python3 sample_plot.py");
	return 0;
}
