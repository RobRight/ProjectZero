
// test nn2

#include "../neural_network.h"

// - settings -
bool debug = false;
bool verbose = true;

std::vector <unsigned int> nc = {1,2,1};  // node count per layer
double mutate_mod = 0.1;
double mutate_chance = 0.5;
// - settings end -

std::vector <double> input;
std::vector <double> output;

void display_output(std::vector <double> in)
{
	std::cout << "\nOutputs:" << std::endl;
	for (std::size_t i=0; i<in.size(); ++i)
	{
		std::cout << "\t" << output.at(i) << std::endl;
	}
	std::cout << std::endl;
}

void test_network()
{
	// setup
	Network::Network n;
	n.verbose = verbose;
	n.debug = debug;
	n.setup(nc, mutate_mod, mutate_chance);

	// test
	input.clear();
	input.push_back(0);
	output = n.cycle(input);
	display_output(output);
	//n.mutate();
	input.clear();
	input.push_back(0.2);	
	output = n.cycle(input);
	display_output(output);
	n.mutate();
	output = n.cycle(input);
	display_output(output);
}

int main()
{
	test_network();
	return 0;
}
