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
	time = ( time * 2 )/3;
	if( time > n ) time = n;
	if( time > 200 ) time = 200; 
	thread thr( myTimer, time, c );

	initRand();



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
