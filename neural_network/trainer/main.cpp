
// Run Network Trainer for Neural Network

#include <iostream>

#include "network_trainer.h"

int main()
{
	if (run_type == 0)
	{
		Trainer::Trainer t;
		t.train();
	}
	return 0;
}
