#include <iostream>
#include <cstdio>
#include <vector>
#include <ctime>
#include <algorithm>
#include <thread>
#include <chrono>
#include "functions.hpp"
#include "controller.hpp"
#include "genetic.hpp"

using namespace std;




int main(){

	int n;
	int time;

	double rate;

	VPDD input;
	VVD dist; 						//distances
	VI route;
	VVI mst;
	Controller * c = new Controller();

	ios_base::sync_with_stdio(false);	
	getInput( n, input );

	cin >> time;
	thread thr( myTimer, time, c );

	initRand();

	Genetic * gen = new Genetic( );
	double result = gen->calculate( route, input );




	cout << fixed ;
	cout << result << endl;
	
	for( auto i : route ){
		cerr << i << " ";
	}
	
	cout << endl;

	thr.join();

	return 0;
}











