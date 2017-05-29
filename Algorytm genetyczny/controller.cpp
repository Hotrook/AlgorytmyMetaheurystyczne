#include "controller.hpp"


void myTimer( int i, Controller * c ){
	this_thread::sleep_for( chrono::milliseconds(max(0,i-2) * 1000 + 800 ));
	c->mtx.lock();
	c->cond = false;
	c->mtx.unlock();
}