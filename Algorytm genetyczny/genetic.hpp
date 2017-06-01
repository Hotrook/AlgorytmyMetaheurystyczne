#ifndef FROST_GENETIC
#define FROST_GENETIC

#include "functions.hpp"
#include "controller.hpp"
#include <algorithm>


class Genetic{ 

public:

	Genetic( int n, Controller * c, VPDD & input );
	double calculate( VI& bestRoute, VPDD& input );

private:

	Controller * c;
	VVI currentPaths;
	VVI reproduction;
	VPDD input;
	VD currentLengths;

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
	void printWithPivots( VI& o, int first, int second );
	void hardFirstGeneration();
	void hardMutate(VI & tmp );
	void cx(VI & o1, int first, int second);
	void up(VI & o );
};

#endif