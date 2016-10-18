
// Run Network Trainer for Neural Network

#include <iostream>

#include "network_trainer.h"

int main()
{
	Trainer::Trainer t;

	// - settings start -
	t.population_size = 100;
	t.round_max = 1000;
	t.sub_round_max = 60;
	t.input_min = 1;
	t.input_max = 5;
	t.hidden_layer_size = 4;
	t.mutate_mod = 0.1;
	t.mutate_chance = 0.5;
	// - settings end -

	//debug = false;
	//verbose = false;

	t.train();

	return 0;
}
