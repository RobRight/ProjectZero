/*

Agent Abstract Class
Copyright (c) 2016 William Gregory.  All rights reserved.

Domains recieve an action and give a state.  (reward?)

*/

#include <double>

class Domain {
private:
    std::vector <double> state;
    //std::vector <double reward;
public:
    void get_action(std::vector <double> in) {

    }
    std::vector <double> give_state(void) {
        return state;
    }
    //std::vector <double> give_reward(void) {
    //    return reward;
    //}
};