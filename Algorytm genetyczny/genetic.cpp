#include "genetic.hpp"


Genetic::Genetic( int n, Controller * c,  VPDD & input) :
		currentPaths(VVI()), 
		reproduction(VVI()),
		finishTime(true){
	this->n = n;
	this->c = c;
	this->input = input;
	parentNumber = 50;
	childNumber = 4;
	noImproveCounter = 0;
	controlTime = 1000;
	if( n > 5000 ){
		controlTime = 500;
	}
	if( n > 10000 ){
		controlTime = 200;
	}

	currentLengths = VD( parentNumber );
}




double Genetic::calculate( VI& bestRoute, VPDD& input ){
	double bestLength;

	generateFirstGeneration();
	bestRoute = currentPaths[ 0 ];
	bestLength = calculateLength( currentPaths[ 0 ], input );
	double initial = bestLength;
	double last = 0;
	int iterationCounter = 0;

	while( haveTime() ){

		createNextGeneration();
		selection( );

		if( currentLengths[ 0 ] < bestLength ){
			noImproveCounter = 0;
			bestLength = currentLengths[ 0 ];
			bestRoute = currentPaths[ 0 ];
		}
		else{ 
			noImproveCounter++;
			if( noImproveCounter % 10 == 0 ){
				mutateAll();
			}
			if( noImproveCounter % 1000 == 0 ){
				finishTime = false;
			}
		}

		iterationCounter++;
		if( iterationCounter % controlTime == 0 ){
			double tmp = (initial - bestLength) / initial;
			if( tmp - last < 0.00015 ){
				finishTime = false;
			}
			last = tmp;
		}
	}

	return bestLength;
}



bool Genetic::haveTime(){
	c->mtx.lock();
	bool result = c->cond;
	c->mtx.unlock();
	return result & finishTime;
}



void Genetic::createNextGeneration(){

	reproduction.clear();
	reproduction = VVI();
	
	FOR( i, 0, parentNumber ){
		// if( rand() % 10 < 6 )
		up( currentPaths[ i ] );
		reproduction.PB( currentPaths[ i ] );
	}

	FOR( i, 0 , parentNumber ){
		int scope;
		if( i >= parentNumber - childNumber-1 ){ // Not enough possibilities to choose when choosing second parent index bigger than current one.
			scope = parentNumber;
			for( int j = 0 ; j < childNumber ; j++ ){
				int parent = uniform( scope );
				combine( i, parent );
			}
		} else {
			scope = parentNumber - i;
			for( int j = 0 ; j < childNumber ; j++ ){
				int parent = uniform( scope );
				combine( i, i + parent );
			}
		}
	}

}



void Genetic::generateFirstGeneration(){
	VI tmpPath = VI();
	int greedyPaths = 30;

	FOR( i, 0, greedyPaths ){
		greedyPath( tmpPath, input, n );
		currentPaths.PB( tmpPath );
	}
	FOR( i, 0 , parentNumber - greedyPaths ){
		init_and_permute( tmpPath );
		currentPaths.PB( tmpPath );
	}
	FOR( i, 0, parentNumber ){
		FOR( j, 0, 1000){
			up( currentPaths[ i ] );
		}
	}
}



void Genetic::init_and_permute( VI& path ){
	path.clear();
	path = VI( n + 1, 0);

	path[ 0 ] = 1;
	path[ n ] = 1;
	FOR( i, 1, n ){
		path[ i ] = i+1;
	}

	FOR( i, 1, n-1 ){
		int pos = uniform( n - i );
		swap( path[ i ], path[ i + pos ] );
	}

}



void Genetic::selection(){
	vector< PID > help;

	for( int i = 0 ; i < reproduction.size() ; ++i ){
		double length = calculateLength( reproduction[ i ], input );
		help.PB( MP( i, length ) );
	}

	auto cmp = []( PID a, PID b ){ return a.second < b.second; };
	sort( help.begin(), help.end(), cmp );

	currentPaths.clear();
	currentPaths = VVI(parentNumber);

	for( int i = 0; i < parentNumber; ++i ){
		currentPaths[ i ] = reproduction[ help[ i ].first ];
		currentLengths[ i ] = help[ i ].second; 
	}

}



void Genetic::combine( int first, int second ){
	VI o1 = VI( n+1 );
	VI o2 = VI( n+1 );

	int num = rand()%99;
	// if( num < 50 )
		// pmx( o1, o2, first, second );
	// else 
		ox( o1, o2, first, second );

	mutate( o1 );
	mutate( o2 );

	reproduction.PB( o1 );
	reproduction.PB( o2 );
}




void Genetic::pmx( VI & o1, VI & o2, int first, int second ){
	int firstPivot = uniform( n-2 ) + 1;
	int secondPivot = firstPivot + 1 + uniform( n - firstPivot - 1);
	VI conflicts1( n+1, 0 );
	VI conflicts2( n+1, 0 );

	o1[ 0 ] = o2[ 0 ] = o1[ n ] = o2[ n ] = 1;

	FOR( i, firstPivot, secondPivot + 1 ){
		o1[ i ] = currentPaths[ second ][ i ];
		o2[ i ] = currentPaths[ first ][ i ];
		conflicts1[ o1[ i ] ] = o2[ i ];
		conflicts2[ o2[ i ] ] = o1[ i ];
	}

	int i = 0;
	while( i < n+1 ){
		if( i == firstPivot ){
			i = secondPivot + 1;
		}

		determineValue( o1, first, i, conflicts1 );
		determineValue( o2, second, i, conflicts2 );

		i++;
	}

}




void Genetic::ox(VI & o1, VI & o2, int first, int second){
	int firstPivot = uniform( n-2 ) + 1;
	int secondPivot = firstPivot + 1 + uniform( n - firstPivot - 1);
	VI conflicts1( n+1, 0 );
	VI conflicts2( n+1, 0 );

	o1[ 0 ] = o2[ 0 ] = o1[ n ] = o2[ n ] = 1;

	FOR( i, firstPivot, secondPivot + 1 ){
		o1[ i ] = currentPaths[ first ][ i ];
		o2[ i ] = currentPaths[ second ][ i ];
		conflicts1[ o1[ i ] ] = 1;
		conflicts2[ o2[ i ] ] = 1;
	}

	int it1 = firstPivot > 1 ? 1 : secondPivot+1;
	int it2 = firstPivot > 1 ? 1 : secondPivot+1;
	FOR( i, secondPivot + 1, n ){
		if( conflicts1[ currentPaths[ second ][ i ] ] == 0 ){
			o1[ it1 ] = currentPaths[ second ][ i ];
			it1 = (it1 + 1 == firstPivot) ? secondPivot+1 : it1+1;
		}
		if( conflicts2[ currentPaths[ first ][ i ] ] == 0 ){
			o2[ it2 ] = currentPaths[ first ][ i ];
			it2 = (it2 + 1 == firstPivot) ? secondPivot+1 : it2+1;
		}
	}

	FOR( i, 1, secondPivot+1 ){
		if( conflicts1[ currentPaths[ second ][ i ] ] == 0 ){
			o1[ it1 ] = currentPaths[ second ][ i ];
			it1 = (it1 + 1 == firstPivot) ? secondPivot+1 : it1+1;
		}
		if( conflicts2[ currentPaths[ first ][ i ] ] == 0 ){
			o2[ it2 ] = currentPaths[ first ][ i ];
			it2 = (it2 + 1 == firstPivot )? secondPivot+1 : it2+1;
		}
	}

}




void Genetic::cx(VI & o1, int first, int second){

}




void Genetic::determineValue( VI & o, int pathIndex, int index, VI & conflicts ){

		if( conflicts[ currentPaths[ pathIndex ][ index ] ] == 0 ){
			o[ index ] = currentPaths[ pathIndex ][ index ];
		} else {
			int tmp = currentPaths[ pathIndex ][ index ];
			while( conflicts[ tmp ] != 0 ){
				tmp =  conflicts[ tmp ];
			}
			o[ index ] = tmp;
		}
}




void Genetic::addNewPermutations(){
	VI tmp;

	FOR( i, 0, parentNumber/2){
		init_and_permute( tmp );
		currentPaths[ i ] = tmp;
	}
}




void Genetic::mutate(VI & o ){
	double p = (double)rand()/RAND_MAX;
	int maxPermutations = n / 5;

	if( p < 0.1 ){
		int mutationsNumber = rand()%maxPermutations;
		FOR( i, 0, mutationsNumber ){
			int first = uniform( n-1 ) + 1;
			int second = uniform( n-1 ) + 1;
			
			swap( o[ first ], o[ second ] );
		}
	}
}




void Genetic::mutateAll(){
	FOR( i, 0, parentNumber ){
		mutate( currentPaths[ i ] );
	}
}





void Genetic::hardFirstGeneration(){
	VI tmp;
	FOR( i, 1, parentNumber ){
		tmp = currentPaths[ 0 ];
		hardMutate( tmp );
		currentPaths.PB( tmp );
	}
}




void Genetic::hardMutate(VI & tmp ){
	int maxPermutations = n / 20;

	int mutationsNumber = rand()%maxPermutations;
	FOR( i, 0, mutationsNumber ){
		int first = uniform( n-1 ) + 1;
		int second = uniform( n-1 ) + 1;
		
		swap( tmp[ first ], tmp[ second ] );
	}

}



void Genetic::up(VI & o ){
	FOR( i, 0 , 1000 ){
		int first = uniform( n-1 ) + 1;
		int second = uniform( n-1 ) + 1;

		if( checkSwap( o, first, second, input ) ){
			swap( o[ first ], o[ second ] );
		}
	}
}