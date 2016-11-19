
// multi agent
// single domain; multi objective

#include "../agents/neural_network/trainer/network_trainer.h"  // domain
//#include "../agents/neural_network/network/"  // domain
#include ""  // agent

class Connection {
    Trainer::Trainer agent;  // AGENT_CLASS agent;
    Cart domain;  // DOMAIN_CLASS domain;
    Connection() {
        domain_setup();
        agent_setup();
    }
    // - domain -
    void domain_setup() {
        // params for domain
        //domain.setup();
    }
    std::vector <double> domain_state() {
        std::vector <double> out;
        out = domain.give_state();
        return out;
    }
    void domain_action(std::vector <double> in) {
        domain.get_action(in);
    }
    double domain_reward() {
        double out;
        out = domain.give_reward();
        return out;
    }
    // - agent -
    void agent_setup() {
        // params for agent
        //agent.population_size = 100;
	    //agent.round_max = 1000;
	    //agent.sub_round_max = 60;
	    //agent.hidden_layer_size = 4;
	    //agent.mutate_mod = 0.1;
	    //agent.mutate_chance = 0.3;
        agent.train();
    }
    void agent_state(std::vector <double> in) {
        agent.get_state(in);
    }
    std::vector <double> agent_action() {
        std::vector <double> out;
        out = agent.give_action();
        return out;
    }
    void agent_reward(double in) {
        agent.get_reward(in);
    }
    // run
    void run() {
        std::vector <double> state;
        std::vector <double> action;
        std::vector <double> fitness;
        while (true) {
            agent_state(domain_state());
            domain_action(agent_action());
            agent_reward(domain_reward());
            
        }
    }
}