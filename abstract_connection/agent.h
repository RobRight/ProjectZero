
/*

Agent Abstract Class
Copyright (c) 2016 William Gregory.  All rights reserved.

Agents give an action and recieve a state.  (reward?)

*/

#include <vector>

class Agent {
private:
    std::vector <double> action;
public:
    std::vector <double> give_action(void) {
        return action;
    }
    void get_state(std::vector <double> in) {

    }
    //void get_reward(std::vector <double> in) {
    //
    //}
};

