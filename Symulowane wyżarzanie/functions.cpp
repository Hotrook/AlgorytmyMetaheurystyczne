#include "functions.hpp"

void getInput( int & n, VPDD & input ){
	int t;
	double x, y;
	cin >> n;
	input = VPDD( n+1, make_pair( 0, 0 ) );
	FOR( i, 0, n ){
		
		cin >> t >> x >> y;

		input[ t ].first = x;
		input[ t ].second = y;
		
	}
}




void generatePermutation( int n, VI& route ){
	FOR( i, 1, n-1 ){
		int j = uniform( n - i - 1);
		swap( route[ i ], route[ i+j ] );
	}
}




void findMst( int n, VVI & mst, VVD & dist, VPDD& input ){
	auto cmp = []( triple a, triple b ){ return a.length > b.length ;};
	priority_queue< triple, vector< triple >, decltype( cmp ) > pq( cmp );
	double result = 0;
	double length;
	triple newTriple;

	mst = VVI( n+1 );
	VI visited( n+1, 0 );
	visited[ 1 ] = 1;
	
	FOR( i, 2, n+1 ){
		length = hypot( 
			input[ 1 ].first  - input[ i ].first,
			input[ 1 ].second - input[ i ].second 
		);
	 	newTriple = { 1, i , length };
		pq.push( newTriple );
	}

	FOR( i, 0, n-1 ){
		while( visited[ pq.top().stop ] == 1 ){ 
			pq.pop();
		}
		newTriple = pq.top();
		pq.pop();
		
		result += newTriple.length;
		mst[ newTriple.start ].PB( newTriple.stop );
		mst[ newTriple.stop ].PB( newTriple.start );
		visited[ newTriple.stop ] = 1;
		
		FOR( i, 1, n+1 ){
			if( i != newTriple.stop and visited[ i ] == 0 ){
				length = hypot( 
					input[ i ].first  - input[ newTriple.stop ].first,
					input[ i ].second - input[ newTriple.stop ].second 
				);
				triple x = { newTriple.stop, i, length };

				pq.push( x );
			}
		}
	}
}




int uniform( int m ){
	return rand()%m;
}




void initRand(  ){
	srand (time(NULL));
}




void DFS( int n, int start, int parent, VVI & mst, VI & way ){
	way.PB( start );

	for( auto i : mst[ start ] ){
		if( i != parent ){
			DFS(  n, i, start, mst, way );
		}
	} 

	way.PB( start );
}




void createRouteFromMST( int n,  VVI &  mst, VI& route ){
	VI visited( n+1 );
	vector< int > way;

	
	DFS( n, 1, -1, mst, way ); 

	for( auto i : way ){
		if( visited[ i ] == 0 ){
			route.PB( i );
			visited[ i ] = 1;
		}
	}

	route.PB( 1 );
}




double calculateRoute( int n, VI & route, VPDD & input ){
	double result = 0;
	FOR( i, 0, n ){
		result += getLength( input, route[ i ], route[ i+1 ] );
	}
	return result;
}




double getLength( VPDD& input, int i, int j ){
	double first = input[ i ].first - input[ j ].first;
	double second = input[ i ].second - input[ j ].second;
	return sqrt( first * first  + second * second );
}




double probability(double temp, double current,double T){
	double tmp = exp( (temp - current )/T );
	double result = 1.0/(1.0+tmp);
	return result;
}




double getRandomNumber(){
	return (double)rand()/((double)RAND_MAX+1.0);
}




PII getTwoRandomNumbers(int n){
	PII result;
	result.first = rand()%(n-1) + 1;
	result.second = rand()%(n-1) + 1;
	while( result.first == result.second ){
		result.second = rand()%(n-1) + 1;
	}
	return result;
}




double checkSwap(int n, double tmpBest, VI & tmp, int i, int j, VPDD & input ){
	if( abs( i - j ) > 1 ){ 
		tmpBest -= getLength( input, tmp[ i - 1 ], tmp[ i ]  );
		tmpBest -= getLength( input, tmp[ i ], tmp[ i+1 ]  );
		tmpBest -= getLength( input, tmp[ j-1 ], tmp[ j ]  );
		tmpBest -= getLength( input, tmp[ j], tmp[ j + 1 ]  );

		tmpBest += getLength( input, tmp[ i - 1 ], tmp[ j ]  );
		tmpBest += getLength( input, tmp[ j ], tmp[ i+1 ]  );
		tmpBest += getLength( input, tmp[ j-1 ], tmp[ i ]  );
		tmpBest += getLength( input, tmp[ i ], tmp[ j + 1 ]  );
	}
	else{
		if( j < i ) swap( i, j );
		tmpBest -= getLength( input, tmp[ i - 1 ], tmp[ i ] );
		tmpBest -= getLength( input, tmp[ j ], tmp[ j+1 ] );

		tmpBest += getLength( input, tmp[ i - 1 ], tmp[ j ] );
		tmpBest += getLength( input, tmp[ i ], tmp[ j+1 ] );
		
	}
	return tmpBest;
}









void greedyPath(VI & p, VPDD & input, int n){
	int first = rand() % (n - 1) + 2;
	p.clear();
	p = VI( n+1 );
	VI visited = VI( n+1, 0 );

	p[ 0 ] = p[ n ] = 1;

	visited[ first ] = visited[ 1 ] = 1;
	p[ 1 ] = first;
	FOR( i, 2, n ){
		double min = -1;
		int pos = 0;

		FOR( j, 1, n+1 ){
			if( visited[ j ] == 0 and j != p[ i - 1 ] ){
				double tmp = getLength( input, p[ i - 1 ], j );
				if( tmp < min or min == -1 ){
					min = tmp;
					pos = j;
				}
			}
		}

		visited[ pos ] = 1;
		p[ i ] = pos;
	}

}





void up(VI & o, VPDD & input ){
	int n = o.size();
	FOR( i, 0 , 1000000 ){
		int first = uniform( n-1 ) + 1;
		int second = uniform( n-1 ) + 1;

		if( checkSwap( o, first, second, input ) ){
			swap( o[ first ], o[ second ] );
		}
	}
}







double checkSwap( VI & tmp, int i, int j, VPDD & input ){
	double tmpBest1 = 0;
	double tmpBest2 = 0;

	if( abs( i - j ) > 1 ){
		tmpBest1 += getLength( input, tmp[ i - 1 ], tmp[ i ]  );
		tmpBest1 += getLength( input, tmp[ i ], tmp[ i+1 ]  );
		tmpBest1 += getLength( input, tmp[ j-1 ], tmp[ j ]  );
		tmpBest1 += getLength( input, tmp[ j], tmp[ j + 1 ]  );

		tmpBest2 += getLength( input, tmp[ i - 1 ], tmp[ j ]  );
		tmpBest2 += getLength( input, tmp[ j ], tmp[ i+1 ]  );
		tmpBest2 += getLength( input, tmp[ j-1 ], tmp[ i ]  );
		tmpBest2 += getLength( input, tmp[ i ], tmp[ j + 1 ]  );
	}
	else{
		if( j < i ) swap( i, j );
		tmpBest1 += getLength( input, tmp[ i - 1 ], tmp[ i ] );
		tmpBest1 += getLength( input, tmp[ j ], tmp[ j+1 ] );

		tmpBest2 += getLength( input, tmp[ i - 1 ], tmp[ j ] );
		tmpBest2 += getLength( input, tmp[ i ], tmp[ j+1 ] );

	}
	return tmpBest2 < tmpBest1 ;
}

