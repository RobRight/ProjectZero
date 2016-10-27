/*
* Neural Network Trainer
*
* Version 2.1
*
* Creaded by William on July 8th, 2016 to Present
* Copyright (C) 2016 William.  All rights reserved
*
*/


/*
* To use:
* call setup()
* change params
* call train()
*
* Node:
* - train functions:
*	(0:x^2+1, 1:step, 2:exp(x), 3:sin(x), 4:cos(x), 5:const, 6:sqrt(x)+1, 7:-x)
*
* ToDo:
*	- add sweeping function to determine best values for...
*		- hidden_layer_size, mutate_mod, mutate_chance, sub_round_max, population_size
*	- add setting for activation type
*	- add setting for function to train
*	- add progress info (an updating bar would be cool)
*
*/

#ifndef _NETWORK_TRAINER_
#define _NETWORK_TRAINER_

//#define NT_DEBUG
//#define NT_VERBOSE
//#define NT_TEST

#define PI 3.14159265

//#include <iostream>
//#include <vector>
//#include <cmath>
//#include <time.h>
//#include <string>
//#include <fstream> // file io
//#include <stdlib.h> // system

#include "../network/neural_network.h"

namespace Trainer
{
	class Trainer
	{
	private:
		// - variables
		// will be true if there is an error
		bool runtime_error;
		// collection of networks being trained
		std::vector <Network::Network> population;
		// nodes in each layer
		std::vector <unsigned int> nodes_per_layer;
		// inputs for the network this round
		std::vector <double> net_inputs;
		// actual outputs from the network this round
		std::vector <std::vector <double> > net_outputs;
		// unscaled net_inputs
		std::vector <double> net_inputs_raw;
		// sum of sub round errors for each network
		std::vector <double> sub_pop_round_errors;
		// avg of sub round errors for each network in population
		std::vector <double> round_pop_errors;
		// round average error of whole population
		double round_pop_avg_error;
		// first round average error
		double starting_avg_error;
		// current round	
		unsigned int round;
		// current sub round
		unsigned int sub_round;
		// current best error
		double best_error;
		// current best network
		Network::Network best_network;
		// next unique ID for network
		unsigned int ID_next;
		// script start time
		clock_t time_start;
		// script run time
		double time_delta;
		// input size
		unsigned int input_state_size;

		// - functions
		void error_call(std::string);
		void print_intro();
		void print_round();
		void print_end();
		void export_error(double, bool);
		void progress();
		void generate_population();
		Network::Network generate_network();
		double scale_value(double, bool, bool);
		std::vector <std::vector <double> > cycle_network(std::vector <double>&, std::vector <Network::Network>&);
		void check_population_error(bool);
		std::vector <Network::Network> prune(std::vector <Network::Network>&, std::vector <double>&);
		std::vector <Network::Network> populate(std::vector <Network::Network>&, unsigned int&);

	public:
		// - variables
		// network input min value
		double input_min;
		// network input max value
		double input_max;
		// round max
		unsigned int round_max;
		// sub round max
		unsigned int sub_round_max;
		// max population size
		unsigned int population_size;
		// nodes in hidden layer
		unsigned int hidden_layer_size;
		// network mutation coefficient 
		double mutate_mod;
		// chance for mutating
		double mutate_chance;
		// train_func_type
		unsigned int train_func_type;

		// - functions
		Trainer();
		void train();
	};


	Trainer::Trainer()
	{
		// settings
		train_func_type = 0;
		population_size = 100;  // must be even and positive
		round_max = 400;  // must greator than zero
		sub_round_max = 60;  // must be greater than zero
		input_min = 0;  // 0
		input_max = 5;  // max grid size
		hidden_layer_size = 4;  // must be greater than zero
		mutate_mod = 0.1; // between zero and one
		mutate_chance = 0.5; // between zero and one
		input_state_size = 2; // dx, dy
		// settings end
		srand(time(NULL));
		// do not modify
		runtime_error = false;
		round = 0;
		sub_round = 0;
		nodes_per_layer = { input_state_size, hidden_layer_size, 4 };
		best_error = HUGE_VAL;
		ID_next = 1;
	}

	void Trainer::error_call(std::string in_message)
	{
		runtime_error = true;
		std::cout << "error: " << in_message << std::endl;
	}

	// general training info at start
	void Trainer::print_intro()
	{
		bool fancy = false;
		if (fancy) system("clear");
		std::cout << std::endl;
		std::cout << "------------------------" << std::endl;
		std::cout << "Network Trainer" << std::endl;
		std::cout << "by " << "WG" << std::endl;
		std::cout << std::endl;
		std::cout << "population size: " << population_size << std::endl;
		std::cout << "max round: " << round_max << std::endl;
		std::cout << "hidden nodes: " << hidden_layer_size << std::endl;
		std::cout << "mutate mod: " << mutate_mod << std::endl;
		std::cout << "input min: " << input_min << std::endl;
		std::cout << "input max: " << input_max << std::endl;
		std::cout << std::endl;
	}

	// current training info after each round
	void Trainer::print_round()
	{
		std::cout << "round: " << round << std::endl;
		//std::cout << "sub round: " << sub_round << std::endl;
		std::cout << "best_error: " << best_error << std::endl;
		std::cout << "average error: " << round_pop_avg_error << std::endl;
		std::cout << std::endl;
	}

	// training info after complete
	void Trainer::print_end()
	{
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << "Training complete" << std::endl;
		if (time_delta > 60)
		{
			// convert to min and seconds
			int temp_min = (int)(time_delta / 60);
			int temp_sec = (int)(time_delta - temp_min * 60);
			std::cout << "time running: " << temp_min << " min " << temp_sec << " sec" << std::endl;
		}
		else {
			std::cout << "time running: " << time_delta << " seconds " << std::endl;
		}
		std::cout << "ending best error: " << best_error << std::endl;
		std::cout << "starting avgerage error: " << starting_avg_error << std::endl;
		std::cout << "ending average error: " << round_pop_avg_error << std::endl;
		std::cout << "------------------------" << std::endl;
		std::cout << std::endl;
	}
 
	void Trainer::export_error(double in, bool in_clear)
	{
		std::ofstream file;
		std::string filename = "output_results/avg_error.txt";
		if (in_clear) file.open(filename);
		else file.open(filename, std::ofstream::app);
		file << in << "\n";
	}

	//
	// progress: sub
	//
	void Trainer::progress()
	{
		unsigned int update_interval = 100.0;  // setting
		double progress = (double)round/round_max;
		unsigned int t_progress = int(progress*10000);
		if (t_progress % update_interval == 0) {
			
			unsigned int bar_width = 40;  // setting
			std::cout << "[";
			unsigned int bar_pos = bar_width * progress;
			unsigned int bar_pos_em = bar_width - bar_pos;
			for (std::size_t i=1; i<=bar_pos; ++i) {
				if (i < bar_pos) std::cout << "=";
				else std::cout << ">";
			}
			for (std::size_t i=0; i<bar_pos_em; ++i) {
				std::cout << " ";
			}
			std::cout << "] " << int(progress * 100) << "%" << "\r";
			std::cout.flush();
		}
	}

	//
	// generate_population: sub
	// generate starting population
	//
	void Trainer::generate_population()
	{
		population.clear();
		for (std::size_t i=0; i<population_size; ++i) {
			population.push_back(generate_network());
		}
	}

	//
	// generate_network: sub
	// generate a network, setup and return
	//
	// output:
	// - output generally configured network
	//
	Network::Network Trainer::generate_network()
	{
		Network::Network net;

		net.ID_value = ID_next;
		net.run_type = 1;
		net.setup(nodes_per_layer, mutate_mod, mutate_chance);
		
		ID_next++;
		return net;
	}

	//
	// generate_value: sub
	// generate a single network input value within allowed range
	//
	// uses: input_min and input_max
	// output:
	// - network input value
	double Trainer::generate_value()
	{
#ifdef NT_DEBUG
		std::cout << "generate_value()" << std::endl;
#endif
		return (double)input_min + LYRAND*(input_max - input_min);
	}

	//
	// scale_value: sub
	// scales raw values to allowed network values (ie <= 1)
	// descales scaled values
	//
	// note: this may not be the best method
	// input:
	// - value to scale
	// - option: (true for input, false for output)
	// - option: (true for scale, false for descale)
	// output:
	// - scaled value
	//
	double Trainer::scale_value(double in_val, bool in_op, bool in_scale)
	{
#ifdef NT_DEBUG
		std::cout << "scale_value()" << std::endl;
#endif
		if (in_scale)
		{
			if (in_op)
			{
				return (in_val / input_max);
			}
			else {
				return (in_val / train_function(input_max));
			}
		}
		else {
			if (in_op)
			{
				return (in_val*input_max);
			}
			else {
				return (in_val*train_function(input_max));
			}
		}
	}

	//
	// cycle_network: main, sub
	// cycle given networks with given inputs and return outputs
	//
	// note: seperate function for cycling one network
	// input:
	// - vector of inputs, one per input node
	// - vector of networks to cycle
	// output: vector (size of given population) vector (size of output node count)
	// - vector (size of given population) <vector (size of output node count) <double> >
	// - - vector of vector outputs
	// - - each network returns a vector of outputs, one per node
	//
	std::vector <std::vector <double> > Trainer::cycle_network(std::vector <double>& in_net_inputs, std::vector <Network::Network>& in_population)
	{
#ifdef NT_DEBUG
		std::cout << "debug: cycle_network() start" << std::endl;
#endif
		// set inputs and cycle
		std::vector <std::vector <double> > temp_net_outputs;
		for (std::size_t i = 0; i < in_population.size(); ++i)
		{
			temp_net_outputs.push_back(in_population.at(i).cycle(in_net_inputs));
		}
		return temp_net_outputs;
	}

	//
	// check_population_error: main
	// check all networks error in population
	//
	// note: add population input and return error vector
	// input:
	// - option: (true for subround, false for round)
	// output:
	//  
	void Trainer::check_population_error(bool is_sub_round)
	{
#ifdef NT_DEBUG
		std::cout << "debug: check_population_error() start" << std::endl;
#endif
		if (is_sub_round)
		{
			// first sub round setup
			if (sub_round == 0)
			{
				sub_pop_round_errors.clear();
				for (std::size_t i = 0; i < population_size; ++i)
				{
					sub_pop_round_errors.push_back(0.0);
				}
			}
			// sub round error
			for (std::size_t i = 0; i < population_size; ++i)
			{
				sub_pop_round_errors.at(i) += given_reward.at(i);
			}
			given_reward.clear();
			// round error
		}
		else {
			// reset round variables
			round_pop_errors.clear();
			round_pop_avg_error = 0.0;
			// take average of sub_round error
			double temp_round_pop_avg_error = 0.0;
			for (std::size_t i = 0; i < sub_pop_round_errors.size(); ++i)
			{
				double temp_error = sub_pop_round_errors.at(i) / sub_round_max;
				round_pop_errors.push_back(temp_error);
				temp_round_pop_avg_error += temp_error;
				// network best error
				if (temp_error < best_error)
				{
					// add: save network weights
					best_error = temp_error;
					best_network = population.at(i);
				}
			}
			round_pop_avg_error = temp_round_pop_avg_error / population_size;
			switch (round) {
				case 0:
					starting_avg_error = round_pop_avg_error;
			}
		}
	}

	//
	// prune: main
	// prune a population using binary selection
	//
	// idea: check average error after and it should be better than the starting average error
	// input:
	// - population to prune
	// - population errors
	// output:
	// - pruned population
	//
	std::vector <Network::Network> Trainer::prune(std::vector <Network::Network>& in_pop, std::vector <double>& in_pop_errors)
	{
#ifdef NT_DEBUG
		std::cout << "debug: prune() start" << std::endl;
#endif
		std::vector <Network::Network> temp_new_pop;
		double temp_avg_error = 0.0; // average error of new pop
		for (std::size_t i = 0; i < in_pop.size() / 2; ++i)
		{
			int try_1 = rand() % in_pop.size();
			int try_2 = rand() % in_pop.size();
			while (try_1 == try_2)
			{
				try_2 = rand() % in_pop.size();
			}
			if (in_pop_errors.at(try_1) < in_pop_errors.at(try_2)) {
				temp_new_pop.push_back(in_pop.at(try_1));
				temp_avg_error += round_pop_errors.at(try_1);
			}
			else {
				temp_new_pop.push_back(in_pop.at(try_2));
				temp_avg_error += round_pop_errors.at(try_2);
			}
		}
#ifdef NT_TEST
		if (temp_new_pop.size() != in_pop.size() / 2)
		{
			error_call("prune() output size wrong");
		}
#endif
#ifdef NT_VERBOSE
			temp_avg_error = temp_avg_error / temp_new_pop.size();
			std::cout << "prune results: " << std::endl;
			std::cout << " - start: " << round_pop_avg_error << std::endl;
			std::cout << " - end: " << temp_avg_error << std::endl;
			double prune_improvement = round_pop_avg_error - temp_avg_error;
			std::cout << " - improvement: " << prune_improvement << std::endl;
#endif
		//if (temp_avg_error > round_pop_avg_error)
		//{
			//error_call("prune() average error got worse");
		//}
		return temp_new_pop;
	}

	//
	// populate: main
	// populate a population with mutations of the existing
	//
	// note: seperate network generation
	// input:
	// - population to populate
	// - desired population size
	// output:
	// - populated population
	//
	std::vector <Network::Network> Trainer::populate(std::vector <Network::Network>& in_pop, unsigned int& in_pop_size)
	{
#ifdef NT_DEBUG
		std::cout << "debug: populate() start" << std::endl;
#endif
#ifdef NT_TEST
		if (in_pop.size() < 2)
		{
			error_call("populate() input pop too small");
		}
		if (in_pop_size <= in_pop.size())
		{
			error_call("populate() input pop and desired size mismatch");
		}
#endif
		// populate until size is 'in_pop_size'
		std::vector <Network::Network> temp_new_pop = in_pop;
		while (temp_new_pop.size() < in_pop_size)
		{
			int copy_index = rand() % in_pop.size();
			// generate a new network
			Network::Network net = generate_network();
			// set to same weight as random
			net.import_weights(in_pop.at(copy_index).export_weights());
			net.mutate();
			temp_new_pop.push_back(net);
		}
#ifdef NT_TEST
		if (temp_new_pop.size() != in_pop_size)
		{
			error_call("populate() output population size wrong");
		}
#endif
		return temp_new_pop;
	}

	//
	// train: main
	// main training loop
	//
	void Trainer::train()
	{
#ifdef NT_DEBUG
		std::cout << "debug: train() start" << std::endl;
#endif
		print_intro();
		generate_population();
		time_start = clock();
		while (round <= round_max && runtime_error == false)
		{
#ifdef NT_DEBUG
			std::cout << "debug: round " << round << "; sub " << sub_round << " start" << std::endl;
#endif
			get_state();
			// cycle network
			net_outputs.clear();
			net_outputs = cycle_network(net_inputs, population);
			// check error
			check_population_error(true); // true: subround
			sub_round++;
			if (sub_round == sub_round_max)
			{
				progress();
				check_population_error(false); // false: round
				population = prune(population, round_pop_errors);
				population = populate(population, population_size);
				//
				switch (round) {
					case 0:
						export_error(round_pop_avg_error, true);
						break;
					default:
						export_error(round_pop_avg_error, false);
				}
				// round complete
				round++;
				sub_round = 0;
			}
#ifdef NT_VERBOSE
			print_round();
#endif
#ifdef NT_DEBUG
			std::cout << "debug: round " << round << ";sub " << sub_round << " complete" << std::endl;
#endif
		}
		// training complete
		time_delta = double(clock() - time_start) / CLOCKS_PER_SEC;
#ifdef NT_DEBUG
		std::cout << "debug: train() end" << std::endl;
#endif
		if (runtime_error) std::cout << "error: runtime error triggered" << std::endl;
		else
		{
			print_end();
		}
	}

}

#endif
