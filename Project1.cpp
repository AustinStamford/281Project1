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

//current color, row, column
std::vector<int> current_state(3);
//endpoint
std::vector<int> endpoint(2);

Board board;


void get_options(int argc, char** argv);
void read_in_board(int num_rows, int argc, char** argv);
void print(Board board);
void algorithm(char dt, std::string output);


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

    read_in_board(num_rows, argc, argv);
     
    print(board);
    
    
    return 0;
}

    //read in the board and store it in the vector in board
    //this function works. It's not amazing but it works.
    //slightly less inefficient - may want to go back and pass in array by ref
 void read_in_board(int num_rows, int argc, char** argv){
    xcode_redirect(argc, argv);
    board.BoardArray.resize(num_rows);
    std::string newline;
    getline(std::cin, newline);
    int rowcount = 0;
    int colcount = 0;
    while(getline(std::cin, newline)){
        if(newline[0] != '/'){
            for(auto c : newline){
                board.BoardArray[rowcount].push_back(c);
                colcount++;
                if(c == '@'){
                    //let's just set current_state right here
                    current_state[0] = 0;//original color
                    current_state[1] = rowcount;
                    current_state[2] = colcount;
                    //find the endpoint too
                }
                else if(c == '?'){
                    endpoint[0] = rowcount;
                    endpoint[1] = colcount;
                }
            }
            //next row
            rowcount++;
            colcount = 0;
        }
    }
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
                board.setdt('q');
                break;
                
            case 's':
                board.setdt('q');
                break;
                
            case 'o':
                board.setoutput(optarg);
                break;
                
            case 'h':
                std::cout << "Under Construction" << std::endl;
                exit(0);
        }
    }
    
    // if no output is provided we want a map
    if(board.getoutput() == "") board.setoutput("map");
    
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
void algorithm(char dt, std::string output){
    //okay the start is the current state
    //we need a deque to contain reachable states
        /*MAKE A FUNCTION FOR THIS
        //add the adjacent stuff with same color to the deque
        //CHECK whether adj states are in the board - what's an efficient way to do this?
        //take the next state and add the last state to discard
         ENDOF*/
    //repeat
    //if current_state is a button what happens?
            /*MAKE A FUNCTION
             -we need to add the current coords with the new color to the reachable
             -and then add literally everything else available with the new color as well
             -need to condition for doors - i.e. if there's a new color then check if
              there are any new doors you can go through
             */
}
