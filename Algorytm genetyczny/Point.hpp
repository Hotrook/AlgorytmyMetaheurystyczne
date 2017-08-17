//
//  Point.hpp
//  OpenGLLearn
//
//  Created by Hotrook on 15.08.2017.
//  Copyright © 2017 Hotrook. All rights reserved.
//

#ifndef Point_hpp
#define Point_hpp

#include <stdio.h>

struct Point{
    double x;
    double y;
    
    Point( double x, double y) {
        this->x = x;
        this->y = y;
    }
};


#endif /* Point_hpp */
