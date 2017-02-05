
/*

Neural Network

Version 3.0

Created by William Gregory on October 1st, 2016
Copyright (c) 2016 William Gregory.	 All rights reserved.

Ideas:
	- create (error, debug, info)/message class or remove functions
	- create testing class
	- add scale function for input and output to meet network requirements given min and max inputs

ToDo:
	- add weight import/export to file (save best network after training)

Info:
	- settings
		- debug: provide function start end info to find where error occurs.
		- verbose: provide info of internal network states while running

*/

#ifndef _NEURAL_NETWORK_HPP_
#define _NEURAL_NETWORK_HPP_

// manage in system
//#define NN_DEBUG
//#define NN_VERBOSE
//#define NN_TEST

#include <iostream>	 // cout, endl
#include <vector>  // .. vectors
#include <stdlib.h> // srand, rand, exit
#include <cstdlib> // RAND_MAX
#include <time.h> // time for srand and delta times
#include <math.h> // exp - sigmoid
#include <string>  // messages
#include <ctime>  // clock
#include <fstream>	// file IO

#define LYRAND (double)rand()/RAND_MAX

namespace Network {

	class Network
	{
	private:
		/// variables
		std::vector <Layer> layers;
		bool runtime_error;
		/// functions
		void error_call(std::string);
		// add bias to npl count (sub) (multi)
		// input: desired npl layer given to setup
		// output: returned with added bias to proper layers
		std::vector <unsigned int> add_bias(std::vector <unsigned int>& );
		// create network (sub) (single)
		// input: nodes per layer (excluding bias), mutate mod and chance
		void create_network(std::vector <unsigned int>&, double&, double& );
		// format input (sub) (multi)
		// input: input vector to network
		// output: formatted for passing to input layer
		std::vector <std::vector <double> > format_input(std::vector <double>& in);
	public:
		/// variables
		unsigned int ID_value;
		std::vector <unsigned int> npl;	 // including bias
		/// functions
		Network();
		// setup (main) (single)
		void setup( std::vector <unsigned int>&, double&, double& );
		// cycle network (main) (multi) (time)
		// input: input vector (one double per input node)
		// output: network output (one double per output node)
		std::vector <double> cycle(std::vector <double>&);
		// mutate (main) (semi-multi)(time)
		void mutate(); 
		// export weights (side)
		std::vector <std::vector <std::vector <double> > > export_weights();
		// import weights (side)
		void import_weights(std::vector <std::vector <std::vector <double> > >);
	};
}

#endif
