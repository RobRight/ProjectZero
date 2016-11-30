
// Run Network Trainer for Neural Network : Domain Gridspace

#include <iostream>

#include "network_trainer.h"

//#define NT_DEBUG
#define NT_VERBOSE
//#define NT_TEST

int main()
{
	Trainer::Trainer t;

	// - settings start -
	//t.population_size = 100;
	//t.round_max = 10000;
	//t.hidden_layer_size = 4;
	//t.mutate_mod = 0.1;
	//t.mutate_chance = 0.3;  // X% chance for weight to mutate
	// - settings end -

	t.train();

	return 0;
}
