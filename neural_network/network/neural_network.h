
// Neural Network
// Copyright (C) William 2016.  All rights reserved.

#include <iostream>  // cout, endl
#include <vector>  // .. vectors
#include <stdlib.h> // srand, rand, exit
#include <cstdlib> // RAND_MAX
#include <time.h> // time for srand and delta times
#include <math.h> // exp - sigmoid
#include <string>  // error call

#define LYRAND (double)rand()/RAND_MAX

// ideas: create error class or just merge them all into one in the network class

namespace Network {
	class Node
	{
	private:
		/// variables
		std::vector <double> weights;  // weight for each connection
		// generated weight allowed min and max values
		double weight_gen_min;
		double weight_gen_max;
		// mutate mod and chance
		double mutate_mod;
		double mutate_chance;
		//std::vector <unsigned int> connections;  // for selectively connected
		bool bias;  // true for bias node (output 1 before weight)
		unsigned int layer_type;  // 0:input, 1:hidden, 2:output
		unsigned int layer_number;  // layer 1 for input, N for output, else hidden

		/// functions

		// generate weights randomly at start (sub)
		// input: number of weights to generate
		// output: populates weight vector
		void generate_weights(unsigned int in_size) {
			weights.clear();
			// not output node - randomly generate weights
			for (std::size_t i = 0; i < in_size; ++i) {
				// generate weights within weight_gen_min:weight_gen_max
				weights.push_back(LYRAND);
			}
		}

		// sum inputs (sub)
		// input: all inputs to node
		// output: summation of inputs
		double sum_inputs(std::vector <double> in) {
			double t_avg = 0.0;
			for (std::size_t i=0; i<in.size(); ++i) {
				t_avg += in.at(i);
			}
			return t_avg;
			//return t_avg / (double) in.size();
		}

		// activation function (sub)
		// input: scaler input
		// output: input after activation function
		double activation_function(double in) {
			//return 1/(1+exp(-in));  // sigmoid
			return tanh(in);  // hyperbolic tangent
		}

		// generate outputs and apply weights (sub)
		// input: scaler network input after activation function
		// output: vector, scaler times connection weight
		std::vector <double> generate_outputs(double in) {
			std::vector <double> t_out;
			if (layer_type == 2) {
				// output layer - no weights
				t_out.push_back(t);
				return t_out;
			} else {
				// output = out * weight(i)
				for (std::size_t i=0; i<weights.size(); ++i) {
					t_out.push_back(in*weights.at(i));
				}
			}
			return t_out;
		}

	public:
		/// functions

		// setup (main)
		// input: node settings
		void setup(	unsigned int in_w_count,  // in weight count
					bool in_bias,
					double in_mod,
					double in_chance,
					unsigned int in_type,
					unsigned int in_num ) {
			// settings
			weight_gen_min = (-1);
			weight_gen_max = ( 1);
			// settings end
			// set input options
			bias = in_bias;
			mutate_mod = in_mod;
			mutate_chance = in_chance;
			layer_type = in_type;
			layer_num = in_num;
			// generate weights except for output layer
			if (layer_type != 2) generate_weights(in_w_count);
		}

		// cycle (main)
		// input: vector of inputs to node
		// output: outputs, one for each connection, with weights applied
		// note: how to deal with cycle of bias node
		std::vector <double> cycle(std::vector <double> in) {
			double t_out;  // node output scaler
			if (!bias) {
				if (layer_type == 0) activation_function(in.at(0)); // input layer
				else t_out = activation_function(sum_inputs(in));  // not input layer
			}
			else t_out = 1.0;  // bias node (output 1.0)
			return generate_outputs(t_out);
		}

		// mutate (main)
		// mutates the node using mod and chance
		void mutate() {
			for (std::size_t i=0; i<weights.size(); ++i) {
				if (LYRAND > mutate_chance) {
					// weight = weight + LYRAND*mod - LYRAND*mod
					weights.at(i) = weights.at(i) + LYRAND*mutate_mod \
									- LYRAND*mutate_mod;
				}
			}
		}

		// export weights (side)
		std::vector <double> export_weights() {
			return weights;
		}

		// import weights (side)
		void import_weights(std::vector <double> in) {
			weights = in;
		}
	};
	////////////////////////////////////
	class Layer
	{
	private:
		/// variables
		// vector of all nodes in this layer
		std::vector <Node> nodes;
		/// functions
		// call to report an error
		void error_call(std::string in) {
			std::cout << "ERROR:Layer: " << in << std::endl;
			runtime_error = true;
			exit(0);
		}
	public:
		/// variables
		bool runtime_error;

		/// functions

		// setup layer (main)
		void setup( unsigned int in_n_count,  // nodes in layer
					unsigned int in_next_n_count,  // nodes in next layer
					double in_mod,
					double in_chance,
					unsigned int in_type,  // layer type
					unsigned int in_num ) {
			// setup
			runtime_error = false;
			nodes.clear();
			// fill node vector with in_n_count nodes
			for (std::size_t i=0; i<in_n_count; ++i) {
				// note:
				// - weight size = nodes in next layer
				// - in_n_count includes bias node
				// - in_next_n_count is 0 when output layer

				// last node bias if not an output layer
				if (i == (in_n_count-1) && in_type != 2) {  // bias
					Node n;
					n.setup(in_next_n_count, true, in_mod, in_chance, in_type, in_num);
					nodes.push_back(n);
				} else {  // normal
					Node n;
					n.setup(in_next_n_count, false, in_mod, in_chance, in_type, in_num);
					nodes.push_back(n);
				}
			}
		}

		// cycle layer (main)
		// input: all inputs for nodes in layer
		// output: all outputs from all nodes in layer
		// format note: < <node1>, <node2>, ..., <nodeN> >
		std::vector <std::vector <double> > cycle(std::vector <std::vector <double> > in) {
			// test
			for (std::size_t i=0; i<in.size(); ++i) {
				if (in.at(i).size() != nodes.size()) error_call("cycle - input size not equal to nodes size");
			}
			// cycle
			std::vector <std::vector <double> > t_out;  // outputs from all nodes
			std::vector <double> t_in;  // temp input vector for node i
			for (std::size_t i=0; i<nodes.size(); ++i) {
				// cycle node, passing inputs, and saving outputs
				t_in = in.at(i);
				t_out.push_back(nodes.at(i).cycle(t_in));
			}
			return t_out;
		}

		// mutate (main)
		// call all nodes mutate function
		void mutate() {
			for (std::size_t i=0; i<nodes.size(); ++i) {
				nodes.at(i).mutate();
			}
		}

		// export weights (side)
		std::vector <std::vector <double> > export_weights() {
			std::vector <std::vector <double> > t;
			for (std::size_t i=0; i<nodes.size(); ++i) {
				t.push_back(nodes.at(i).export_weights());
			}
			return t;
		}

		// import weights (side)
		void import_weights(std::vector <std::vector <double> > in) {
			if (in.size() != nodes.size()) error_call("import weights size mismatch");
			for (std::size_t i=0; i<in.size(); ++i) {
				nodes.at(i).import_weights(in.at(i));
			}
		}
	};
	////////////////////////////////////
	class Network
	{
	private:
		/// variables
		std::vector <Layer> layers;
		bool runtime_error;
		/// functions
		// 
		void error_call(std::string in) {
			std::cout << "ERROR:Network: " << in << std::endl;
			runtime_error = true;
			exit(0);
		}
		// 
		void debug_call(std::string in) {
			std::cout << "DEBUG:network: " << in << std::endl;
		}
		//
		void print_intro() {
			std::cout << std::endl;
			std::cout << "Neural Network" << std::endl;
			std::cout << "---------------------" << std::endl;
		}

		// create network (sub)
		// input: nodes per layer (excluding bias), mutate mod and chance
		void create_network(std::vector <unsigned int> in_npl, double in_mod, double in_chance)
		{
			if (debug) debug_call("create network start");
			// add bias node to all layers but the output
			std::vector <unsigned int> t_nc = in_npl;
			for (std::size_t i = 0; i<(t_nc.size()-1); ++i) {
				t_nc.at(i) = (t_nc.at(i) + 1);
			}
			npl = t_nc;  // set node count variable for reference
			// create layers
			for (std::size_t i = 0; i < t_nc.size(); ++i) {
				// next node count
				unsigned int t_nnc;
				if (i == (t_nc.size()-1)) t_nnc = 0;  // output layer
				else t_nnc = t_nc.at(i+1);
				// layer type
				unsigned int t_type;
				if (i == 0) t_type = 0;  // input
				else if (i == (t_nc.size()-1)) t_type = 2;  // output
				else t_type = 1;  // hidden
				// debug
				if (debug) {
					std::cout << "Create layer: "  << i << ";\t";
					std::cout << "nodes: " << t_nc.at(i) << ";\t";
					std::cout << "connections: " << t_nnc << ";" << std::endl;
				}
				// layer
				Layer l;
				l.setup(t_nc.at(i), t_nnc, in_mod, in_chance, t_type, i+1);
				layers.push_back(l);
			}
			if (debug) debug_call("create_network end");
		}
	public:
		/// variables
		bool debug;
		bool run_type;  // 0:single, 1:train
		unsigned int ID_value;
		std::vector <unsigned int> npl;  // including bias

		/// functions

		// constructor
		Network() {
			debug = false;
			runtime_error = false;
			ID_value = 0;
			run_type = 0;
			srand(time(0));
		}

		// setup (main)
		void setup(std::vector <unsigned int> in_npl, double in_m, double in_c) {
			if (debug) debug_call("setup start");
			create_network(in_npl, in_m, in_c);
			if(run_type == 0) print_intro();
			if (debug) debug_call("setup end");
		}

		// cycle network (main) (time)
		// input: input vector (one double per input node)
		// output: network output (one double per output node)
		std::vector <double> cycle(std::vector <double> in) {
			if (debug) debug_call("cycle start");
			std::vector <std::vector <double> > inputs;
			std::vector <std::vector <double> > outputs;
			for (std::size_t i=0; i<layers.size(); ++i) {
				// input layer
				if (i == 0) {
					inputs.clear();
					// format given inputs
					std::vector <double> t_in;
					for (std::size_t x=0; x<in.size(); ++x) {
						t_in.push_back(in.at(x));
						inputs.push_back(t_in);
					}
					inputs.push_back(1.0);  // bias node
				}
				else inputs = outputs;
				// test
				if (inputs.size() != layers.at(i).size()) error_call("input size mismatch nodes in layer");
				// cycle
				if (debug) debug_call("cycle layer");
				outputs = layers.at(i).cycle(inputs);
			}
			// prepare output
			std::vector <double> outputs_simple;
			// simplify output vector
			for (std::size_t i=0; i<outputs.size(); ++i) {
				// output nodes will only have a value at the first index
				outputs_simple.push_back(outputs.at(i).at(0));
			}
			if (debug) debug_call("cycle end");
			return outputs_simple;
		}

		// mutate (main) (time)
		void mutate() {
			for (std::size_t i=0; i<layers.size(); ++i) {
				layers.at(i).mutate();
			}
		}

		// export weights (side)
		std::vector <std::vector <std::vector <double> > > export_weights()
		{
			std::vector <std::vector <std::vector <double> > > t;
			for (std::size_t i=0; i<layers.size(); ++i) {
				t.push_back(layers.at(i).export_weights());
			}
			return t;
		}

		// import weights (side)
		void import_weights(std::vector <std::vector <std::vector <double> > > in)
		{
			if (in.size() != layers.size()) error_call("import_weights size mismatch");
			for (std::size_t i=0; i<in.size(); ++i) {
				layers.at(i).import_weights(in.at(i));
			}
		}
	};
}
