//
//  main.cpp
//  Project1
//
//  Created by Austin Stamford on 1/19/19.
//  Copyright © 2019 Austin Stamford. All rights reserved.
//

#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <getopt.h>
#include "xcode_redirect.hpp"
#include "Board.h"

char dt;
std::string output = "";
//3D vector containing where we've been
//color, row, col
std::vector<std::vector<std::vector<int> > > discard_pile;
//current color, row, column
std::vector<int> current_state(3);

const int CARAT_ASCII = 94;

void get_options(int argc, char** argv);
std::vector<std::vector<char> > read_in_board(int num_rows, int argc, char** argv);
void print(Board board);



int main(int argc, char** argv) {
    /***********************************/
    std::ios_base::sync_with_stdio(false);
    xcode_redirect(argc, argv);
    /***********************************/
    int num_colors, num_rows, num_columns;
    std::cin >> num_colors
             >> num_rows
             >> num_columns;
    get_options(argc, argv);

    Board board(output, dt, num_colors, num_rows, num_columns);

    board.setarray(read_in_board(num_rows, argc, argv));
     
    print(board);
    
    
    return 0;
}

    //read in the board and store it in the vector in board
    //this function works. It's not amazing but it works.
    //the only thing is this uses the BoardArray member variable for essentially no reason
    //okay now it doesn't. Is this inefficient? It feels inefficient
 std::vector<std::vector<char> > read_in_board(int num_rows, int argc, char** argv){
    xcode_redirect(argc, argv);
    std::vector<std::vector<char> > grid;
    grid.resize(num_rows);
    std::string newline;
    std::vector<char> row;
    getline(std::cin, newline);
    int rowcount = 0;
    int colcount = 0;
    while(getline(std::cin, newline)){
        if(newline[0] != '/'){
            for(auto c : newline){
                row.push_back(c);
                colcount++;
                if(c == '@'){
                    //let's just set current_state right here
                    current_state[0] = CARAT_ASCII;
                    current_state[1] = rowcount;
                    current_state[2] = colcount;
                }
            }
            //makes a copy of row - this might be super inefficient
            grid[rowcount] = row;
            row.clear();
            rowcount++;
            colcount = 0;
        }
    }
     return grid;
}


//let's make a function that deals with input
void get_options(int argc, char** argv) {
    int option_index = 0, option = 0;
    
    // Don't display getopt error messages about options
    opterr = false;
    
    // use getopt to find command line options
    struct option longOpts[] = { { "queue", no_argument, nullptr, 'q' },
        { "stack", no_argument, nullptr, 's' },
        { "output", required_argument, nullptr, 'o' },
        { "help", no_argument, nullptr, 'h' },
        { nullptr, 0, nullptr, '\0' }
    };
    
    //process options
    while ((option = getopt_long(argc, argv, "qso:h", longOpts, &option_index)) != -1) {
        switch (option) {
            case 'q':
                dt = 'q';
                break;
                
            case 's':
                dt = 's';
                break;
                
            case 'o':
                output = optarg;
                break;
                
            case 'h':
                std::cout << "Under Construction" << std::endl;
                exit(0);
        }
    }
    
    // if no output is provided we want a map
    if (output == "") output = "map";
    
}

//prints contents of board
void print(Board board){
    for(auto r : board.BoardArray){
        for(auto c : r){
            std::cout << c;
        }
        std::cout << std::endl;
    }
}

//BIG algorithm boy
void algorithm(){
    //okay the start is the current state
    //we need a deque to contain reachable states
    //add the adjacent stuff with same color to the deque
    //CHECK whether adj states are in the board - what's an efficient way to do this?
    //take the next state and repeat
    //if current_state
}
