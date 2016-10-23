#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
 
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/density.hpp>
#include <boost/accumulators/statistics/stats.hpp>
 
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>


// COMPILE WITH :  gcc histogram.cpp -o histogram -lboost_program_options

using namespace boost;
using namespace boost::accumulators;
using namespace boost::program_options; 
 
typedef accumulator_set<double, features<tag::density,tag::count> > acc;
typedef iterator_range<std::vector<std::pair<double, double> >::iterator > histogram_type; 
 
template <typename T>
class data_filler 
{  
public:
  data_filler(){}
  T operator()() { return rand()/(T)RAND_MAX; }
};
 
int main(int argc, char** argv)
{
 
  //create some random data
  //std::vector<double> data(100);
  //std::generate(data.begin(), data.end(), data_filler<double>());


	std::vector<double> data;
	std::string line;
	bool norm = true;

	options_description desc("Histogram program, the allowed options are");
	desc.add_options()
    	("help,h", "produce help message")
    	("min", value<double>()->default_value(0.0), "sets lower data cut for bins minimum")
    	("max", value<double>()->default_value(4096.0), "sets upper data cut for bins maximum")
    	("bins,n", value<int>()->default_value(50), "sets number of bins")
    	("norm,m", value<bool>()->default_value(true), "normalises the data")
    	("input,i", value<std::string>(),"input data file");

	variables_map vm;
	store(parse_command_line(argc, argv, desc), vm);
	notify(vm);    

	if (vm.count("norm")) {
		norm=vm["norm"].as<bool>();
	}
	if (vm.count("help")) {
    		std::cout << desc << "\n";
    		return 1;
	}

	double min_limit=0.0;
	double max_limit=4096.0;
	if (vm.count("min")) {
		min_limit=vm["min"].as<double>();
	}
	if (vm.count("max")) {
		max_limit=vm["max"].as<double>();
	}

	if (!vm.count("input")) {
		std::cin >> line;
		while(!line.empty()){
			data.push_back(atof(line.c_str()));
			line.clear();
			std::cin>>line;
			// std::cout << data.size() << std::endl;
		}
	}else{

		std::string filename(vm["input"].as<std::string>());	
		std::ifstream myfile(filename.c_str());// input filename
 		 if(myfile.is_open()) {
		 	while ( getline (myfile,line) ) {
 				data.push_back(atof(line.c_str()));
  			}
 		}
 		myfile.close();
	}


	int Nbins=50;
	if (vm.count("bins")) {
		Nbins = vm["bins"].as<int>();
	}


	int c = data.size();//cache size for histogram
	acc myAccumulator( tag::density::num_bins = Nbins, tag::density::cache_size = 2);
 	myAccumulator(min_limit);
 	myAccumulator(max_limit);

  	//fill accumulator 
  	for (int j = 0; j < c; ++j)
  	{
		myAccumulator(data[j]);
  	}
	// std::cout << " " << count(myAccumulator) << std::endl; //should be 1 (and it is)
   	
	histogram_type hist = density(myAccumulator); 
  	double total = 0.0;
 
  	for( int i = 0; i < hist.size(); i++ ) 
  	{
    		if(!norm){
			std::cout << std::setw(10) << hist[i].first << "  " << std::setw(10) << hist[i].second*count(myAccumulator) << std::endl; 
		}else{
			std::cout << std::setw(10) << hist[i].first << "  " << std::setw(10) << hist[i].second << std::endl; 		
		}
  	}
 
  // std::cout << "Total: " << total << std::endl; //should be 1 (and it is)
 
  return 0;
}
