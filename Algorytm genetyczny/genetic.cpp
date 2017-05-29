#include "genetic.hpp"


Genetic::Genetic(){

}




double Genetic::calculate( VI& bestRoute, VPDD& input ){
	double best = calculateLength( bestRoute, input );
	double current;

	while( haveTime() ){

	}

	return best;
} 



bool Genetic::haveTime(){
	c->mtx.lock();
	bool result = c->cond;
	c->mtx.unlock();
	return result;
}