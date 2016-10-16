
// test nn2

#include "../neural_network.h"

std::vector <unsigned int> nc = {1,2,1};  // node count per layer
double mutate_mod = 0.1;
double mutate_chance = 0.5;

std::vector <double> input = {0.6};
std::vector <double> output;

void display_output(std::vector <double> in)
{
	std::cout << "Outputs:" << std::endl;
	for (std::size_t i=0; i<in.size(); ++i)
	{
		std::cout << "\t" << output.at(i) << std::endl;
	}
	std::cout << std::endl;
}

int main()
{
	Network::Network n;
	n.debug = false;

	n.setup(nc, mutate_mod, mutate_chance);
	//for (std::size_t i=0; i<10; ++i)
	//{
	output = n.cycle(input);
	display_output(output);
	input.at(0) = 0.1;	
	output = n.cycle(input);
	display_output(output);
	//	n.mutate();
	//}
	
	return 0;
}
