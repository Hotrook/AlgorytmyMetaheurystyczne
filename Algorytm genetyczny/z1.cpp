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
#include "Drawer.hpp"

using namespace std;

void applier(SynchronizedRoute * sr, Genetic * genetic){
	double result = genetic->calculate( sr->route, sr->points );

	cout << fixed ;
	cout << result << endl;
}

void czikita(int i , vector<Point> * input){
	cout << " lol udalo sie zal pl" << endl;
}

int main(){

	ios_base::sync_with_stdio(false);

	int n;
	int time;

	std::vector< Point > input;
	std::vector< int > route;
	
	Controller * c = new Controller();

	getInput( n, input );

	cin >> time;

	time = ( time * 2 )/3;
	if( time > n ) time = n;
	if( time > 200 ) time = 200; 

	thread thr( myTimer, time, c );

	initRand();

	std::mutex routeMutex;
	SynchronizedRoute * sr = new SynchronizedRoute( input, route, routeMutex );


	Genetic * gen = new Genetic( n, c, input, sr );
	thread xd( applier, sr , gen );

    std::this_thread::sleep_for (std::chrono::seconds(3));


	sf::RenderWindow * window = new sf::RenderWindow( sf::VideoMode(X_SIZE, Y_SIZE), "My Window");

	drawRoute( window, sr);
	xd.join();
	// double result = gen->calculate( route, input );
	// thread algr( applier, route, input, gen );
	// algr.join();


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
