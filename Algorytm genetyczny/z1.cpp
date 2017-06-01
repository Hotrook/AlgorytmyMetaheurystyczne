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

	VPDD input;
	VI route;
	VVI mst;
	Controller * c = new Controller();

	ios_base::sync_with_stdio(false);
	getInput( n, input );

	cin >> time;
	thread thr( myTimer, time, c );

	initRand();
	cout << fixed ;


	// findMst( n, mst, input );
	// createRouteFromMST( n, mst, route );

	Genetic * gen = new Genetic( n, c, input );
	double result = gen->calculate( route, input );




	cout << fixed ;
	cout << result << endl;

	for( auto i : route ){
		cerr << i << " ";
	}


	cout << endl;

	c->mtx.lock();
	if( c->cond == false ){
		thr.join();
	}
	c->mtx.unlock();

	exit(0);
	// return 0;
}
