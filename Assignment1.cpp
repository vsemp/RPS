//
//  main.cpp
//  move
//
//  Created by Vladimir Pchelin on 9/24/17.
//  Copyright © 2017 Vladimir Pchelin. All rights reserved.
//


// My code should compile with C++11

#include <iostream>
#include <map>
#include <vector>
#include <time.h>
#include <assert.h>

using namespace std; // bad style

// Internal manager of the game
struct manager {
    enum class Result {
        win,
        draw,
        loss,
    };
    enum class Symbol {
        rock,
        paper,
        scissors,
    };
    
    manager(){
        history[Result::win] = 0;
        history[Result::draw] = 0;
        history[Result::loss] = 0;
    }
    
    // A getter function
    std::map<Result, uint> record() {
        return history;
    }
    
    // This function plays the game once and save the record.
    // player - the hand a human player wants to play
    // Returns the outcome of the game
    Result play(Symbol player) {
        Result result = outcome(player);
        record(result);
        return result;
    }
    
    // This function shows last hand of your opponent
    // The assumption is that you played at least once.
    Symbol show_opponent() const {
        assert(opponent_history.size() > 0);
        return opponent_history.back();
    }
private:
    
    std::vector<Symbol> player_history;
    std::vector<Symbol> opponent_history;
    std::map<Result, uint> history;
    
    // Compares two symbols
    // Returns a result for "player"
    Result outcome(Symbol player, Symbol opponent);
    
    // Compares a human player symbol with a computer symbol
    // Returns a result for "player"
    Result outcome(Symbol player);
    
    // Records a result in "history"
    // result - should be a result from the last game
    std::map<Result, uint> record(Result result) {
        history[result]++;
        return history;
    }
};

manager::Result manager::outcome(Symbol player) {
    srand(uint(time(NULL)));
    Symbol opponent = static_cast<Symbol>(rand() % 3);
    return outcome(player, opponent);
}

manager::Result manager::outcome(Symbol player, Symbol opponent) {
    player_history.push_back(player);
    opponent_history.push_back(opponent);
    if ( player == opponent ) {
        return Result::draw;
    }
    else if ( player == Symbol::paper && opponent == Symbol::rock ) {
        return Result::win;
    }
    else if ( player == Symbol::rock && opponent == Symbol::scissors ) {
        return Result::win;
    }
    else if ( player == Symbol::scissors && opponent == Symbol::paper ) {
        return Result::win;
    }
    else {
        return Result::loss;
    }
}

// Command line interface to play the game
class interface {
    manager game;
    uint lifetime = 0;
    map<manager::Result, string> result_str;
    map<string, manager::Symbol> str_symbol;
    map<manager::Symbol, string> symbol_str;
    
    // Displays statistics at the end
    void end();
    
    // Plays one turn
    void play();
    
public:
    
    interface() {
        result_str[manager::Result::win] = "win";
        result_str[manager::Result::draw] = "draw";
        result_str[manager::Result::loss] = "loss";
        str_symbol["paper"] = manager::Symbol::paper;
        str_symbol["rock"] = manager::Symbol::rock;
        str_symbol["scissors"] = manager::Symbol::scissors;
        symbol_str[manager::Symbol::paper] = "paper";
        symbol_str[manager::Symbol::rock] = "rock";
        symbol_str[manager::Symbol::scissors] = "scissors";
    }
    
    // Function to start the game.
    void start();
};

void interface::start(){
    cout << "Welcome to the game Paper-Rock-Scissors.\n"
    "Please enter a positive number of games you want to play.\n"
    "Enter anything else to exit\n";
    int n;
    if ( cin >> n && n > 0 ) {
        lifetime = n;
        cin.ignore();
        play();
    }
    else {
        return;
    }
}

void interface::play() {
    if (lifetime > 0) {
        cout << "Please enter paper / rock / scissors / exit.\n";
        string line;
        if (getline(cin, line)) {
            if ( line == "exit" ) {
                end();
            }
            else if ( str_symbol.count(line) ) {
                cout << result_str[game.play(str_symbol[line])] << endl;
                cout << "Your opponent played " + symbol_str[game.show_opponent()] << endl;
                lifetime--;
                play();
            }
            else {
                play();
            }
        }
        else {
            play();
        }
    }
    else {
        end();
    }
}

void interface::end() {
    cout << "\n"
    "You finished the game.\n"
    "Your cumulative statistics:\n";
    cout << "wins: " << game.record()[manager::Result::win] << endl;
    cout << "draws: " << game.record()[manager::Result::draw] << endl;
    cout << "losses: " << game.record()[manager::Result::loss] << endl;
}


int main(int argc, const char * argv[]) {
    interface game;
    game.start();
    return 0;
}

