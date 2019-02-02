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
//endpoint
std::vector<int> start_point(2);
//deque that stores reachable states
std::deque<std::vector<int> > reachable_states;


Board board;


void get_options(int argc, char** argv);
void read_in_board(int num_rows, int argc, char** argv);
void print(Board board);
bool algorithm(char dt);
//this is probably super ineffecient but it's fine for now
void print_t(Board board);
bool already_visited(const std::vector<int> &state);
void print_unreachable();
void map_output(std::vector<int> output,
                std::vector<std::vector<std::vector<char> > > &trackercopy);
void list_output(std::vector<int> output);
void output();
std::string print_coords(int c, int x, int y);

//algo helpers
void open_move(int co, int x, int y, char ch);
void button(int co_old, int x, int y, char ch);
void trap(int co, int x, int y, char ch);
bool valid_open(int x, int y, int c);
bool valid_button(int x, int y);
bool valid_door(int x, int y, int c);
bool valid_trap(int x, int y, int c);
void move(int x, int y, int c, char c1, char c2);
bool colorchange(int x, int y, int c);
bool isTrapOrButton(int x, int y);



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
    
    read_in_board(num_rows, argc, argv);
    
    /** this is a super naive way to do this **/
    std::vector<std::vector<std::vector<char>>> tracker1
    (num_colors + 1, std::vector<std::vector<char>>(num_rows,
                                                    std::vector<char>(num_columns, '.')));
    board.tracker = tracker1;
    /***************************/
    
    //print(board);
    if(algorithm(board.getdt())){
        output();
    }
    else{
        //else puzzle is not solvable
        std::cout << "No solution.\nReachable:\n";
        print_unreachable();
    }
    //print_t(board);
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
                    start_point[0] = rowcount; //original color
                    start_point[1] = colcount;
                    current_state[0] = 0;
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
                break;
                
            default:
                std::cerr << "Yikes" << std::endl;
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
void print_t(std::vector<std::vector<std::vector<char> > > tracker){
    char c = '^';
    for(int i = 0; i < board.getcolors() + 1; i++){
        if(i != 0) c = static_cast<char>(i + 96);
        std::cout << "// color "<< c << "\n";
        for(auto c1 : tracker[i]){
            for(auto c2 : c1){
                std::cout << c2;
            }
            std::cout << "\n";
        }
    }
}

//returns whether a solution was found
bool algorithm(char dt){
    int c, x, y;
    reachable_states.push_back(current_state);
    //while loop that terminates when the deque is empty
    while(!reachable_states.empty()){
        if(dt == 'q'){
            current_state = reachable_states.front();
            reachable_states.pop_front();
        }//endof queue
        //stack
        else{
            current_state = reachable_states.back();
            reachable_states.pop_back();
        }
        
        c = current_state[0];
        x = current_state[1];
        y = current_state[2];
        
        if(board.BoardArray[x][y] == '?'){
            return true;
        }
        
        //CHECK IF CURRENT IS A TRAP OR BUTTON
        if(isTrapOrButton(x, y) && colorchange(x, y, c)){
            //if button
            if(valid_button(x, y)){
                if(!already_visited(std::vector<int>{board.BoardArray[x][y] - 96, x, y})){
                    //so this is already added to the tracker in the NESW part
                    //this makes me think I JUST need to add to reachable states here.
                    reachable_states.push_back(std::vector<int>{
                        (board.BoardArray[x][y] - 96), x, y});
                    //mark new using capital letter for old
                    if(c == 0){
                        board.tracker[board.BoardArray[x][y] - 96][x][y] = '!' ;
                    }
                    else board.tracker[board.BoardArray[x][y] - 96][x][y] = static_cast<char>(c + 64);
                }
                c = board.BoardArray[x][y] - 96;
                current_state[0] = board.BoardArray[x][y] - 96;
            }
            else{
                if(!already_visited(std::vector<int>{0, x, y})){
                    reachable_states.push_back(std::vector<int>{0, x, y});
                    board.tracker[0][x][y] = static_cast<char>(c + 96);
                }
            }
            
        }
        else{
            //check north
            if(x - 1 >= 0){
                move(x - 1, y, c, '-', '}');
            }
            //check right
            if(y + 1 < board.getcols()){
                move(x, y + 1, c, '<', '|');
            }//endof right
            //south
            if(x + 1 < board.getrows()){
                move(x + 1, y, c, '^', '{');
            }
            //left
            if(y - 1 >= 0){
                move(x, y - 1, c, '>', '~');
            }
        }
    }//endof stack
    //END WHILE NOT EMPTY
    print_t(board.tracker);
    return false;
}


bool already_visited(const std::vector<int> &state){
    return board.tracker[state[0]][state[1]][state[2]] != '.';
}

//condition: map is unsolvable
void print_unreachable(){
    bool reachable = false;
    //print out original map but with everything not reached '#'
    for(int x = 0; x < board.getrows(); x++){
        for(int y = 0; y < board.getcols(); y++){
            for(int c = 0; c < board.getcolors() + 1; c++){
                if(board.tracker[c][x][y] != '.'){
                    reachable = true;
                }
            }
            if(!reachable) board.BoardArray[x][y] = '#';
            reachable = false;
        }
    }
    print(board);
}

//handles output
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
    //    if(c == -1){
    //        std:: cerr << "end not reached";
    //        exit(0);
    //    }
    output.push_back(std::vector<int>{c, x, y});
    while(!(board.BoardArray[x][y] == '@' && c == 0)){
        //look north
        if(board.tracker[c][x][y] == '^' || board.tracker[c][x][y] == '{'){
            output.push_back(std::vector<int>{c, x - 1, y});
            x--;
        }
        else if(board.tracker[c][x][y] == '<' || board.tracker[c][x][y] == '|'){
            output.push_back(std::vector<int>{c, x, y - 1});
            y--;
        }
        else if(board.tracker[c][x][y] == '>' || board.tracker[c][x][y] == '~'){
            output.push_back(std::vector<int>{c, x, y + 1});
            y++;
        }
        else if(board.tracker[c][x][y] == '-' || board.tracker[c][x][y] == '}'){
            output.push_back(std::vector<int>{c, x + 1, y});
            x++;
        }
        else if(board.tracker[c][x][y] > 64 && board.tracker[c][x][y] < 91){
            output.push_back(std::vector<int>{board.tracker[c][x][y] - 64, x, y});
            c = board.tracker[c][x][y] - 64;
            //            if(board.BoardArray[x][y] == 'a'){
            //                output.push_back(std::vector<int>{0, x, y});
            //                c = 0;
            //            }
            //            else{
            //                output.push_back(std::vector<int>{c - (board.BoardArray[x][y] - 97), x, y});
            //                c-= (board.BoardArray[x][y] - 97);
            //            }
        }
        else if(board.tracker[c][x][y] > 96 && board.tracker[c][x][y] < 123){
            c+= (board.tracker[c][x][y] - 96);
            output.push_back(std::vector<int>{c, x, y});
        }
        else if(board.tracker[c][x][y] == '!'){
            output.push_back(std::vector<int>{0, x, y});
            c = 0;
        }
    }
    if(board.getoutput() == "list"){
        while(!output.empty()){
            list_output(output.back());
            output.pop_back();
        }
    }
    else{
        std::vector<std::vector<std::vector<char> > > trackercopy = board.tracker;
        for(int i = 0; i < board.getcolors() + 1; i++){
            trackercopy[i] = board.BoardArray;
            if(i != 0) trackercopy[i][start_point[0]][start_point[1]] = '.';
        }
        for(int i = 0; i < board.getrows(); i++){
            for(int j = 0; j < board.getcols(); j++){
                if(board.BoardArray[i][j] < 91 && board.BoardArray[i][j] > 64){
                    trackercopy[board.BoardArray[i][j] - 64][i][j] = '.';
                }
                else if(board.BoardArray[i][j] < 123 && board.BoardArray[i][j] > 96){
                    trackercopy[board.BoardArray[i][j] - 96][i][j] = '.';
                }
                else if(board.BoardArray[i][j] == '^'){
                    trackercopy[0][i][j] = '.';
                }
            }
        }
        trackercopy[output.front()[0]][output.front()[1]][output.front()[2]] = '?';
        output.pop_front();
        while(!output.empty()){
            map_output(output.back(), trackercopy);
            output.pop_back();
        }
        print_t(trackercopy);
    }
}

void list_output(std::vector<int> output){
    char color;
    if(output[0] == 0) color = '^';
    else color = static_cast<char>(output[0] + 96);
    std::cout << "(" << color << ", (" << std::to_string(output[1])
    << ", " << std::to_string(output[2]) << "))\n";
}

//handles output in map form
//important to know what the input for this function is:
//only need to deal with THIS input
void map_output(std::vector<int> output, std::vector<std::vector<std::vector<char> > > &trackercopy){
    //in parent function loop through to get here
    char c = board.tracker[output[0]][output[1]][output[2]];
    //match arg vector with tracker
    //Switch ^>v< to +
    if(output == std::vector<int>{0, start_point[0], start_point[1]}){
        trackercopy[output[0]][output[1]][output[2]] = '@';
        return;
    }
    if(c == '^' || c == '-' || c == '<' || c == '>'){
        trackercopy[output[0]][output[1]][output[2]] = '+';
    }
    else if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '!')){
        trackercopy[output[0]][output[1]][output[2]] = '@';
    }
    else if(c == '}' || c == '|' || c == '{' || c == '~' ){
        trackercopy[output[0]][output[1]][output[2]] = '%';
    }
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

void open_move(int co, int x, int y, char ch){
    reachable_states.push_back(std::vector<int>{co, x, y});
    board.tracker[co][x][y] = ch;
}

void button(int co_old, int x, int y, char ch){
    //co old color, x row, y col, ch marking  }|{~
    board.tracker[co_old][x][y] = ch;
    reachable_states.push_back(std::vector<int>{co_old, x, y});
}

void trap(int co, int x, int y, char ch){
    
    //track which direction we came from in the co-level grid
    board.tracker[co][x][y] = ch;
    reachable_states.push_back(std::vector<int>{co, x, y});
}

bool valid_open(int x, int y, int c){
    return (board.BoardArray[x][y] == '.' ||
            board.BoardArray[x][y] == '@' ||
            board.BoardArray[x][y] =='?' ||
            (board.BoardArray[x][y] == '^' && c == 0));
}

bool valid_button(int x, int y){
    return (board.BoardArray[x][y] - 96 > 0 &&
            board.BoardArray[x][y] - 97 < board.getcolors());
}

bool valid_door(int x, int y, int c){
    return (board.BoardArray[x][y] - c == 64);
}

bool valid_trap(int x, int y, int c){
    return (board.BoardArray[x][y] == '^' && c != 0);
}


void move(int x, int y, int c, char c1, char c2){
    if(valid_open(x, y, c)){
        if(!already_visited(std::vector<int>{c, x, y})){
            open_move(c, x, y, c1);
        }
    }
    //this is how we check for a button
    else if(valid_button(x, y)){
        if(!already_visited(std::vector<int>{
            board.BoardArray[x][y] - 96, x, y}) &&
           !already_visited(std::vector<int>{
            c, x, y})){
                button(c, x,  y, c2);
            }
    }
    //check for a door
    //64 b/c A is 65 and color a will be 1 in current_state, B is 65, b is 2, and so on
    else if(valid_door(x, y, c)){
        if(!already_visited(std::vector<int>{c, x, y})){
            open_move(c, x, y, c1);
        }
    }
    //check for a trap
    else if(valid_trap(x, y, c)){
        if(!already_visited(std::vector<int>{c, x, y})){
            trap(c, x, y, c2);
        }
        
    }
    
}

bool isTrapOrButton(int x, int y){
    return (board.BoardArray[x][y] == '^' ||
            (board.BoardArray[x][y] > 96 && board.BoardArray[x][y] < 123));
}

bool colorchange(int x, int y, int c){
    if(valid_button(x, y)) return static_cast<char>(c + 96) != board.BoardArray[x][y];
    else return (board.BoardArray[x][y] == '^' && c !=0);
}
