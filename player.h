#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.h"
#include "board.h"
#include <vector>
#include <map>
using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();
    
    Move *doMove(Move *opponentsMove, int msLeft);
    std::vector<Move*> possibleMoves(Board board, Side side);
    int heuristic(Board board, Move* potential_move, Side side);
    Move* singleHeuristic();
    Move* miniMax();
    Move* miniMaxNaive();
    Board my_board;
    int naiveHeuristic(Board board, Move* potential_move, Side side);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
