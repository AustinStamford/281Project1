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
//deque that stores reachable states
std::deque<std::vector<int> > reachable_states;

Board board;


void get_options(int argc, char** argv);
void read_in_board(int num_rows, int argc, char** argv);
void print(Board board);
void algorithm(char dt, std::string output);
//this is probably super ineffecient but it's fine for now
void print_t(Board board);
bool already_visited(const std::vector<int> state);


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
    
    board.setcols(num_columns);
    board.setrows(num_rows);
    board.setcolors(num_colors);
    
    read_in_board(num_rows, argc, argv);
    
    /** this is a super naive way to do this **/
    std::vector<std::vector<std::vector<char>>> tracker1
     (num_colors + 1, std::vector<std::vector<char>>(num_rows,
     std::vector<char>(num_columns, '.')));
    board.tracker = tracker1;
     /***************************/
    
    print(board);
    
    algorithm('s', "anna");
    std::cout << "\n\n";
    print_t(board);
    
    
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
                colcount++;
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
                board.setdt('s');
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

//prints contents of tracker
void print_t(Board board){
    for(auto r : board.tracker){
        std::cout << "new color" << std::endl;
        for(auto c1 : r){
            for(auto c2 : c1){
                std::cout << c2;
            }
        std::cout << std::endl;
        }
        std::cout << "\n\n\n";
    }
}

//BIG algorithm boy
void algorithm(char dt, std::string output){
    reachable_states.push_back(current_state);
    //while loop that terminates when the deque is empty
    while(!reachable_states.empty()){
        //let's do queue later
        if(dt == 'q'){
            std::cout << "Under Construction" << std::endl;
            exit(0);
        }//endof queue
        //stack
        else{
            while(already_visited(reachable_states.back())){
                reachable_states.pop_back();
            }
            current_state = reachable_states.back();
            reachable_states.pop_back();
            //check north
            if(current_state[1] - 1 >= 0){
               if(board.BoardArray[current_state[1]-1][current_state[2]] == '.'){
                //i.e. north is valid
                reachable_states.push_back(std::vector<int>{current_state[0], current_state[1] - 1, current_state[2]});
                board.tracker[reachable_states.back()[0]][reachable_states.back()[1]][reachable_states.back()[2]] = 'v';
               }
                //this is how we check for a button
               else if(board.BoardArray[current_state[1]-1][current_state[2]] - 96 > 0 &&
                       board.BoardArray[current_state[1]-1][current_state[2]] - 97 < board.getcolors()){
                   reachable_states.push_back(std::vector<int>{board.BoardArray[current_state[1]-1][current_state[2]] - 96, current_state[1] - 1, current_state[2]});
                   board.tracker[reachable_states.back()[0]][reachable_states.back()[1]][reachable_states.back()[2]] = '#';
               }
            }//endof norf norf
            //check right
            if(current_state[2] + 1 < board.getcols()){
                if(board.BoardArray[current_state[1]][current_state[2] + 1] == '.'){
                    //i.e. right is valid
                    reachable_states.push_back(std::vector<int>{current_state[0], current_state[1], current_state[2] + 1});
                    board.tracker[reachable_states.back()[0]][reachable_states.back()[1]][reachable_states.back()[2]] = '<';
                }
                //this is how we check for a button
                else if(board.BoardArray[current_state[1]][current_state[2] + 1] - 96 > 0 &&
                        board.BoardArray[current_state[1]][current_state[2] + 1] - 97 < board.getcolors()){
                    reachable_states.push_back(std::vector<int>{board.BoardArray[current_state[1]][current_state[2] + 1] - 96, current_state[1], current_state[2] + 1});
                    board.tracker[reachable_states.back()[0]][reachable_states.back()[1]][reachable_states.back()[2]] = '#';
                }
            }//endof right
            //south
            if(current_state[1] + 1 < board.getrows()){
                if(board.BoardArray[current_state[1] + 1][current_state[2]] == '.'){
                //i.e. south is valid
                reachable_states.push_back(std::vector<int>{current_state[0], current_state[1] + 1, current_state[2]});
                board.tracker[reachable_states.back()[0]][reachable_states.back()[1]][reachable_states.back()[2]] = '^';
            }
                //this is how we check for a button
                else if(board.BoardArray[current_state[1]+1][current_state[2]] - 96 > 0 &&
                        board.BoardArray[current_state[1]+1][current_state[2]] - 97 < board.getcolors()){
                    reachable_states.push_back(std::vector<int>{board.BoardArray[current_state[1]+1][current_state[2]] - 96, current_state[1] + 1, current_state[2]});
                    board.tracker[reachable_states.back()[0]][reachable_states.back()[1]][reachable_states.back()[2]] = '#';
                }
            }
            //left
            if(current_state[2] - 1 >= 0){
                    if(board.BoardArray[current_state[1]][current_state[2] - 1] == '.'){
                //i.e. left is valid
                reachable_states.push_back(std::vector<int>{current_state[0], current_state[1], current_state[2] - 1});
                board.tracker[reachable_states.back()[0]][reachable_states.back()[1]][reachable_states.back()[2]] = '>';
            }
                    else if(board.BoardArray[current_state[1]][current_state[2] - 1] - 96 > 0 &&
                            board.BoardArray[current_state[1]][current_state[2] - 1] - 97 < board.getcolors()){
                        reachable_states.push_back(std::vector<int>{board.BoardArray[current_state[1]][current_state[2] - 1] - 96, current_state[1], current_state[2] - 1});
                        board.tracker[reachable_states.back()[0]][reachable_states.back()[1]][reachable_states.back()[2]] = '#';
                    }
            }
            std::cout << "reachable state back : " << reachable_states.back()[0] << reachable_states.back()[1] << reachable_states.back()[2] << std::endl;
        }//endof stack
        
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
    }//END WHILE NOT EMPTY
}

bool already_visited(const std::vector<int> state){
    //check north
    if(state[1] - 1 >= 0 &&
       board.BoardArray[state[1]-1][state[2]] != '.'){
        return true;
    }
    //check south
    if(state[1] + 1 < board.getrows() &&
       board.BoardArray[state[1] + 1][state[2]] != '.'){
        return true;
    }
    //check left
    if(state[2] - 1 >= 0 &&
       board.BoardArray[state[1]][state[2] - 1] != '.'){
        return true;
    }
    //check right
    if(state[2] + 1 < board.getcols() &&
       board.BoardArray[state[1]][state[2] + 1] != '.'){
        return true;
    }
    return false;
}

