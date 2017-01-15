/*

System 1.1
agent <> domain

William Gregory
copyright 2017 (c) all rights reserved

date: 12/20/2016 to Present

---------------------------------

about:
- multi agent; single domain

setup:
- single agent; single objective

notes:
- DOMAIN : domain specific
- FIX : needs work
- ERROR : current error point

status:
- adding steps in System::run() to find bugs
- nural network halting at weight generation

---------------------------------

todo:
- add data output
- add round to system?
- check update in/out for trainer and cart_balance
- add needed local values (consider GUI) updated with 'agent_update()'
- add test best network function to trainer
- add trainer domain.exort_all_states() control
- efnorce namespace and class standards to prevent code modification need
- posibly create connection class in agent and domain namespace
- how to handle which network is being tested in agent
- - instead of local loop, checkin and increment with report
- - add new domain option from agent
- - rename agent and domain object_... and move to seperate files
- problem with neural network ***
- - compare with past version
- maybe add folders to source

ideas:
- system has two modes (single and train (multiple)) replace run code
- add class for iostream use (error, debug, ...)

//double progress = (double)in_cr/in_rm;

*/

#ifndef _SYSTEM_
#define _SYSTEM_

#include "utilities.hpp"

#include "network_trainer.hpp"	// AGENT
#include "pendulum.hpp"	 // DOMAIN

namespace System {

	/*

		agent class - "port" between agent and system

	*/
	class Agent {
		private:
		Trainer::Trainer agent;	 // AGENT
		double best_fitness;
		Network::Network best_network;
		//
		public:
		Agent() {
			agent.setup();
	}
		// send state to agent
		void state(std::vector <double> in) {
			agent.get_state(in);
		}
		// get action from agent
		std::vector <double> action() {
			std::vector <double> out;
			out = agent.give_action();
			return out;
		}
		// send fitness to agent
		void fitness(std::vector <double> in) {
			agent.get_fitness(in);
		}
		// data from system; update agent
		void update_in(std::vector <double> in) {
			agent.update_in(in);
		}
		// data from agent; update system
		std::vector <double>  update_out() {
			std::vector <double> u;
			u = agent.update_out();
			return u;
		}
	};

	/*

		domain class - "port" between domain and system

	*/
	class Domain {
		private:
		InvPend::Pendulum domain; // DOMAIN

		public:
		Domain() {
		unsigned int test_count = 100;
			domain.setup(test_count, 89.0);	 // FIX - need test count from agent
		}
		// return domain state
		std::vector <double> state() {
			std::vector <double> out;
			out = domain.give_state();
			return out;
		}
		// set domain action
		void action(std::vector <double> in) {
			domain.get_action(in);
		}
		// return domain fitness // just double?
		std::vector <double> fitness() {
			std::vector <double> out;
			out = domain.give_fitness();
			return out;
		}
		// pass other information
		void update_in(std::vector <double> in) {
			domain.update_in(in);
		}
		std::vector <double> update_out() {
			std::vector <double> u;
			u = domain.update_out();
			return u;
		}
	};

	/*
		State: system state container
	*/
	struct State {
		std::vector <double> state;
		std::vector <double> action_sum;
		std::vector <std::vector <double> > action_all;
		std::vector <double> fitness;
		//
		std::vector <double> domain_update;	 // from domain
		std::vector <std::vector <double> > agent_updates; // from agent
		unsigned int round_current;
	};

	/*

		System

	*/
	class System {
		private:
		bool system_active;
		unsigned int run_type;
		//
		double best_fitness;
		Agent best_agent;
		std::vector <double> agent_weights;	 // action weight
		//
		std::vector <Agent> agents;
		Domain domain;
		//
		std::vector <State> state_all;
		State state_last;
		//
		unsigned int agent_count;
		//
		unsigned int round_current;
		unsigned int round_max;
		bool round_last;
		//
		bool domain_fail;

		public:
		System() {
			round_current = 1;
			domain_fail = false;
			round_last = false;
		}

		void setup(unsigned int in_rm, unsigned int in_ac, unsigned int in_rt) {
			round_max = in_rm;
			agent_count = in_ac;
			run_type = in_rt;
			// create agent(s)
			for (std::size_t i=0; i<agent_count; ++i) {
				Agent a;
				agents.push_back(a);
				agent_weights.push_back(1.0); // FIX
			}
			//
			system_active = true;
		}

		void reset_domain() {
			Domain d;
			domain = d;
		}

		//-----------------------------------

		std::vector <double> create_domain_update() {
			std::vector <double> t;
			if (round_last) t.push_back(1.0);
			else t.push_back(0.0);
			return t;
		}

		std::vector <double> create_agent_update(State in) {
			std::vector <double> t;
			if (in.domain_update.at(0) == 1.0) t.push_back(1.0);
			else t.push_back(0.0);
			return t;
		}

		void check_domain_update(std::vector <double> in) {
			// domain fail
			if (in.at(0) == 1.0) domain_fail = true;
		}

		void check_agent_update(std::vector <double> in) {
			// index: 0 reset domain
			if (in.at(0) == 1.0) reset_domain();
		}

		void run_single() {

		}

		void run_trainer() {
			// MAIN LOOP
			
			//while (system_active) {
				State s;
		s.round_current = round_current;
				state_last = s;
				// get state
				state_last.state = domain.state();
				// manage agents
		for (std::size_t i=0; i<agents.size(); ++i) {
					// give state
					agents.at(i).state(state_last.state);  // ERROR
					// get action
					//state_last.action_all.push_back(agents.at(i).action());
				}
		/*
				// compile actions
				for (std::size_t i=0; i<state_last.action_all.size(); ++i) {
					for (std::size_t j=0; j<state_last.action_all.at(i).size(); ++j) {
						if (i == 0) state_last.action_sum.push_back(0.0);
						// sum actions from each agent with agent action weight
						state_last.action_sum.at(j) += state_last.action_all.at(i).at(j) * agent_weights.at(i);
					}
				}
				// domain action
				domain.action(state_last.action_sum);
				// domain fitness
				state_last.fitness = domain.fitness();
				//domain update in
				domain.update_in(create_domain_update());
				// domain update out
				state_last.domain_update = domain.update_out();
				// manage agents
				for (std::size_t i=0; i<agents.size(); ++i) {
					// fitness
					agents.at(i).fitness(state_last.fitness);
					// update in
					agents.at(i).update_in(create_agent_update(state_last));
					// update out
					state_last.agent_updates.push_back(agents.at(i).update_out());
				}
				state_all.push_back(state_last);

				++round_current;
				if (round_current == round_max) {
					system_active = false;
				} else if (round_current-1 == round_max) {
					round_last = true;
				}
			*/
			//}
		}

		void run() {
			clock_t time_start = clock();

			if (run_type == 1) run_single();
			else if (run_type == 2) run_trainer();
			else {
				std::cout << "unknown run_type" << std::endl;
			}
			std::cout << "system exiting" << std::endl;
		}
	};
}

#endif
