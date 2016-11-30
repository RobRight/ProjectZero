
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
#include "../../../domains/_not_mine/cart_balance/cart_balance.h"  // domain

namespace Trainer
{
	class Trainer
	{
	private:
        std::vector <Network::Network> population;
		std::vector <double> pop_fitness;
        std::vector <double> best_fitness;
        std::vector <Network::Network> best_network;
        unsigned int current_round;
		unsigned int ID_next;
		unsigned int network_test_count;  // current
		CB::Pendulum domain;  // domain // DOMAIN SPECIFIC

		std::vector <double> last_state;
		std::vector <double> last_action;
		double last_fitness;

        void print_intro();
        void print_end();

        void get_state(std::vector <double>&);
        std::vector <double> give_action();
        void get_reward(double&);

		CB::Pendulum generate_domain();  // DOMAIN SPECIFIC
		Network::Network generate_network();
		void generate_population();
		std::vector <double> cycle_network(std::vector <double>&, unsigned int&);
		void log_reward(double&, unsigned int&);
		void cycle();
		void error_manager(std::vector <double>&);

        std::vector <Network::Network> prune(std::vector <Network::Network>&, std::vector <double>&);
        std::vector <Network::Network> populate(std::vector <Network::Network>&, unsigned int&);
    public:
        unsigned int round_max;
        unsigned int population_size;
        std::vector <unsigned int> nodes_per_layer;
		double mutate_mod;
		double mutate_chance;
		unsigned int test_count;  // domain cycles

        Trainer();
        void train();
    };

    Trainer::Trainer() {
        // do not modify
        runtime_error = false;
        current_round = 0;
		ID_next = 1;
		// settings
		test_count = 100;
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
    void Trainer::get_state() {
		last_state = domain.give_state();
		//last_state = in;
    }

    // give action to domain
    void Trainer::give_action() {
		domain.get_action(last_action);
		//return last_action;
    }

    void Trainer::get_reward() {
		last_fitness = domain.give_reward();
		//last_fitness = in_val;
    }
    //-----------------------------

	// generate single domain and return
	CB::Pendulum Trainer::generate_domain()
	{
		CB::Pendulum pend;
		return pend;
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
	// generate_population: sub
	// generate starting population
	// fill pop_fitness with zeros
	//
	void Trainer::generate_population()
	{
		population.clear();
		for (std::size_t i=0; i<population_size; ++i) {
			population.push_back(generate_network());
			pop_fitness.push_back(0.0);
		}
	}

	// cycle network with given inputs and return outputs
	std::vector <double> Trainer::cycle_network(std::vector <double>& in_val, unsigned int& in_count) {
		std::vector <double> t_out;
		t_out.at(in_count).cycle(in_val);
		return t_out;
	}

	// add last fitness to pop_fitness
	void Trainer::log_reward(double& in_val, unsigned int& in_count) {
		pop_fitness.at(in_count) += in_val;
	}

	// cycle agent - domain for 'test_count' iterations
	void Trainer::cycle() {
		for (std::size_t i = 0; i<test_count; ++i) {
			get_state();
			last_action = cycle_network(last_state, network_test_count);
			give_action();
			get_reward();
			log_reward(last_fitness, network_test_count);
		}
	}

	//
	void Trainer::error_manager(std::vector <double>& in_fitness) {
		// check for best fitness
		// average fitness
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
			for (std::size_t i=0; i<population.size(); ++i)
			{
				network_run_count = i;
				domain = generate_domain();
				cycle();
			}
			error_manager(pop_fitness);
			population = prune(population, pop_fitness);
            population = populate(population, population_size);
        }
        double delta_time = (clock() - time_start) / CLOCKS_PER_SEC;
    }
}