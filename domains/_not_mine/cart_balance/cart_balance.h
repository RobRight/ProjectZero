// Cart_Balance-Code.cpp : Defines the entry point for the console application.
// Min-Max Evolutionary Controllers Physics Portion

#ifndef _CART_BALANCE_CODE_
#define _CART_BALANCE_CODE_

#define CB_CONSULE
#define CB_FILE

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
	};

	Pendulum::Pendulum()
	{
		Pend_state initial;
		// able to change //
		mass_p = 50;
		length = 10;
		//output_filename = "Pendulumdata.csv";
		////////////////////
		cycle_count = 1;
		initial.theta = 45 * M_PI / 180;
		initial.Px = length*cos(initial.theta);
		initial.Py = length*cos(initial.theta);
		// initialize theta_dot=0 and theta double dot= little less that 90 degrees
		initial.theta_dot = 0; // rad/s // theta dot of this specific pendulum
		initial.theta_dd = 0;

		pend.push_back(initial); //push_back pushes it to the back of the vector
		
		std::ofstream fout;
		fout.open("pendulumdata.csv", std::ofstream::out | std::ofstream::trunc);
		fout.close();
	}



	void Pendulum::cycle() {

		// variables
		Pend_state nextState;
		//note

		// does all necessary calculations, given an action (already set from set_action), to arrive at the next state at the next timestep.
		//torque to theta dd
		nextState.theta_dd = -g*cos(pend.at(pend.size() - 1).theta) / (mass_p*length) + torq; //rad/s^2   // define theta_dd with t variable 
		//thetat_dd to theta_dot
		nextState.theta_dot = pend.at(pend.size() - 1).theta_dot + nextState.theta_dd*dt;
		//theta_dot to theta
		nextState.theta = pend.at(pend.size() - 1).theta + nextState.theta_dot*dt;
		//theta to xy
		nextState.Px = length*cos(nextState.theta);
		nextState.Py = length*sin(nextState.theta);
#ifdef CB_CONSULE
		std::cout << nextState.theta << "," << nextState.theta_dot << "," \
		<< nextState.theta_dd << "," << nextState.Px << "," << nextState.Py \
		<< std::endl;
#endif		

		pend.push_back(nextState); //update state

		fitness = determine_reward();
#ifdef CB_FILE
		std::ofstream fout;
		fout.open("pendulumdata.csv", std::ofstream::out | std::ofstream::app);
		//calculate xy
		//use theta
		//output xy
		fout << pend.at(pend.size()-1).Px << "," << nextState.Py \
			<< "," << dt*cycle_count << "," << nextState.theta << "," \
			<< nextState.theta_dot << "," \
			<< nextState.theta_dd << "\n";
	 
		fout.close();
#endif 
		++cycle_count;
	}

	double Pendulum::determine_reward() {
		double total_fitness;
		double fitness_1 = abs(M_PI/2 - pend.at(pend.size()-1).theta);
		double fitness_2 = abs(0 - pend.at(pend.size()-1).theta_dot);
		
		total_fitness = fitness_1 + fitness_2;
		
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
}

#endif // !