
/*

Agent Abstract Class
Copyright (c) 2016 William Gregory.  All rights reserved.

Agents give an action and recieve a state and an optional reward.

Node:
    - how to incorperate setup functions? 
    - how and when is reward involved?
    - how to organize function calls so give_action passes value to get_action in the domain
*/

#include <vector>

class Agent {
protected:
    std::vector <double> recieved_state;  // store states here. retreive in agent from here.
    std::vector <double> recieved_reward;
public:
    virtual std::vector <double> give_action(void) = 0;
    void get_state(std::vector <double> in) {
        recieved_state = in;
    }
    void get_reward(std::vector <double> in) {
        recieved_reward = in;
    }
};


/*

network cycle becomes:

- setup(); (not here)

- BEFORE output cycle(input); NOW get_state(); give_action();

*/
