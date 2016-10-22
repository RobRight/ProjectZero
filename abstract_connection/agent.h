
/*

Agent Abstract Class
Copyright (c) 2016 William Gregory.  All rights reserved.

Agents give an action and recieve a state.

Node: how and when is reward involved?

*/

#include <vector>

class Agent {
protected:
    std::vector <double> state;
    std::vector <double> reward;
public:
    virtual std::vector <double> give_action(void) = 0;
    void get_state(std::vector <double> in) {
        state = in;
    }
    void get_reward(std::vector <double> in) {
        reward = in;
    }
};
