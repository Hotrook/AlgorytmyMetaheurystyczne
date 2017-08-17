#ifndef FROST_GENETIC
#define FROST_GENETIC


#include <algorithm>

#include "SynchronizedRoute.hpp"
#include "functions.hpp"
#include "controller.hpp"


class Genetic{ 

public:

	Genetic( int n, Controller * c, std::vector<Point> & input, SynchronizedRoute * sroute );

	double calculate( VI& bestRoute, std::vector<Point>& input );

private:

	Controller * c;
	VVI currentPaths;
	VVI reproduction;
	std::vector<Point> input;
	VD currentLengths;

	SynchronizedRoute * sroute;

	bool finishTime;

	int n;
	int parentNumber;
	int childNumber;
	int noImproveCounter;
	int controlTime;

	bool haveTime();
	void generateFirstGeneration();
	void createNextGeneration();
	void selection();
	void init_and_permute( VI& path );
	void combine( int first, int second );
	void pmx( VI & o1, VI & o2, int first, int second );
	void ox( VI & o1, VI & o2, int first, int second );
	void determineValue( VI & o, int pathIndex, int index, VI & conflicts );
	void addNewPermutations();
	void mutate( VI & o );
	void mutateAll();
	void hardFirstGeneration();
	void hardMutate(VI & tmp );
	void cx(VI & o1, int first, int second);
	void up(VI & o );
};

#endif