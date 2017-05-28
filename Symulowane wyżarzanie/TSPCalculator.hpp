#ifndef FROST_TSP_CALCULATOR
#define FROST_TSP_CALCULATOR

#include "functions.hpp"
#include <mutex>
#include <thread>


struct Controller{
	mutex mtx;
	bool cond;

	Controller(){
		cond = true;
	}
};


class TSPCalculator{

public:

	TSPCalculator( int n, VI & route, VPDD& input, Controller * c ); 
	double calculateTSP( VVD & dist, VI & bestRoute, VPDD& input );

private:

	int INIT;
	int startTemp;
	int n;
	int tempFallsCounter;
	int repetitionCounter;
	int radicalTempIncCounter; // Radical Temperature Increase Counter
	int iterWithoutTempChange;
	int regularIncrease;
	int rareIncrease;

	Controller * c;

	VI route;
	
	void controlTemperature();
	double bestRouteEverLen; // Result;
	double curRouteLen; // Current_Route_Length
	double previousLength;// Current_Route_Length
	double afterSwapLen;
	double T; // Temperature
	long long int iterationsCounter;
	
	bool haveTime();

};



void myTimer( int i, Controller * c );



#endif