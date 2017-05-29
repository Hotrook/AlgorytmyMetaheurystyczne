#include <iostream>
#include <cstdio>
#include <vector>
#include <ctime>
#include <algorithm>
#include <thread>
#include "functions.hpp"
#include "TSPCalculator.hpp"

using namespace std;




int main(){

	int n;
	int time;

	double rate;

	VPDD input;
	VVD dist; 						//distances
	VI route;
	VVI mst;
	// TSPCalculator * tsp;
	Controller * c = new Controller();

	ios_base::sync_with_stdio(false);	
	getInput( n, input );

	cin >> time;
	thread thr( myTimer, time, c );
	if( time > n * 5 ) time = n * 5;

	initRand();

	findMst( n, mst, dist, input );
	createRouteFromMST( n, mst, route );

	TSPCalculator tsp( n, route, input, c );
	double result = tsp.calculateTSP( dist, route, input );

	cout << fixed ;
	cout << result << endl;
	
	for( auto i : route ){
		cerr << i << " ";
	}
	
	cout << endl;

	thr.join();

	return 0;
}











