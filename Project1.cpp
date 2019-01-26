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
bool algorithm(char dt, std::string output);
//this is probably super ineffecient but it's fine for now
void print_t(Board board);
bool already_visited(const std::vector<int> state);
void print_unreachable();
void map_output();
void list_output(std::vector<int> output);
void output();
std::string print_coords(int c, int x, int y);


int main(int argc, char** argv) {
    /***********DONT TOUCH**************/
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
    board.setoutput("list");
    
    read_in_board(num_rows, argc, argv);
    
    /** this is a super naive way to do this **/
    std::vector<std::vector<std::vector<char>>> tracker1
    (num_colors + 1, std::vector<std::vector<char>>(num_rows,
                                                    std::vector<char>(num_columns, '.')));
    board.tracker = tracker1;
    /***************************/
    
    print(board);
    //EDIT THIS DUMBASS
    if(algorithm('q', "list")){
        output();
    }
    else{
        //else puzzle is not solvable
        std::cout << "No solution.\nReachable:\n";
        print_unreachable();
    }
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

//returns whether a solution was found
bool algorithm(char dt, std::string output){
    reachable_states.push_back(current_state);
    //while loop that terminates when the deque is empty
    while(!reachable_states.empty()){
        //let's do queue later
        if(dt == 'q'){
            current_state = reachable_states.front();
            reachable_states.pop_front();
        }//endof queue
        //stack
        else{
            current_state = reachable_states.back();
            reachable_states.pop_back();
        }
        if(board.BoardArray[current_state[1]][current_state[2]] == '?'){
            return true;
        }
        //check north
        if(current_state[1] - 1 >= 0){
            if(board.BoardArray[current_state[1]-1][current_state[2]] == '.' ||
               board.BoardArray[current_state[1]-1][current_state[2]] =='?'){
                //i.e. north is valid
                if(!already_visited(std::vector<int>{current_state[0], current_state[1] - 1, current_state[2]})){
                    reachable_states.push_back(std::vector<int>{current_state[0], current_state[1] - 1, current_state[2]});
                    board.tracker[reachable_states.back()[0]][reachable_states.back()[1]][reachable_states.back()[2]] = 'v';
                }
            }
            //this is how we check for a button
            else if(board.BoardArray[current_state[1]-1][current_state[2]] - 96 > 0 &&
                    board.BoardArray[current_state[1]-1][current_state[2]] - 97 < board.getcolors()){
                if(!already_visited(std::vector<int>{
                    board.BoardArray[current_state[1]-1][current_state[2]] - 96,
                    current_state[1] - 1,
                    current_state[2]})){
                        reachable_states.push_back(std::vector<int>{board.BoardArray[current_state[1]-1][current_state[2]] - 96, current_state[1] - 1, current_state[2]});
                        board.tracker[current_state[0]][reachable_states.back()[1]][reachable_states.back()[2]] = 'v';
                        board.tracker[reachable_states.back()[0]][reachable_states.back()[1]][reachable_states.back()[2]] = '#';
                    }
            }
            //check for a door
            //64 b/c A is 65 and color a will be 1 in current_state, B is 65, b is 2, and so on
            else if(board.BoardArray[current_state[1]-1][current_state[2]] - current_state[0] == 64){
                if(!already_visited(std::vector<int>{current_state[0], current_state[1] - 1, current_state[2]})){
                    reachable_states.push_back(std::vector<int>{current_state[0], current_state[1] - 1, current_state[2]});
                    board.tracker[reachable_states.back()[0]][reachable_states.back()[1]][reachable_states.back()[2]] = 'v';
                }
            }
            
            
        }//endof norf norf
        //check right
        if(current_state[2] + 1 < board.getcols()){
            if(board.BoardArray[current_state[1]][current_state[2] + 1] == '.' ||
               board.BoardArray[current_state[1]][current_state[2] + 1] =='?'){
                //i.e. right is valid
                if(!already_visited(std::vector<int>{current_state[0], current_state[1], current_state[2] + 1})){
                    reachable_states.push_back(std::vector<int>{current_state[0], current_state[1], current_state[2] + 1});
                    board.tracker[reachable_states.back()[0]][reachable_states.back()[1]][reachable_states.back()[2]] = '<';
                }
            }
            //this is how we check for a button
            else if(board.BoardArray[current_state[1]][current_state[2] + 1] - 96 > 0 &&
                    board.BoardArray[current_state[1]][current_state[2] + 1] - 97 < board.getcolors()){
                if(!already_visited(std::vector<int>{
                    board.BoardArray[current_state[1]][current_state[2] + 1] - 96,
                    current_state[1],
                    current_state[2] + 1})){
                        reachable_states.push_back(std::vector<int>{board.BoardArray[current_state[1]][current_state[2] + 1] - 96, current_state[1], current_state[2] + 1});
                        board.tracker[current_state[0]][reachable_states.back()[1]][reachable_states.back()[2]] = '<';
                        board.tracker[reachable_states.back()[0]][reachable_states.back()[1]][reachable_states.back()[2]] = '#';
                    }
            }
            //right door
            //64 b/c A is 65 and color a will be 1 in current_state, B is 65, b is 2, and so on
            else if(board.BoardArray[current_state[1]][current_state[2] + 1] - current_state[0] == 64){
                if(!already_visited(std::vector<int>{current_state[0], current_state[1], current_state[2] + 1})){
                    reachable_states.push_back(std::vector<int>{current_state[0], current_state[1], current_state[2] + 1});
                    board.tracker[reachable_states.back()[0]][reachable_states.back()[1]][reachable_states.back()[2]] = '<';
                }
            }
        }//endof right
        //south
        if(current_state[1] + 1 < board.getrows()){
            if(board.BoardArray[current_state[1] + 1][current_state[2]] == '.' ||
               board.BoardArray[current_state[1] + 1][current_state[2]] =='?'){
                //i.e. south is valid
                if(!already_visited(std::vector<int>{current_state[0], current_state[1] + 1, current_state[2]})){
                    reachable_states.push_back(std::vector<int>{current_state[0], current_state[1] + 1, current_state[2]});
                    board.tracker[reachable_states.back()[0]][reachable_states.back()[1]][reachable_states.back()[2]] = '^';
                }
            }
            //this is how we check for a button
            else if(board.BoardArray[current_state[1]+1][current_state[2]] - 96 > 0 &&
                    board.BoardArray[current_state[1]+1][current_state[2]] - 97 < board.getcolors()){
                if(!already_visited(std::vector<int>{current_state[0], current_state[1] + 1, current_state[2]})){
                    reachable_states.push_back(std::vector<int>{board.BoardArray[current_state[1]+1][current_state[2]] - 96, current_state[1] + 1, current_state[2]});
                    board.tracker[current_state[0]][reachable_states.back()[1]][reachable_states.back()[2]] = '^';
                    board.tracker[reachable_states.back()[0]][reachable_states.back()[1]][reachable_states.back()[2]] = '#';
                }
            }
            //check for a door
            //64 b/c A is 65 and color a will be 1 in current_state, B is 65, b is 2, and so on
            else if(board.BoardArray[current_state[1]+1][current_state[2]] - current_state[0] == 64){
                if(!already_visited(std::vector<int>{current_state[0], current_state[1] + 1, current_state[2]})){
                    reachable_states.push_back(std::vector<int>{current_state[0], current_state[1] + 1, current_state[2]});
                    board.tracker[reachable_states.back()[0]][reachable_states.back()[1]][reachable_states.back()[2]] = '^';
                }
            }
        }
        //left
        if(current_state[2] - 1 >= 0){
            if(board.BoardArray[current_state[1]][current_state[2] - 1] == '.' ||
               board.BoardArray[current_state[1]][current_state[2] - 1] =='?'){
                //i.e. left is valid
                if(!already_visited(std::vector<int>{current_state[0], current_state[1], current_state[2] - 1})){
                    reachable_states.push_back(std::vector<int>{current_state[0], current_state[1], current_state[2] - 1});
                    board.tracker[reachable_states.back()[0]][reachable_states.back()[1]][reachable_states.back()[2]] = '>';
                }
            }
            else if(board.BoardArray[current_state[1]][current_state[2] - 1] - 96 > 0 &&
                    board.BoardArray[current_state[1]][current_state[2] - 1] - 97 < board.getcolors()){
                if(!already_visited(std::vector<int>{
                    board.BoardArray[current_state[1]][current_state[2] - 1] - 96,
                    current_state[1],
                    current_state[2] - 1})){
                        reachable_states.push_back(std::vector<int>{board.BoardArray[current_state[1]][current_state[2] - 1] - 96, current_state[1], current_state[2] - 1});
                        board.tracker[current_state[0]][reachable_states.back()[1]][reachable_states.back()[2]] = '>';
                        board.tracker[reachable_states.back()[0]][reachable_states.back()[1]][reachable_states.back()[2]] = '#';
                    }
            }
            //64 b/c A is 65 and color a will be 1 in current_state, B is 65, b is 2, and so on
            else if(board.BoardArray[current_state[1]][current_state[2] - 1] - current_state[0] == 64){
                if(!already_visited(std::vector<int>{current_state[0], current_state[1], current_state[2] - 1})){
                    reachable_states.push_back(std::vector<int>{current_state[0], current_state[1], current_state[2] - 1});
                    board.tracker[reachable_states.back()[0]][reachable_states.back()[1]][reachable_states.back()[2]] = '>';
                }
            }
            
        }//endof stack
    }//END WHILE NOT EMPTY
    return false;
}


bool already_visited(const std::vector<int> state){
    return board.tracker[state[0]][state[1]][state[2]] != '.';
}

//condition: map is unsolvable
void print_unreachable(){
    int pointcount = 0;
    //print out original map but with everything not reached '#'
    for(int x = 0; x < board.getrows(); x++){
        for(int y = 0; y < board.getcols(); y++){
            for(int c = 0; c < board.getcolors() + 1; c++){
                if(board.tracker[c][x][y] == '.'){
                    pointcount++;
                }
            }
            if(pointcount == 3 && board.BoardArray[x][y] != '^') board.BoardArray[x][y] = '#';
            pointcount = 0;
        }
    }
    print(board);
}

//handles output in list form
//let's do this first
void output(){
    int x = endpoint[0];
    int y = endpoint[1];
    int c = -1;
    std::deque<std::vector<int>> output;
    for(int i = 0; i < board.getcolors() + 1; i++){
        if(board.tracker[i][x][y] != '.'){
            c = i;
        }
    }
    if(c == -1){
        std:: cerr << "end not reached";
        exit(0);
    }
    output.push_back(std::vector<int>{c, x, y});
    while(board.BoardArray[x][y] != '@'){
        //look north
        if(board.tracker[c][x][y] == '^'){
            output.push_back(std::vector<int>{c, x - 1, y});
            x--;
        }
        else if(board.tracker[c][x][y] == '<'){
            output.push_back(std::vector<int>{c, x, y - 1});
            y--;
        }
        else if(board.tracker[c][x][y] == '>'){
            output.push_back(std::vector<int>{c, x, y + 1});
            y++;
        }
        else if(board.tracker[c][x][y] == 'v'){
            output.push_back(std::vector<int>{c, x + 1, y});
            x++;
        }
        else if(board.tracker[c][x][y] == '#'){
            output.push_back(std::vector<int>{c - 1, x, y});
            c--;
        }
    }
    if(board.getoutput() == "list"){
        while(!output.empty()){
            list_output(output.back());
            output.pop_back();
        }
    }
}

void list_output(std::vector<int> output){
    char color;
    std::string str = "";
    if(output[0] == 0) color = '^';
    else color = output[0] + 96;
    std::cout << "(" << color << ", (" << std::to_string(output[1])
    << ", " << std::to_string(output[2]) << "))\n";
}

//handles output in map form
void map_output(std::vector<int> output){
    //in parent function loop through to get here
    //match arg vector with tracker
    //Switch ^>v< to +
    //switch # to %
    //call modified print_t function
    //we're just straight modifying tracker here because we don't need it anymore
}

//std::string print_coords(int c, int x, int y){
//    char color;
//    std::string str = "";
//    if(c == 0) color = '^';
//    else color = c + 96;
//    str += "(";
//    str += color;
//    return (str + ", ("
//    + std::to_string(x) + ", " + std::to_string(y) + "))");
//}
