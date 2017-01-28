
/*

Test Code

Copyright 2016 William Gregory.	 All rights reserved.

*/

#include <iostream>

#include "../cart_balance.hpp"
#include "../neural_network.hpp"

#define PI 3.14159

void cart_balance_delta_theta() {
	std::cout << "cart_balance_delta_theta" << std::endl;

	// add test P/F
	// FIX : last two failing

	// test 1, 359, 91, 89
	CB::Pendulum pend;
	std::cout << "0:90" << "\t" << pend.delta_theta(0*PI/180)*180/PI << std::endl;	// 90
	std::cout << "90:0" << "\t" << pend.delta_theta(90*PI/180)*180/PI << std::endl;	 // 0
	std::cout << "180:90" << "\t" << pend.delta_theta(180*PI/180)*180/PI << std::endl;	// 90
	std::cout << "270:180" << "\t" << pend.delta_theta(270*PI/180)*180/PI << std::endl;	 // 180
	std::cout << "360:90" << "\t" << pend.delta_theta(360*PI/180)*180/PI << std::endl;	// 90
	//
	std::cout << "cart_balance_delta_theta - done" << std::endl;
}

// agent
void neural_network() {
	std::cout << "neural_network" << std::endl;
	std::vector <unsigned int> nc = {2,4,2};
	double mutate_mod = 1.0;
	double mutate_chance = 0.5;
	//
	Network::Network n;
	n.setup(nc, mutate_mod, mutate_chance);
	std::cout << "neural_network - done" << std::endl;
}

int main () {

	std::cout << "\nTesting:" << std::endl;
	std::cout << "------------" << std::endl;

	cart_balance_delta_theta();
	neural_network();

	std::cout << "-------------------" << std::endl;
	std::cout << "training complete" << std::endl;
	std::cout << "" << std::endl;

	return 0;
}
