//
//  Drawer.cpp
//  OpenGLLearn
//
//  Created by Hotrook on 15.08.2017.
//  Copyright © 2017 Hotrook All rights reserved.
//

#include "Drawer.hpp"

sf::Color generateRandomColor(){
    return sf::Color(rand()%255, rand()%255,rand()%255);
}

void createCircle(Point & point, sf::CircleShape & circle){
    circle.setPosition(point.x - circle.getRadius(), point.y - circle.getRadius());
    circle.setFillColor(sf::Color(100,250,50));
}

void drawLine( Point & first, Point & second, sf::RenderWindow & window ){
    sf::Vertex line[] = {
        sf::Vector2f(sf::Vector2f(first.x, first.y)),
        sf::Vector2f(sf::Vector2f(second.x, second.y))
    };
    
    line[0].color = generateRandomColor();
    line[1].color = generateRandomColor();
    
    window.draw(line, 2, sf::Lines);
}

void generatePoints( std::vector<Point>& points, int size ){
    points.clear();
    for( int i = 0 ; i < size ; ++i ){
        int x = rand() % X_SIZE;
        int y = rand() % Y_SIZE;
        
        Point point( x, y );
        
        points.push_back(point);
    }
}

void generateRoute( std::vector< int > & route, int size ){
    route.clear();
    
    for( int i = 0 ; i < size ; ++i ){
        route.push_back( i );
    }
    
    for ( int i = 0 ; i < size - 1 ; ++i ) {
        int x = uniform2( size - 1 - i );
        std::swap( route[ i ], route[ i + x ]);
    }
    
}

void drawPaths(std::vector<Point> & points, std::vector< int > & route, sf::RenderWindow & window){
    for( int i = 0; i < route.size()-1 ; ++i ){
        drawLine( points[ route[ i ] ], points[ route[ i + 1 ] ], window);
    }
    drawLine( points[ route[ route.size() - 1 ] ], points[ route[ 0 ] ], window);
}

void drawCircles(std::vector<Point> & route, sf::RenderWindow & window){
    for( auto point : route ){
        sf::CircleShape circle(RADIUS);
        createCircle(point, circle);
        window.draw(circle);
    }
}

void drawRoute( std::vector<Point> & points, std::vector< int > & route, sf::RenderWindow & window ){
    drawPaths(points, route, window);
    drawCircles(points, window);
}

void permute( std::vector<Point> & route ){
    int permNumber = rand()%5;
    
    for( int i = 0 ; i < permNumber ; ++i ){
        int first = rand() % route.size();
        int second = rand() % route.size();
        
        std::swap( route[ first ], route[ second ]);
    }
}

void drawRoute(sf::RenderWindow * window, SynchronizedRoute * sroute){
    while( window->isOpen() ){
        sf::Event event;
        
        while( window->pollEvent(event)){
            if( event.type == sf::Event::Closed )
                window->close();
            
        }
        
        
        sroute->routeMutex.lock();
        if( sroute->updated ){
            window->clear();
            drawRoute( sroute->points, sroute->route, *window );
            window->display();
        }
        if( sroute->finish ){
            window->close();
        }
        sroute->routeMutex.unlock();
        
        std::this_thread::sleep_for (std::chrono::milliseconds(10));
    }
}

int uniform2( int scope ){
    return rand()%scope + 1;
}
