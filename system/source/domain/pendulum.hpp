// Cart Balance by Sierra
// inverted pendulum on a cart
// main objective to keep theta around 90*

// add random theta given range function

#ifndef _PENDULUM_HPP_
#define _PENDULUM_HPP_

// manage in system
//#define PD_DEBUG
//#define PD_CONSULE
//#define PD_MIDLOG

#ifndef gravity
#define gravity 9.81  //m/s^2
#endif

#ifndef PI
#define PI 3.14159265
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <string>

namespace InvPend {	 // Inverted Pendulum

	// pendulum state structure
	struct PendState {
		// with regards to the end of the pendulum
		double theta; // current theta - rad
		double theta_dot; // current velocity of theta - rad/s
		double theta_dd; // current acceleration of theta - rad/s^2
	};

	class Pendulum {
	private:
		/// variables
		// all actions for log
		std::vector <double> torq_history;
		// all fitness' for log
		std::vector <double> fitness_history;
		// all states.	last element is most current
		std::vector <PendState> pend;
		// applied torq - action
		double torq;
		// last determined fitness
		double fitness;
		// message before debug
		std::string pre_debug;
		/// functions
		// determine fitness
		double determine_reward();
	public:
		Pendulum();
		// fitness settings
		double tp_weight;  // theta
		double tv_weight;  // angular velocity
		double ch_weight;  // below horizontal axis
		// static settings
		double mass_p; // mass of pendulum
		double length; // length of the pendulum
		double theta_init; // inital theta
		double dt;	// time step
		/// functions
		// set prendulum properties
		void set_pendulum_pos(double, double);
		// set fitness weights
		void set_weights(std::vector <double>);
		// setup and set rounds
		void setup(unsigned int, double);
		// calculates next state given previous and an action
		void cycle();
		// offset from PI/2
		double delta_theta(double);
		// log all state history to file
		void export_all_states();
		//---------------------------------------
		// system functions
		// ----------------
		// return last state
		std::vector <double> give_state();
		// get action then cycle
		void get_action(std::vector <double>);
		// return last reward
		std::vector <double> give_fitness();
		// return update info
		std::vector <double> update_out();
		// get update info
		void update_in(std::vector <double>);
		// training
		bool below_horizontal;
	};

#endif
