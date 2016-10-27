/*

Agent Abstract Class
Copyright (c) 2016 William Gregory.  All rights reserved.

Domains recieve an action and give a state.

Node: 
 - how and when is reward involved?
 - how to incorperate setup functions? 

*/

#include <vector>

class Domain {
    std::vector <double> recieved_action;
public:
    virtual std::vector <double> give_state(void) = 0;
    virtual std::vector <double> give_reward(void) = 0; // ?
    virtual void get_action(std::vector <double> in) {
        recieved_action = in;
    }
};

/*

gridspace cycle becomes:

- give_state(); get_action();

*/
