#ifndef FROST_CONTROLLER
#define FROST_CONTROLLER


#include <mutex>
#include <thread>
#include <cstdlib>
#include <cmath>
#include <chrono>
#include <cstdio>
#include <iostream>
#include "functions.hpp"

using namespace std;


struct Controller{
	mutex mtx;
	bool cond;

	Controller(){
		cond = true;
	}
};


void myTimer( int i, Controller * c );




#endif