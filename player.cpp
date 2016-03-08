#include "player.h"
#include <chrono>

const Side my_side = BLACK;
const Side opp_side = WHITE;
const int board_values[8][8] = {{3,-2,2,2,2,2,-2,3},
                                {-2,-3,1,1,1,1,-3,-2},
                                {2,1,1,1,1,1,1,2},
                                {2,1,1,1,1,1,1,2},
                                {2,1,1,1,1,1,1,2},
                                {2,1,1,1,1,1,1,2},
                                {-2,-3,1,1,1,1,-3,-2},
                                {3,-2,2,2,2,2,-2,3}};
/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    my_board = Board();

}

/*
 * Destructor for the player.
 */
Player::~Player() {
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    my_board.doMove(opponentsMove, opp_side);
    Move* my_move = Player::getBestMove();
    if (my_move) {
        my_board.doMove(my_move, my_side);
    }
    return my_move;

}
Move* Player::getBestMove() {
    std::vector<Move*> moves = Player::possibleMoves(my_side);
    Move* best = NULL;
    if (moves.size() > 0) {
        best = moves[0];
        for (int i = 0; i < moves.size(); ++i) {
            if (Player::heuristic(moves[i], my_side) >
                    Player::heuristic(moves[0], my_side)) {
                best = moves[i];
            }
        }
    } return best;

}
std::vector<Move *> Player::possibleMoves(Side side){
    std::vector<Move*> moves;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Move *m = new Move(i,j);
            if(my_board.checkMove(m, side)) {
                moves.push_back(m);
            }
        }
    }
    return moves;
}

int Player::heuristic(Move *potential_move, Side side) {
    Board *potential_board = my_board.copy();
    potential_board->doMove(potential_move, side);
    int my_score = 0, opp_score = 0;

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if(potential_board->get(my_side, i, j)) {
                my_score += board_values[i][j];
            }
            if (potential_board->get(opp_side, i, j)){
                opp_score += board_values[i][j];
            }
        }
    }
    return my_score - opp_score;
}
