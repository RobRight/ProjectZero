/*

System 1.1
agent <> domain

William Gregory
copyright 2017 (c) all rights reserved

date: 12/20/2016 to Present

---------------------------------

info:
- Agent
- Domain
- SystemSingle
- SystemTrainer
- System

about:
- multi agent; single domain
- 'updates' allow communication between agent, domain, and system
- each agent is treated the same besides an optional 'action' weight

cycle:
- domain state > agent(s)
- compile agent(s) actions using weights
- action > domain
- domain fitness > agent(s)
- posible configuration change or end of program

setup:
- single agent; single objective

inline note format:
- DOMAIN : domain specific
- FIX : needs work
- ERROR : current error point

---------------------------------

status: keep updated log here
-- serate SystemSingle and SystemTrainer
-- SystemTrainer uses SystemSingle

- next step is merging the trainer into the system.
- - no seperate trainer class.  manage the population of agents from the system.
- - posible Evolutionary Algorithm seperate from system for easy modification.
- - consider alternitives to currention 'update_in/out' method.
- - testing seperate trainer; no mutation; only running population at first index.
- - note: single run agents are wrong.  both should be agents, no trainer.

---------------------------------

todo:
- add data output to system
- add round to system - consider trainer
- check update in/out for trainer and cart_balance
- add needed local values (consider GUI) updated with 'agent_update()'
- add test best network function to trainer
- add trainer domain.exort_all_states() control
- efnorce namespace and class standards to prevent code modification need
- how to handle which network is being tested in agent
- - instead of local loop, checkin and increment with report
- - add new domain option from agent
- update Makefile so only modified files are compiled

ideas:
- system has two modes (single and train (multiple)) alternate main run code
- - make network_trainer a general purpose 'agent trainer'
- - incorperate trainer into the system
- add class for iostream use (error, debug, ...)
- create individual test files for agent, domain and system
- - see _testing folder

---------------------------------

//double progress = (double)in_cr/in_rm;

*/

#ifndef _SYSTEM_
#define _SYSTEM_

#include "utilities.hpp"

#include "agent/network_trainer.hpp"	// AGENT
#include "domain/pendulum.hpp"	 // DOMAIN

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
		// data from system; update domain
		void update_in(std::vector <double> in) {
			domain.update_in(in);
		}
		// data from domain; update system
		std::vector <double> update_out() {
			std::vector <double> u;
			u = domain.update_out();
			return u;
		}
	};

	/*
		State: system state container
	*/
	struct StateSingle {
		std::vector <double> state;  // from domain
		std::vector <std::vector <double> > action_all;  // from agent
        std::vector <double> action_sum;
		std::vector <double> fitness_all;
        double fitness; // sum of fitness_all (cycle specific)
		//
		std::vector <double> domain_update;	 // from domain
		unsigned int cycle_current;
	};

    /*
     *  SystemSingle
     */
	class SystemSingle {
        std::vector <StateSingle> states;  // collection of states
        std::vector <Agent> agents;
        Domain domain;
        std::vector <double> agent_weights;
        unsigned int cycle_max;  // run count
        unsigned int cycle_current;
        double fitness; // sum of all states fitness - result
	public:
        void setup_single(std::vector<Agent> in_a, Domain in_d, unsigned int in_c, std::vector <double> in_w) {
#ifdef S_DEBUG
			std::cout << "S_DEBUG: setup single" << std::endl;
#endif
            agents = in_a;
            domain = in_d;
            cycle_max = in_c;
            agent_weights = in_w;
        }
        // return fitness
		double run_single() {
#ifdef S_DEBUG
			std::cout << "S_DEBUG: run single" << std::endl;
#endif
			while (cycle_current < cycle_max) {
                /*
				// new system state
				State s;
				state_last = s;
				state_last.round_current = round_current;
				// get domain state
				state_last.state = domain.state();
				// manage agents
				for (std::size_t i=0; i<agents.size(); ++i) {
					// give state
					agents.at(i).state(state_last.state);
					// get action
					state_last.action_all.push_back(agents.at(i).action());
				}
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
				// domain update in
				domain.update_in(create_domain_update());
				// domain update out
				state_last.domain_update = domain.update_out();
				// manage agents
				for (std::size_t i=0; i<agents.size(); ++i) {
					// update in (to agent)
					agents.at(i).update_in(create_agent_update(state_last));
					// update out (from agent)
					state_last.agent_updates.push_back(agents.at(i).update_out());
				}
				// round cleanup
				state_all.push_back(state_last);
				++round_current;
				if (round_current == round_max) {
					system_active = false;
				} else if (round_current-1 == round_max) {
					round_last = true;
				}
				*/
			}
			return fitness;
		}
	};

    /*
     *  SystemTrainer
     */
	class SystemTrainer {
    private:
        std::vector <std::vector <Agent> > population;   // groups of agents
        std::vector <double> fitness; // double per group
        //
        unsigned int pop_size;  // population size
        unsigned int agent_count;  // agents in a group
        unsigned int cycle_max;
        unsigned int cycle_current;
	public:
        void setup(unsigned int in_pop_size, unsigned int in_agent_count, unsigned int in_cycle) {
            // parameters
            pop_size = in_pop_size;
            agent_count = in_agent_count;
            cycle_max = in_cycle;
            cycle_current = 0;
            // create population
            population.clear();
            for (std::size_t p=0; p<pop_size; ++p) {
                std::vector <Agent> t;
                for (std::size_t i=0; i<agent_count; ++i) {
                    Agent a;
                    t.push_back(a);
                    //agent_weights.push_back(1.0); // FIX - pre-define
                }
                population.push_back(t);
            }
        }
		void run() {
#ifdef S_DEBUG
			std::cout << "S_DEBUG: run trainer" << std::endl;
#endif
		}
	};

	/*

		System

	*/
	class System {
    private:
		unsigned int type_run;  // run type (0:single, 1:trainer)
		unsigned int agent_count; // total number of agents
		//
		double best_fitness;
        std::vector <Agent> best_agents;
    public:
		// create agent(s) and domain
		// set inital system parameters
		void setup(unsigned int in_rm, unsigned int in_ac, unsigned int in_rt) {
#ifdef S_DEBUG
			std::cout << "S_DEBUG: system setup" << std::endl;
#endif
			round_max = in_rm;
			agent_count = in_ac;
			run_type = in_rt;
			
			// create domain
			Domain d;
			domain = d;
			// set system parameters
			system_active = true;
		}

		// replace domain with a fresh domain
		// note: old domain is lost
		void reset_domain() {
#ifdef S_DEBUG
			std::cout << "S_DEBUG: reset domain" << std::endl;
#endif
			Domain d;
			domain = d;
		}

		//-----------------------------------

		// update creation for domain:
		// - round_last
		std::vector <double> create_domain_update() {
#ifdef S_DEBUG
			std::cout << "S_DEBUG: create domain update" << std::endl;
#endif
			std::vector <double> t;
			if (round_last) t.push_back(1.0);
			else t.push_back(0.0);
			return t;
		}

		// update creation for agent:
		// - domain_update.at(0) // domain_fail
		std::vector <double> create_agent_update(State in) {
#ifdef S_DEBUG
			std::cout << "S_DEBUG: create agent update" << std::endl;
#endif
			std::vector <double> t;
			if (in.domain_update.at(0) == 1.0) t.push_back(1.0);
			else t.push_back(0.0);
			return t;
		}

		// domain update interpretation
		void check_domain_update(std::vector <double> in) {
#ifdef S_DEBUG
			std::cout << "S_DEBUG: check domain update" << std::endl;
#endif
			// domain fail
			if (in.at(0) == 1.0) domain_fail = true;
		}

		// agent update interpretation
		void check_agent_update(std::vector <double> in) {
#ifdef S_DEBUG
			std::cout << "S_DEBUG: check agent update" << std::endl;
#endif
			// index: 0 reset domain
			if (in.at(0) == 1.0) reset_domain();
		}

		//-----------------------------------


		void run() {
#ifdef S_DEBUG
			std::cout << "S_DEBUG: run" << std::endl;
#endif
			//clock_t time_start = clock();

			if (run_type == 1) run_single();
			else if (run_type == 2) run_trainer();
			else {
				std::cout << "unknown run_type" << std::endl;
			}

#ifdef S_DEBUG
			std::cout << "S_DEBUG: system complete" << std::endl;
#endif
		}
	};

}

#endif

