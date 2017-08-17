//
//  Mapper.hpp
//  OpenGLLearn
///Users/sebastianmroz/Desktop/CODING/xCode/OpenGLLearn/OpenGLLearn/Mapper.cpp
//  Created by Sebastian Mróz on 17.08.2017.
//  Copyright © 2017 Sebastian Mróz. All rights reserved.
//

#ifndef Mapper_hpp
#define Mapper_hpp

#include <iostream>
#include <stdio.h>
#include <vector>
#include <cmath>
#include "Point.hpp"


void mapPointsToWindow();

double findMinX(std::vector<Point> & points );
double findMaxY(std::vector<Point> & points );

double findMinFactorX(double addFactorX, std::vector<Point> & points, double xSize );
double findMaxFactorX(double addFactorX, std::vector<Point> & points, double xSize );

double findMinFactorX(double addFactorY, std::vector<Point> & points, double ySize );
double findMaxFactorX(double addFactorY, std::vector<Point> & points, double ySize );

class Mapper {
public:
    Mapper( std::vector< Point > & s, std::vector< Point > & d, double xSize, double ySize) :
        source(s), dest(d), xSize(xSize), ySize(ySize){
            this->dest = s;
    }

    void mapPointsToWindow();

private:
    std::vector< Point > & source;
    std::vector< Point > & dest;
    double xSize;
    double ySize;

    void scakeAllValuesToStartFrom0();
    double findMinX();
    double findMaxY();
    double findMaxX();
    
    void scaleValues(double factor );
};

#endif /* Mapper_hpp */
