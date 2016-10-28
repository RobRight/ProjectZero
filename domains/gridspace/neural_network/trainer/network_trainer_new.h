
/*

Network Trainer
Version 3.0

Created by William Gregory on October 27th to present.
Copyright (c) William Gregory.  All rights reserved.

idea: generate a new domain for each test.

*/

#ifndef _NETWORK_TRAINER_
#define _NETWORK_TRAINER_

//#define NT_DEBUG
//#define NT_VERBOSE
//#define NT_TEST

#define PI 3.14159265

#include "../../gridspace.h"  // domain
#include "../network/neural_network.h"

namespace Trainer
{
	class Trainer
	{
	private:
        std::vector <Network::Network> population;
        std::vector <double> best_fitness;
        std::vector <Network::Network> best_network;
        unsigned int current_round;
        //unsigned int current_sub_round;
        bool runtime_error;

        void print_intro();
        void print_end();

        std::vector <double> get_state();
        void give_action(std::vector <double>&);
        double get_reward();

        double test(Network::Network);  // test net sub_round times, return total reward
        std::vector <Network::Network> prune(std::vector <Network::Network>&, std::vector <double>&);
        std::vector <Network::Network> populate(std::vector <Network::Network>&, unsigned int&);
    public:
        unsigned int round_max;
        //unsigned int round_sub_max;
        unsigned int population_size;
        std::vector <unsigned int> nodes_per_layer;

        Trainer();
        void train();
    };

    Trainer::Trainer() {

        // do not modify
        runtime_error = false;
        current_round = 0;
        //current_sub_round = 0;
    }

    void Trainer::print_intro() {
        std::cout << std::endl;
        std::cout << "Network Trainer" << std::endl;
        std::cout << "version 3.0" << std::endl;
        std::cout << "--------------------" << std::endl;
        std::cout << std::endl;
    }

    void Trainer::print_end() {
        std::cout << std::endl;
        std::cout << "--------------------" << std::endl;
        std::cout << "training complete" << std::endl;
        std::cout << std::endl;
    }

    //----------------------------
    // get state from domain
    std::vector <double> get_state() {
        std::vector <double> t_out;
        t_out = domain.give_state();
        return t_out;
    }

    // give action to domain
    void give_action(std::vector <double>& in) {
        domain.get_action(in);
    }

    double get_reward() {
        double t_out;
        domain.get_reward();
        return t_out;
    }
    //-----------------------------

    Grid new_domain() {
        Grid g;
        g.play_game(10, 10, false);
        return g;
    }

    double test() {
        std::vector <double> state;
		std::vector <double> action;
        double reward;
        for (std::size_t i=0; i<sub_round; ++i) {
            Grid domain = new_domain();
            state = domain.give_state();
		    action = cycle_network(state, population);
            domain.get_action(action);
            reward += domain.give_reward();
        }
        return reward;
    }

    //
	// prune: main
	// prune a population using binary selection
	//
	// input:
	// - population to prune
	// - population errors
	// output:
	// - pruned population
	//
    std::vector <Network::Network> Trainer::prune(std::vector <Network::Network>& in_pop, std::vector <double>& in_pop_error) {
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
    std::vector <Network::Network> Trainer::populate(std::vector <Network::Network>& in_pop, unsigned int& in_pop_size) {
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
		return temp_new_pop;
    }

	//
	// train: main
	// main training loop
	//
    void Trainer::train() {
#ifdef NT_DEBUG
		std::cout << "debug: train() start" << std::endl;
#endif
		print_intro();
		generate_population();
		clock_t time_start = clock();
		while (round <= round_max && !runtime_error)
		{
#ifdef NT_DEBUG
			std::cout << "debug: round " << round << "; sub " << sub_round << " start" << std::endl;
#endif
            for (std::size_t i=0; i<population.size(); ++i) {
                test(population.at(i));
            }
			prune();
            populate();
        }
        double delta_time = (clock() - time_start); // clocks per second?
    }
}