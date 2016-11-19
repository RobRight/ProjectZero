
/*

Network Trainer
Version 3.0

Created by William Gregory on October 27th to present.
Copyright (c) William Gregory.  All rights reserved.

*/

#ifndef _NETWORK_TRAINER_
#define _NETWORK_TRAINER_

//#define NT_DEBUG

#define PI 3.14159265

#include "../network/neural_network.h"  // agent
#include "domain.h"  // domain

namespace Trainer
{
	class Trainer
	{
	private:
        std::vector <Network::Network> population;
        std::vector <double> best_fitness;
        std::vector <Network::Network> best_network;
        unsigned int current_round;
		unsigned int ID_next;

		std::vector <double> last_state;
		std::vector <double> last_action;
		double last_fitness;
		std::vector <double> pop_fitness;

        void print_intro();
        void print_end();

        void get_state(std::vector <double>&);
        std::vector <double> give_action();
        void get_reward(double);

        std::vector <Network::Network> prune(std::vector <Network::Network>&, std::vector <double>&);
        std::vector <Network::Network> populate(std::vector <Network::Network>&, unsigned int&);
    public:
        unsigned int round_max;
        unsigned int population_size;
        std::vector <unsigned int> nodes_per_layer;
		double mutate_mod;
		double mutate_chance;

        Trainer();
        void train();
    };

    Trainer::Trainer() {
        // do not modify
        runtime_error = false;
        current_round = 0;
		ID_next = 1;
		// settings
		round_max = 1000;
		population_size = 100;
	    hidden_layer_size = 4;
	    mutate_mod = 0.1;
	    mutate_chance = 0.3;
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
    void get_state(std::vector <double>& in) {
		state = in;
    }

    // give action to domain
    std::vector <double> give_action() {
		return 
    }

    void get_reward(double in_val) {
		last_fitness = in_val;
    }
    //-----------------------------

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
	void Trainer::cycle() {

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
		int try_1;
		int try_2;
		std::vector <Network::Network> temp_new_pop;
		for (std::size_t i = 0; i < in_pop.size() / 2; ++i)
		{
			try_1 = rand() % in_pop.size();
			try_2 = rand() % in_pop.size();
			while (try_1 == try_2)
				try_2 = rand() % in_pop.size();
			if (in_pop_error.at(try_1) < in_pop_error.at(try_2))
				temp_new_pop.push_back(in_pop.at(try_1));
			else
				temp_new_pop.push_back(in_pop.at(try_2));
		}
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
		int copy_index;
		Network::Network net;
		std::vector <Network::Network> temp_new_pop = in_pop;
		while (temp_new_pop.size() < in_pop_size)
		{
			copy_index = rand() % in_pop.size();
			// generate a new network
			net = generate_network();
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
				generate_domain();
				cycle();
			}
			prune();
            populate();
        }
        double delta_time = (clock() - time_start) / CLOCKS_PER_SEC;
    }
}
