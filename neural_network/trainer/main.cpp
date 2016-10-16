
#include <iostream>

#include "network_trainer.h"

// 0:trainer
// 1:gridspace
// 2:rover

int run_type = 0;

int main()
{
	if (run_type == 0)
	{
		Trainer::Trainer t;
		t.train();
	}
	/*
	else if (run_type == 1)
	{
		Gridspace::Grid g;
		// dimentions (x, y)
		// player (human:true, bot:false)
		g.play_game(10, 10, true);
	}
	else if (run_type == 2)
	{
		Rover::Rover r;
		r.rw.create_world(20, 20);
		r.add_rover(2, 1, 45);
	}
	*/
	return 0;
}
