//
//  Coordinate.h
//  Project1
//
//  Created by Austin Stamford on 2/3/19.
//  Copyright © 2019 Austin Stamford. All rights reserved.
//

#ifndef Coordinate_h
#define Coordinate_h

class Coord {
    
public:
    Coord(const int first, const int second, const int third){
        color = first;
        row = second;
        column = third;
    }
    
    Coord(){
        color = 0;
        row = 0;
        column = 0;
    }
    
    int color;
    int row;
    int column;
};

#endif /* Coordinate_h */
