//
//  Mapper.cpp
//  OpenGLLearn
//
//  Created by Hotrook on 17.08.2017.
//  Copyright © 2017 Hotrook. All rights reserved.
//

#include "Mapper.hpp"


void Mapper::mapPointsToWindow(){
    
    scakeAllValuesToStartFrom0();
    
    double maxY = findMaxY();
    double maxX = findMaxX();
    
    double factorX = xSize / maxX;
    double factorY = ySize / maxY;
    
    double multFactor = std::min( factorX, factorY );
    scaleValues( multFactor );
    
    for( int i = 0 ; i < dest.size(); ++i ){
        std::cout << source[ i ].x << " " << source[ i ].y << " -> " << dest[ i ].x << " " << dest[ i ].y << std::endl;
    }
    
}

void Mapper::scakeAllValuesToStartFrom0(){
    double addFactorX = -1 * findMinX();
    double addFactorY = -1 * findMaxY();
    
    for( auto & i : dest ){
        i.x += addFactorX;
        i.y += addFactorY;
        i.y = std::abs( (double) i.y );
    }
}

bool allPointsInTheWindow(double minMultFactorX, double maxMultFactorY){
    return ( minMultFactorX >= 1 and maxMultFactorY >= 1 );
}

double Mapper::findMinX(){
    double minX = dest[ 1 ].x;
    
    for( int i = 1 ; i < dest.size() ; ++i ){
        minX = std::min(minX, dest[ i ].x );
    }
    
    return minX;
}

double Mapper::findMaxY(){
    double maxY = dest[ 1 ].y;
    
    for( int i = 1 ; i < dest.size() ; ++i ){
        maxY = std::max( maxY, dest[ i ].y );
    }
    return maxY;
}

double Mapper::findMaxX(){
    double maxX = dest[ 1 ].x;
    
    for( int i = 1 ; i < dest.size() ; ++i  ){
        maxX = std::max( maxX, dest[ i ].x );
    }
    return maxX;
}

void Mapper::scaleValues(double factor ){
    for( auto & i : dest ){
        i.x = i.x * factor;
        i.y = i.y * factor;
    }
}
