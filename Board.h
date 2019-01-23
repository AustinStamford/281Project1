#ifndef BOARD_H
#define BOARD_H
//
//  Board.h
//  Project1
//
//  Created by Austin Stamford on 1/19/19.
//  Copyright © 2019 Austin Stamford. All rights reserved.
//

#include <vector>

//template <typename T>
class Board {
    
public:
    //constructor stores number of colors rows, columns
    //actually we probably need data types in here too
    Board(const std::string output, const char dt, const int colors, const int rows, const int columns){
        num_colors = colors;
        num_rows = rows;
        num_columns = columns;
        //this might be rows but I think it's columns
        BoardArray.reserve(num_columns);
    }
    
    //function that reads lines of input in and initializes the board 2D vector
    //make this a vector of chars instead of a string
    void read(const std::vector<char> row){
        BoardArray.push_back(row);
    }
    
    //I want to write a function that searches through the grid and gets ALL important symbols and adds them to a SWEET map along with their (x,y) coordinates as a pair
    //This can for sure be done when I read everything in
    //Just requires me to rewrite that completely
    /*IDEAS:
        I know how many colors I have so I can do a little for loop and use ASCII values to store those bad boys
     */
    //DO I EVEN NEED THIS?
    //for now I';m just gonna add the start to the read function
    void getSymbols(){
        //we want to get the ascii codes of the correct letters
        //loop through the board - linearly for now
        /*ask the board if the current char is a:
         -?
         -@
         -capital letter
         -lowercase letter
         */
    }
    
    //row getter
    int getrows(){
        return num_rows;
    }
    
    //col getter
    int getcols(){
        return num_columns;
    }
    
    //color getter
    int getcolors(){
        return num_colors;
    }
    
    //array setter
    void setarray(std::vector<std::vector<char> > array){
        BoardArray = array;
    }
    
    std::vector<std::vector<char> > BoardArray;
    
    std::pair<int, int> Start;
    
    
private:
    int num_colors;
    int num_rows;
    int num_columns;
};

#endif /* BOARD_H */
