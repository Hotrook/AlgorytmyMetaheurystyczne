#include "controller.hpp"


void myTimer( int time, Controller * c ){
	FOR( j, 0, time-2){
		this_thread::sleep_for( chrono::milliseconds(1000) );
	}
	this_thread::sleep_for(chrono::milliseconds( 500 ) );
	c->mtx.lock();
	c->cond = false;
	c->mtx.unlock();
}