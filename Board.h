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
    
    //row setter
    void setrows(int r){
        num_rows = r;
    }
    
    //col setter
    void setcols(int c){
        num_columns = c;
    }
    
    //color setter
    void setcolors(int cr){
        num_colors = cr;
    }
    
    //datatype setter
    void setdt(char c){
        dt = c;
    }
    
    //output setter
    void setoutput(std::string s){
        output = s;
    }
    
    //output getter
    std::string getoutput(){
        return output;
    }
    
    //dt getter
    char getdt(){
        return dt;
    }
    

    //row, col
    std::vector<std::vector<char> > BoardArray;
    
private:
    int num_colors;
    int num_rows;
    int num_columns;
    char dt;
    std::string output;
};

#endif /* BOARD_H */
