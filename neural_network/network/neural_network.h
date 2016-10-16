
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
	//
	// note:
	// - add input one by one OR all at once in vector/scalar form
	//
	private:
		/// variables
		std::vector <double> weights;  // weight for each connection
		// generated weight min and max values
		double weight_gen_min;
		double weight_gen_max;
		// mutate mod and chance
		double mutate_mod;
		double mutate_chance;
		//std::vector <unsigned int> connections;
		bool bias;  // true for bias node (output 1 before weight)
		/// functions
		// generate weights randomly at start (sub)
		void generate_weights(unsigned int in_size)
		{
			weights.clear();
			if (in_size == 0)
			{ // output node - one output with no weight
				weights.push_back(1.0);
			}
			else
			{ // not output node - randomly generate weights
				for (std::size_t i = 0; i < in_size; ++i)
				{ // generate weights within weight_gen_min:weight_gen_max
					weights.push_back(LYRAND);
				}
			}
		}
		// average inputs (sub)
		double average_inputs(std::vector <double> in)
		{
			double t_avg = 0.0;
			for (std::size_t i=0; i<in.size(); ++i)
			{
				t_avg += in.at(i);
			}
			return t_avg / (double) in.size();
		}
		// activation function (sub)
		double activation_function(double in)
		{
			////////////////////////
			//return 1/(1+exp(-in));
			return tanh(in);
			////////////////////////
		}
		// generate outputs and apply weights (sub)
		std::vector <double> generate_outputs(double in)
		{
			std::vector <double> t_out;
			// output = out * weight
			for (std::size_t i=0; i<weights.size(); ++i)
			{
				t_out.push_back(in*weights.at(i));
			}
			return t_out;
		}
	public:
		/// functions
		// setup (main)
		// input: weight count for all outgoing connections, bias node option, connection IDs (NI)
		void setup(unsigned int in_w_count, bool in_bias, double in_mod, double in_chance) //, std::vector <int> in_conn)
		{
			// settings
			weight_gen_min = (-1);
			weight_gen_max = ( 1);
			// ---
			// set input options
			bias = in_bias;
			mutate_mod = in_mod;
			mutate_chance = in_chance;
			//connections = in_conn;
			// generate weights
			generate_weights(in_w_count);
		}
		// cycle (main)
		// cycle node - must have set inputs prior (see note and add_input)
		// return: outputs, one for each connection, with weights applied
		std::vector <double> cycle(std::vector <double> in)
		{
			double t;  // node output scaler
			if (!bias) t = activation_function(average_inputs(in));
			else t = 1;
			return generate_outputs(t);
		}
		// mutate (main)
		void mutate()
		{
			for (std::size_t i=0; i<weights.size(); ++i)
			{
				if (LYRAND > mutate_chance)
				{
					weights.at(i) = weights.at(i) + LYRAND*mutate_mod - weights.at(i)*mutate_mod;
				}
			}
		}
		std::vector <double> export_weights()
		{
			return weights;
		}
		void import_weights(std::vector <double> in)
		{
			weights = in;
		}
	};
	////////////////////////////////////
	class Layer
	{
	// note:
	// - in_n_count includes bias node
	// - in_next_n_count is 0 when output layer
	private:
		/// variables
		// vector of all nodes in this layer
		std::vector <Node> nodes;
		/// functions
		// call to report an error
		void error_call(std::string in)
		{
			std::cout << "Node::ERROR: " << in << std::endl;
			runtime_error = true;
			exit(0);
		}
	public:
		/// variables
		bool runtime_error;
		/// functions
		// create layer, ie setup (main)
		void setup(unsigned int in_n_count, unsigned int in_next_n_count, double in_mod, double in_chance)
		{
			// setup
			runtime_error = false;
			nodes.clear();
			// fill node vector with in_n_count nodes
			for (std::size_t i=0; i<in_n_count; ++i)
			{
				// last node bias if not an output
				//if (in_n_count != 0 && in_next_n_count != 0)
				if (i == (in_n_count-1) && in_next_n_count != 0)
				{
					Node n;
					n.setup(in_next_n_count, true, in_mod, in_chance);
					nodes.push_back(n);
				}
				else
				{
					Node n;
					// weight size = nodes in next layer
					n.setup(in_next_n_count, false, in_mod, in_chance);
					nodes.push_back(n);
				}
			}
		}
		// cycle (main)
		std::vector <std::vector <double> > cycle(std::vector <std::vector <double> > in)
		{
			// test
			for (std::size_t i=0; i<in.size(); ++i)
			{
				if (in.at(i).size() != nodes.size()) error_call("cycle - input size not equal to nodes size");
			}
			// outputs
			std::vector <std::vector <double> > t_out;  // outputs for all nodes
			for (std::size_t i=0; i<nodes.size(); ++i)
			{
				// cycle node, passing inputs, and saving output
				std::vector <double> t_in;
				for (std::size_t j=0; j<in.size(); ++j)
				{
					t_in.push_back(in.at(j).at((i)));
				}
				t_out.push_back(nodes.at(i).cycle(t_in));
			}
			return t_out;
		}
		// mutate (main)
		void mutate()
		{
			for (std::size_t i=0; i<nodes.size(); ++i)
			{
				nodes.at(i).mutate();
			}
		}
		// return nodes size (info)
		int info_node_count()
		{
			return nodes.size();
		}
		std::vector <std::vector <double> > export_weights()
		{
			std::vector <std::vector <double> > t;
			for (std::size_t i=0; i<nodes.size(); ++i)
			{
				t.push_back(nodes.at(i).export_weights());
			}
			return t;
		}
		void import_weights(std::vector <std::vector <double> > in)
		{
			if (in.size() != nodes.size()) error_call("import weights size mismatch");
			for (std::size_t i=0; i<in.size(); ++i)
			{
				nodes.at(i).import_weights(in.at(i));
			}
		}
	};
	////////////////////////////////////
	class Network
	{
	private:
		/// variables
		bool runtime_error;
		std::vector <Layer> layers;
		/// functions
		//
		void error_call(std::string in)
		{
			std::cout << "Network::ERROR: " << in << std::endl;
			runtime_error = true;
			exit(0);
		}
		//
		void debug_call(std::string in)
		{
			std::cout << "debug: network::" << in << std::endl;
		}
		//
		void print_intro()
		{
			std::cout << std::endl;
			std::cout << "Neural Network" << std::endl;
			std::cout << "---------------------" << std::endl;
		}
		//
		void create_network(std::vector <unsigned int> in_npl, double in_mod, double in_chance)
		{
			if (debug) debug_call("create network start");
			// add bias node to all layers but the output
			std::vector <unsigned int> t_nc = in_npl;
			for (std::size_t i = 0; i<(t_nc.size()-1); ++i)
			{
				t_nc.at(i) = (t_nc.at(i) + 1);
			}
			// create layers
			for (std::size_t i = 0; i < t_nc.size(); ++i)
			{
				unsigned int t_nnc;  // next node count
				// if output layer
				if (i == (t_nc.size()-1)) t_nnc = 0;
				else t_nnc = t_nc.at(i+1);
				//
				if (debug)
				{
					std::cout << "Create layer: "  << i << "\t";
					std::cout << "nodes: " << t_nc.at(i) << "\t";
					std::cout << "connections: " << t_nnc << std::endl;
				}
				//
				Layer l;
				l.setup(t_nc.at(i), t_nnc, in_mod, in_chance);
				layers.push_back(l);
			}
			if (debug) debug_call("create_network end");
		}
	public:
		/// variables
		bool debug;
		unsigned int ID_value;
		/// functions
		// constructor
		Network()
		{
			debug = false;
			runtime_error = false;
			srand(time(0));
			//print_intro();
		}
		// setup (main)
		void setup(std::vector <unsigned int> in_npl, double in_m, double in_c)
		{
			if (debug) debug_call("setup start");
			create_network(in_npl, in_m, in_c);
			if (debug) debug_call("setup end");
		}

		// cycle network (main)
		std::vector <double> cycle(std::vector <double> in)
		{
			if (debug) debug_call("cycle start");
			std::vector <std::vector <double> > outputs;
			for (std::size_t i=0; i<layers.size(); ++i)
			{
				std::vector <std::vector <double> > inputs;
				// input layer
				if (i == 0)
				{
					// format given inputs
					std::vector <double> t;
					for (std::size_t x=0; x<in.size(); ++x)
					{
						t.push_back(in.at(x));
					}
					t.push_back(1.0);
					inputs.push_back(t);  // bias node
				}
				else inputs = outputs;
				// cycle
				if (debug) debug_call("cycle layer");
				outputs = layers.at(i).cycle(inputs);
			}

			std::vector <double> outputs_simple;
			// simplify inputs vector to create outputs
			for (std::size_t i=0; i<outputs.size(); ++i)
			{  // output nodes will only have a value at the first index
				outputs_simple.push_back(outputs.at(i).at(0));
			}
			if (debug) debug_call("cycle end");
			return outputs_simple;
		}
		// mutate (main)
		void mutate()
		{
			for (std::size_t i=0; i<layers.size(); ++i)
			{
				layers.at(i).mutate();
			}
		}
		std::vector <std::vector <std::vector <double> > > export_weights()
		{
			std::vector <std::vector <std::vector <double> > > t;
			for (std::size_t i=0; i<layers.size(); ++i)
			{
				t.push_back(layers.at(i).export_weights());
			}
			return t;
		}
		void import_weights(std::vector <std::vector <std::vector <double> > > in)
		{
			if (in.size() != layers.size()) error_call("import_weights size mismatch");
			for (std::size_t i=0; i<in.size(); ++i)
			{
				layers.at(i).import_weights(in.at(i));
			}
		}
	};
}
