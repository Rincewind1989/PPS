#include <SFML/Graphics.hpp>
#include "World.h"

//A simulator based on "https://www.ncbi.nlm.nih.gov/pmc/articles/PMC5346932/" with an emergence behavior effect.
//Can be run on linux and windows with the C++ library SFML.
//The main function takes as first argument the number of particles,
//As second the x-dimension and as third the y-dimension of the world size

int main(int argc, char** argv)
{	
	World world(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atof(argv[4]));
	world.startWorld();
	
	
    return 0;
}
