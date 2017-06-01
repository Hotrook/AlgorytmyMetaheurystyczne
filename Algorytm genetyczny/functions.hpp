#ifndef FROST_FUNCTIONS
#define FROST_FUNCTIONS
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <queue>



using namespace std;

#define FOR( i, a, b ) 		for( int i = a ; i < b ; ++i )
#define VPDD 				vector< pair< double, double> >
#define VVD 				vector< vector< double > >
#define VI 					vector< int >
#define VD 					vector< double >
#define VVI 				vector< VI >
#define VVPDI				vector< vector< pair< double, int > > >
#define PII 				pair< int, int >
#define PID 				pair< int, double >
#define PB 					push_back
#define MP 					make_pair

struct triple{ int start; int stop; double length; };


void getInput( int & n, VPDD & input );
void generatePermutation( int n, VI& route );
void findMst( int n, VVI & mst, VPDD& input );

int uniform( int m );
void initRand();
void DFS( int n, int start, int parent, VVI & mst, VI & way );

void createRouteFromMST( int n,  VVI &  mst, VI& route );
double calculateRoute( int n, VI &, VPDD & input );
double getLength( VPDD& input, int i, int j );

double probability(double temp,double current,double T);
double getRandomNumber();
PII getTwoRandomNumbers(int n);

double checkSwap( VI & tmp, int i, int j, VPDD & input );
double calculateLength( VI& path, VPDD& input );

void greedyPath(VI & p, VPDD & input, int n);

#endif
