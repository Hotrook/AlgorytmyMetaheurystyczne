#include <iostream>
#include <cstdio>
#include <vector>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <queue>
#include <thread>
#include <chrono>
#include <mutex>

#define FOR( i, a, b ) 		for( int i = a ; i < b ; ++i )
#define VPDD 				vector< pair< double, double> >
#define VVD 				vector< vector< double > > 
#define VI 					vector< int > 
#define VVI 				vector< VI >
#define VVPDI				vector< vector< pair< double, int > > >
#define PII 				pair< int, int > 
#define PB 					push_back

using namespace std;

const int MAX_TRIES = 4;
mutex mtx;
bool cond = true;
int toAdd;
VVPDI sorted;


int uniform( int m );
bool haveTime();
void findMst( int n, VVI & mst, VVD & dist );
void getInput( int & n, VPDD & input );
void initializeArrays( int n, VPDD & input, VVD & dist, VI& route );
void generatePermutation( int n, VI& route );
void createRouteFromMST( int n,  VVI &  mst, VI& route );
void DFS( int n, int start, int parent, VVI & mst, VI & way );
double calculateRoute( int n, VI &, VPDD & input );
double probability(double temp,double current,double T);
double getRandomNumber();
PII getTwoRandomNumbers(int n);
inline double checkSwap( int n, double tmpBest, VI & tmp, int i, int j, VPDD & input );
inline double getLength( VPDD& input, int i, int j );
struct triple{ int start; int stop; double length; };
void myTimer( int i ){
	this_thread::sleep_for( chrono::milliseconds(max(0,i-2) * 1000 + 800 + toAdd ));
	mtx.lock();
	cond = false;
	mtx.unlock();
}

class TSPCalculator{

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
	double bestRouteEverLen; // Result;
	double curRouteLen; // Current_Route_Length
	double previousLength;// Current_Route_Length
	double afterSwapLen;
	double T; // Temperature
	long long int iterationsCounter;
	VI route;

public:

	TSPCalculator( int n, VI & route, VPDD& input  ); 
	double calculateTSP( VVD & dist, VI & bestRoute, VPDD& input );

private:

	void controlTemperature();

};




int main(){

	int n;
	int time;
	VPDD input;
	VVD dist; 						//distances
	VI route;
	VVI mst;

	ios_base::sync_with_stdio(false);
	getInput( n, input );
	double rate = 1.0 - double(n)/15000.0;
	toAdd = 750 * abs(rate);

	cin >> time;
	if( time > n * 10 ) time = n * 10;
	thread thr( myTimer, time );

	initializeArrays( n, input, dist, route );

	findMst( n, mst, dist );
	createRouteFromMST( n, mst, route );

	cout << fixed ;
	TSPCalculator tsp( n, route, input );
	double result = tsp.calculateTSP( dist, route, input );

	cout << result << endl;
	
	for( auto i : route ) cerr << i << " ";
	
	cout << endl;

	thr.join();
	return 0;
}







//*****************************************************************************
void getInput( int & n, VPDD & input ){
	int t;
	double x, y;
	cin >> n;
	input = VPDD( n+1, make_pair( 0, 0 ) );
	FOR( i, 0, n ){
		
		cin >> t >> x >> y;

		input[ t ].first = x;
		input[ t ].second = y;
		
	}
}




void initializeArrays( int n, VPDD & input, VVD & dist, VI& route ){

	srand (time(NULL));

	sorted = VVPDI( n+1, vector< pair< double, int > >() );

	FOR( i, 0, n )
	FOR( j, i+1, n+1 ){
		double length = hypot( input[ i ].first  - input[ j ].first,
							 input[ i ].second - input[ j ].second );
		sorted[ i ].push_back( make_pair( length, j ) );
		sorted[ j ].push_back( make_pair( length, i ) );
	}

	auto cmp = []( pair< double, int> a, pair< double, int > b ){
		return a.first < b.first ;
	};
	FOR( i, 1, n+1 ){
		sort( sorted[ i ].begin(), sorted[ i ].end(), cmp );
	}


}




void generatePermutation( int n, VI& route ){

	FOR( i, 1, n-1 ){

		int j = uniform( n - i - 1);
		swap( route[ i ], route[ i+j ] );
	}

}





int uniform( int m ){
	return rand()%m;
}





double calculateRoute( int n, VI & route, VPDD & input ){
	double result = 0;
	FOR( i, 0, n ) result += getLength( input, route[ i ], route[ i+1 ] );
	return result;
}




inline double checkSwap( int n, double tmpBest, VI & tmp, int i, int j, VPDD & input ){
	if( abs( i - j ) > 1 ){ 
		tmpBest -= getLength( input, tmp[ i - 1 ], tmp[ i ]  );
		tmpBest -= getLength( input, tmp[ i ], tmp[ i+1 ]  );
		tmpBest -= getLength( input, tmp[ j-1 ], tmp[ j ]  );
		tmpBest -= getLength( input, tmp[ j], tmp[ j + 1 ]  );

		tmpBest += getLength( input, tmp[ i - 1 ], tmp[ j ]  );
		tmpBest += getLength( input, tmp[ j ], tmp[ i+1 ]  );
		tmpBest += getLength( input, tmp[ j-1 ], tmp[ i ]  );
		tmpBest += getLength( input, tmp[ i ], tmp[ j + 1 ]  );
	}
	else{
		if( j < i ) swap( i, j );
		tmpBest -= getLength( input, tmp[ i - 1 ], tmp[ i ] );
		tmpBest -= getLength( input, tmp[ j ], tmp[ j+1 ] );

		tmpBest += getLength( input, tmp[ i - 1 ], tmp[ j ] );
		tmpBest += getLength( input, tmp[ i ], tmp[ j+1 ] );
		
	}
	return tmpBest;
}






inline double getLength( VPDD& input, int i, int j ){
	double first = input[ i ].first - input[ j ].first;
	double second = input[ i ].second - input[ j ].second;
	return sqrt( first * first  + second * second );
}




void findMst( int n, VVI & mst, VVD & dist ){

	auto cmp = []( triple a, triple b ){ return a.length > b.length ;};
	priority_queue< triple, vector< triple >, decltype( cmp ) > pq( cmp );

	mst = VVI( n+1 );
	VI visited( n+1, 0 );
	visited[ 1 ] = 1;
	double result = 0;

	FOR( i, 0, n-1 ){
		triple x = { 1, sorted[ 1 ][ i ].second , sorted[ 1 ][ i ].first };
		pq.push( x );
	}

	FOR( i, 0, n-1 ){
		while( visited[ pq.top().stop ] == 1 ){ pq.pop(); }
		triple tmp = pq.top();
		pq.pop();
		
		result += tmp.length;
		mst[ tmp.start ].PB( tmp.stop );
		mst[ tmp.stop ].PB( tmp.start );
		visited[ tmp.stop ] = 1;
		
		FOR( i, 0, n-1 ){
			if( visited[ sorted[ tmp.stop ][ i ].second ] == 0 ){
				triple x = { tmp.stop, sorted[ tmp.stop ][ i ].second, sorted[ tmp.stop ][ i ].first };
				pq.push( x );
			}
		}
	}
}




void createRouteFromMST( int n,  VVI &  mst, VI& route ){

	VI visited( n+1 );

	vector< int > way;
	DFS( n, 1, -1, mst, way ); 

	for( auto i : way ){
		if( visited[ i ] == 0 ){
			route.PB( i );
			visited[ i ] = 1;
		}
	}
	route.PB( 1 );
}




void DFS( int n, int start, int parent, VVI & mst, VI & way ){
	way.PB( start );
	for( auto i : mst[ start ] ){
		if( i != parent )
			DFS(  n, i, start, mst, way );
	} 
	way.PB( start );
}




bool haveTime(){
	mtx.lock();
	bool result = cond;
	mtx.unlock();
	return result;
}




PII getTwoRandomNumbers(int n){
	PII result;
	result.first = rand()%(n-1) + 1;
	result.second = rand()%(n-1) + 1;
	while( result.first == result.second ){
		result.second = rand()%(n-1) + 1;
	}
	return result;
}





double probability(double temp, double current,double T){
	double tmp = exp( (temp - current )/T );
	double result = 1.0/(1.0+tmp);
	return result;
}





double getRandomNumber(){
	return (double)rand()/((double)RAND_MAX+1.0);
}




TSPCalculator::TSPCalculator( int n, VI & route, VPDD& input ){
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
}




double TSPCalculator::calculateTSP( VVD & dist, VI & bestRoute, VPDD& input ){

	while( haveTime() ){

		PII toSwap = getTwoRandomNumbers(n);
		afterSwapLen = checkSwap( n, curRouteLen, route, 
										toSwap.first, toSwap.second, input );

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









/*
double calculateTSP( int n, VVD & dist, VI & bestRoute, VPDD& input ){

	int INIT = 60;
	int start = INIT;
	int a = start * start/2, counter = 0, repetitionCounter = 0, ccounter = 0;
	double bestRouteEverLen = calculateRoute(n, bestRoute, input ); // Result;
	double curRouteLen = bestRouteEverLen; // Current_Route_Length
	double previousLength = bestRouteEverLen; // Current_Route_Length
	double afterSwapLen;
	double T = n/start; // Temperature
	long long int t = 0;
	VI route( bestRoute );

	while( haveTime() ){

		PII toSwap = getTwoRandomNumbers(n);
		afterSwapLen = checkSwap( n, curRouteLen, route, 
										toSwap.first, toSwap.second, input );

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
		t++;
		if( t%100000 == 0){
			if( curRouteLen == previousLength ) repetitionCounter++;
			else{
				repetitionCounter = 0;
				previousLength = curRouteLen;
			}
			T  = n / (start + t/100000);
			cout << curRouteLen << " " << counter << endl;
		}
		if( repetitionCounter == 10 ){
			repetitionCounter = 0;
			start += 15;
			counter++;
			t = 0;
		}
		if( counter == 10 ){ 
			counter = 0;
			ccounter++;
			start -= 140;
		}
		if( ccounter == 5){
			ccounter = 0;
			INIT += 5;
			start = INIT;
			t = 0;
		}

	}

	return bestRouteEverLen;
}












*/