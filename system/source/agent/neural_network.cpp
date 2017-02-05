
/*

Neural Network

Created by William Gregory on October 1st, 2016
Copyright (c) 2016 William Gregory.	 All rights reserved.

*/

#ifndef _NEURAL_NETWORK_CPP_
#define _NEURAL_NETWORK_CPP_

#include "neural_network.hpp"

namespace Network {

	void Network::error_call(std::string in) {
		std::cout << "ERROR:Network: " << in << std::endl;
		runtime_error = true;
		exit(0);
	}

	// add bias to npl count (sub) (multi)
	// input: desired npl layer given to setup
	// output: returned with added bias to proper layers
	std::vector <unsigned int> Network::add_bias(std::vector <unsigned int>& in_npl) {
#ifdef NN_DEBUG
		std::cout << "DEBUG:Network: add bias start" << std::endl;
#endif
		std::vector <unsigned int> t_nc = in_npl;  // FIX
		// add bias node to all layers but the output
		for (std::size_t i = 0; i<(t_nc.size()-1); ++i) {
			t_nc.at(i) = (t_nc.at(i) + 1);
		}
		return t_nc;
	}

	// create network (sub) (single)
	// input: nodes per layer (excluding bias), mutate mod and chance
	void Network::create_network(std::vector <unsigned int>& in_npl, double& in_mod, double& in_chance)
	{
#ifdef NN_DEBUG
		std::cout << "DEBUG:Network: create network start" << std::endl;
#endif
		npl = add_bias(in_npl);	 // set node per layer with bias
		// create layers
		for (std::size_t i = 0; i < npl.size(); ++i) {
			// next node count
			unsigned int t_nnc;
			if (i == (npl.size()-1)) t_nnc = 0;	 // output layer
			else t_nnc = npl.at(i+1);
			// layer type
			unsigned int t_type;
			if (i == 0) t_type = 0;	 // input
			else if (i == (npl.size()-1)) t_type = 2;  // output
			else t_type = 1;  // hidden
			// debug
#ifdef NN_DEBUG
			std::string tm = "Creating layer: " + std::to_string(i) +
				"; node: " + std::to_string(npl.at(i)) +
				"; connections: " + std::to_string(t_nnc) + ";";
			std::cout << "DEBUG:Network: " << tm << std::endl;
#endif
			// layer
			Layer l;
			unsigned int t_num = i+1;
			l.setup(npl.at(i), t_nnc, in_mod, in_chance, t_type, t_num);
			layers.push_back(l);
		}
#ifdef NN_DEBUG
		std::string tm = "creating network: ";
		for (std::size_t m = 0; m<npl.size(); ++m) {
			tm = tm + std::to_string(npl.at(m)) + " ";
		}
		std::cout << "DEBUG:Network: " << tm << std::endl;
#endif
	}

	// format input (sub) (multi)
	// input: input vector to network
	// output: formatted for passing to input layer
	std::vector <std::vector <double> > Network::format_input(std::vector <double>& in) {
#ifdef NN_DEBUG
		std::cout << "DEBUG:Network: format input start" << std::endl;
#endif
		std::vector <std::vector <double> > t_out;
		std::vector <double> t_in = in;	 // FIX
		// bias node
		t_in.push_back(1.0);
		t_out.push_back(t_in);
		return t_out;
	}

	Network::Network() {
		runtime_error = false;
		ID_value = 007;
		//srand(time(0));
	}
	void Network::setup( std::vector <unsigned int>& in_npl,
				double& in_m,
				double& in_c ) {
#ifdef NN_DEBUG
		std::cout << "DEBUG:Network: setup start" << std::endl;
#endif
		create_network(in_npl, in_m, in_c);
	}

	// cycle network (main) (multi) (time)
	// input: input vector (one double per input node)
	// output: network output (one double per output node)
	std::vector <double> Network::cycle(std::vector <double>& in) {
		std::string tm = ""; // test and verbose
#ifdef NN_DEBUG
		std::cout << "DEBUG:Network: cycle start" << std::endl;
#endif
		std::vector <std::vector <double> > inputs;
		std::vector <std::vector <double> > outputs;
		for (std::size_t i=0; i<layers.size(); ++i) {
			// input layer
			switch(i) {
				case 0:
					inputs = format_input(in);
					break;
				default:
					inputs = outputs;
			}
			if (inputs.at(0).size() != layers.at(i).return_node_count()) {
				tm = "cycle - input size mismatch to nodes in layer. layer: " +
					std::to_string(i) + "; input: " +
					std::to_string(inputs.size()) + "; nodes: " +
					std::to_string(layers.at(i).return_node_count()) + ";";
				
				error_call(tm);
			}
#ifdef NN_VERBOSE
			tm = "cycle layer: " + std::to_string(i);
			std::cout << "VERB:Network: " << tm << std::endl;
			tm = "inputs: ";
			for (std::size_t d=0; d<inputs.size(); ++d) {
				for (std::size_t e=0; e<inputs.at(d).size(); ++e) {
					tm = tm + std::to_string(inputs.at(d).at(e)) + "; ";
				}
				if (d != inputs.size()-1) tm = tm + "\t";
			}
			std::cout << "VERB:Network: " << tm << std::endl;
#endif
			// cycle
			outputs = layers.at(i).cycle(inputs);
#ifdef NN_VERBOSE
			tm = "outputs: ";
			for (std::size_t d=0; d<outputs.size(); ++d) {
				for (std::size_t e=0; e<outputs.at(d).size(); ++e) {
					tm = tm + std::to_string(outputs.at(d).at(e)) + "; ";
				}
				if (d != outputs.size()-1) tm = tm + "\t";
			}
			std::cout << "VERB:Network: " << tm << std::endl;
#endif
#ifdef NN_DEBUG
			std::cout << "DEBUG:Network: cycle layer complete" << std::endl;
#endif
		}
		// prepare output
		std::vector <double> outputs_simple;
		// simplify output vector
		for (std::size_t i=0; i<outputs.size(); ++i) {
			// output nodes will only have a value at the first index
			outputs_simple.push_back(outputs.at(i).at(0));
		}
#ifdef NN_DEBUG
		std::cout << "DEBUG:Network: cycle end" << std::endl;
#endif
		return outputs_simple;
	}

	// mutate (main) (semi-multi)(time)
	void Network::mutate() {
#ifdef NN_DEBUG
		std::cout << "DEBUG:Network: mutate start" << std::endl;
#endif
		for (std::size_t i=0; i<layers.size(); ++i) {
			layers.at(i).mutate();
		}
	}

	// export weights (side)
	std::vector <std::vector <std::vector <double> > > Network::export_weights()
	{
#ifdef NN_DEBUG
		std::cout << "DEBUG:Network: export weights start" << std::endl;
#endif
		std::vector <std::vector <std::vector <double> > > t;
		for (std::size_t i=0; i<layers.size(); ++i) {
			t.push_back(layers.at(i).export_weights());
		}
		return t;
	}

	// import weights (side)
	void Network::import_weights(std::vector <std::vector <std::vector <double> > > in)
	{
#ifdef NN_DEBUG
		std::cout << "DEBUG:Network: import weights start" << std::endl;
#endif
		if (in.size() != layers.size()) error_call("import_weights size mismatch");
		for (std::size_t i=0; i<in.size(); ++i) {
			layers.at(i).import_weights(in.at(i));
		}
	}

}

#endif
