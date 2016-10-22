
// Rover

#include "rover.h"

// - settings -
// world entities
unsigned int pois = 1;      // POIs to generate at start
unsigned int rovers = 1;    // rovers to generate at start
// world size (min default to zero)
unsigned int world_x = 6;   // X max
unsigned int world_y = 6;   // Y max
// - settings end - 

std::vector <double> states;
std::vector <unsigned int> moves;

int main()
{
    RD::RoverDomain rd(world_x, 0, world_y, 0);
    rd.setup(pois, rovers);

    /*
    states = rd.return_states();
    std::cout << "states:" std::endl;
    for (std::size_t i=0; i<states.size(); ++i)
        std::cout << "\t" << states.at(i) << std::endl;
    std::cout << std::endl;

    if(rd.set_moves(moves)) std::cout << "moves set" << std::endl;
    */

    return 0;
}