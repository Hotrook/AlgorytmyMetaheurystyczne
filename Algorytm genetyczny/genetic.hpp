#ifndef FROST_GENETIC
#define FROST_GENETIC

#include "functions.hpp"
#include "controller.hpp"



class Genetic{ 

public:

	Genetic();
	double calculate( VI& bestRoute, VPDD& input );

private:

	Controller * c;
	bool haveTime();

};

#endif