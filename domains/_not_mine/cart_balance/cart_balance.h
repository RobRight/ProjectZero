// Cart_Balance-Code.cpp : Defines the entry point for the console application.
// Min-Max Evolutionary Controllers Physics Portion

#ifndef _CART_BALANCE_CODE_
#define _CART_BALANCE_CODE_

//#define CB_CONSULE
//#define CB_FILE

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <iostream>
//#include <string>

// Establish boundaries
// Table boundaries
// gravity = 9.81; // m/s



////Struct has function 
//Class has public, private, function and inheritance

namespace CB {

	// Initialize knowns
	const float g = 9.81f; // m/s^2
	static const double dt = 0.1; //time step
	// Cart
	// Establish cart boundaries
	// cart movements - left and right
	// Cart starting position
	// cart position - x
	// cart velocity - xdot 
	// forces on the cart
	// weight of cart
	struct Cart {

		const int mass_c; // Mass of the Cart
		const int y; // Y position of Cart - Irrelevant due to gravity
		const int y_dot; // Velocity of y component of Cart - Irrelevant due to gravity
		const int y_dd; // Y double dot - Y component of acceleration of Cart

				// Changing Variables
		double x; // X Position of Cart
		double x_dot; // Velocity of x component of Cart
		double x_dd; // Acceleration of x component of Cart
		// Cart cart;
		// cart.mass_c = 100;
	};

	///////////////////////////////////
	// Establish Pendulum boundaries //
	///////////////////////////////////
	struct Pend_state {
		// ball position - theta
		double theta; // ever changing theta - main objective to keep theta around 90*
		// ball position
		double Px; // x coordinate of Pendulum;
		double Py; // y coordinate of Pendulum;{
			
		double theta_dot; // ever changing velocity of theta.
		double theta_dd; // acceleration of theta.
	};


	class Pendulum {
	private:
		std::vector <double> torq_history;
		// static variables
		double mass_p; // Mass of Pendulum
		double length; // Length of the Pendulum
		
		// collection of states
		std::vector <Pend_state> pend;

		// Changing variables
		double torq;
		double I;
		double fitness; //last reward for the last action

		// public functions
		double determine_reward();
	public:
		Pendulum();
		//std::string output_filename;
		std::vector <double> give_reward();
		std::vector <double> give_state();
		void get_action(std::vector <double>);
		void cycle();
		unsigned int cycle_count;
		void export_all_states();
	};

	Pendulum::Pendulum()
	{
		Pend_state initial;
		// able to change //
		mass_p = 5; // kg
		length = 1; // m
		//output_filename = "Pendulumdata.csv";
		////////////////////
		cycle_count = 1;
		initial.theta = 89 * M_PI / 180;
		initial.Px = length*cos(initial.theta);
		initial.Py = length*cos(initial.theta);
		// initialize theta_dot=0 and theta double dot= little less that 90 degrees
		initial.theta_dot = 0; // rad/s // theta dot of this specific pendulum
		initial.theta_dd = 0;
		torq = 0; // N*m

		pend.push_back(initial); //push_back pushes it to the back of the vector
	
		torq_history.push_back(torq);

}

	void Pendulum::cycle() {

		// variables
		Pend_state nextState;
		//note

		// does all necessary calculations, given an action (already set from set_action),
		// to arrive at the next state at the next timestep.
		//torque to theta dd
		nextState.theta_dd = -g*cos(pend.at(pend.size() - 1).theta) / (mass_p*length) + torq;
		//rad/s^2   // define theta_dd with t variable 
		//thetat_dd to theta_dot
		nextState.theta_dot = pend.at(pend.size() - 1).theta_dot + nextState.theta_dd*dt;
		//theta_dot to theta
		nextState.theta = pend.at(pend.size() - 1).theta + nextState.theta_dot*dt;
		// keep theta between 0 and 2*PI
		if (nextState.theta < 0.0) nextState.theta = nextState.theta + 2*M_PI;
		//theta to xy
		nextState.Px = length*cos(nextState.theta);
		nextState.Py = length*sin(nextState.theta);

#ifdef CB_CONSULE
		std::cout << nextState.theta << "," << nextState.theta_dot << "," \
		<< nextState.theta_dd << "," << nextState.Px << "," << nextState.Py \
		<< std::endl;
#endif

		pend.push_back(nextState); //update state

		torq_history.push_back(torq);
		
		fitness = determine_reward();
		
		++cycle_count;
	}

	double Pendulum::determine_reward() {
		double total_fitness;
		double fitness_1 = abs((M_PI/2 - pend.at(pend.size()-1).theta)*1000); // 90* - theta // max: 6
		double fitness_2 = abs((pend.at(pend.size()-1).theta_dot)*10);  // theta_dot // max:10 ( inf )
		
		double fitness_ch = 0.0;  // cross horizontal for better positon 
		if (pend.at(pend.size()-1).theta > M_PI)
			fitness_ch = 1000;
				// penalty for crossing horizontal axis

		total_fitness = abs(fitness_1) + abs(fitness_2) + abs(fitness_ch);
		
		return total_fitness;
	}

	void Pendulum::get_action(std::vector <double> in_action) { //receives "action vection", which in the first case will just consist of the torque at the joint

		//name vector
		// torq=t@0
		torq = in_action.at(0);
		cycle();
	}

	std::vector <double> Pendulum::give_state() {
		//gives the state of the pendulum at the given timestep
		std::vector <double> temp_state;
		temp_state.push_back(pend.at(pend.size()-1).theta);
		temp_state.push_back(pend.at(pend.size()-1).theta_dot);
		return temp_state;
	}
	std::vector <double> Pendulum::give_reward() {
		std::vector <double> total_fitness;
		total_fitness.push_back(fitness);

		return total_fitness;
	}
	void Pendulum::export_all_states() {
		std::ofstream fout;
		fout.open("pend_state_log.csv", std::ofstream::out | std::ofstream::trunc);
		fout << "torq, x, y, theta, theta_dot, theta_dd" << "\n";
		for (std::size_t i=0; i<pend.size(); ++i) {
			fout << torq_history.at(i) << ", " << pend.at(i).Px << ", " << pend.at(i).Py << ", " << \
				pend.at(i).theta*180/M_PI << ", " << pend.at(i).theta_dot << ", " << \
				pend.at(i).theta_dd << "\n";
		}	
		fout.close();
	}	
}

#endif // !
