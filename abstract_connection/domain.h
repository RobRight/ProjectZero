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
protected:
    std::vector <double> action;  // store action here orrrr in domain?
public:
    virtual std::vector <double> give_state() = 0;
    virtual std::vector <double> give_reward() = 0;
    void get_action(std::vector <double> in) {
        action = in;
    }
};

/*

gridspace cycle becomes:

- give_state(); get_action();

*/