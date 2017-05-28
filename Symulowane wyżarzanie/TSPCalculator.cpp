#include "TSPCalculator.hpp"

TSPCalculator::TSPCalculator( int n, VI & route, VPDD& input, Controller * c){
	this->n = n;
	this->route = route;
	if( n <= 50){
		INIT = 3;
		regularIncrease = 1;
		rareIncrease = 1;
	}
	else if( n <= 1000 ){
		double rate = double( n )/double(1000);
		INIT = 70 * rate ;
		regularIncrease = 17 * rate ;
		rareIncrease = 5 * rate ;
	}
	else{
		double rate = double( n - 1000 )/double(14000);
		INIT = 60 + 940 * rate ;
		regularIncrease = 15 + 235 * rate;
		rareIncrease = 5 + 80 * rate;
	}
	startTemp = INIT;
	tempFallsCounter = 0;
	repetitionCounter = 0;
	radicalTempIncCounter = 0;
	bestRouteEverLen = calculateRoute(n, route, input ); 
	curRouteLen = bestRouteEverLen; 
	previousLength = bestRouteEverLen; 
	T = n/startTemp; // Temperature
	iterationsCounter = 0;
	iterWithoutTempChange = 100000;

	this->c = c;
}




double TSPCalculator::calculateTSP( VVD & dist, VI & bestRoute, VPDD& input ){

	cout << "start" << endl;
	while( haveTime() ){

		PII toSwap = getTwoRandomNumbers(n);
		afterSwapLen = checkSwap( 
			n, 
			curRouteLen, 
			route, 
			toSwap.first, 
			toSwap.second, 
			input 
		);

		if( afterSwapLen < curRouteLen ){

			curRouteLen = afterSwapLen;
			swap( route[ toSwap.first ], route[ toSwap.second ] );

			if( curRouteLen < bestRouteEverLen ){

				bestRouteEverLen = curRouteLen;
				bestRoute = route;
			}

		}
		else if( getRandomNumber() < probability(afterSwapLen, curRouteLen, T)){
			
			curRouteLen = afterSwapLen;
			swap( route[ toSwap.first ], route[ toSwap.second ] );
	
		}
		
		controlTemperature();
	}

	return bestRouteEverLen;
}





void TSPCalculator::controlTemperature(){

	iterationsCounter++;
	if( iterationsCounter % iterWithoutTempChange == 0 ){
		
		if( curRouteLen == previousLength ){
			repetitionCounter++;
		}
		else{
			repetitionCounter = 0;
			previousLength = curRouteLen;
		}
		T  = n / (startTemp + iterationsCounter / iterWithoutTempChange);
	}

	if( repetitionCounter == 5 ){

		repetitionCounter = 0;
		startTemp += regularIncrease;
		tempFallsCounter++;
		iterationsCounter = 0;

	}

	if( tempFallsCounter == 10 ){ 

		tempFallsCounter = 0;
		radicalTempIncCounter++;
		startTemp -= regularIncrease * 9 ;

	}

	if( radicalTempIncCounter == 5){

		radicalTempIncCounter = 0;
		INIT += rareIncrease;
		startTemp = INIT;
		iterationsCounter = 0;

	}
}





bool TSPCalculator::haveTime(){
	this->c->mtx.lock();
	bool result = this->c->cond;//tochange
	this->c->mtx.unlock();
	return result;
}






void myTimer( int i, Controller * c ){
	this_thread::sleep_for( chrono::milliseconds(max(0,i-2) * 1000 + 800 ));
	c->mtx.lock();
	c->cond = false;
	c->mtx.unlock();
}
