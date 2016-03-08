#include "player.h"
#include <chrono>

Side my_side = WHITE;
Side opp_side = BLACK;
const int board_values[8][8] = {{99,-8,8,6,6,8,-8,99},
                                {-8,-24,-4,-3,-3,-4,-24,-8},
                                {8,-4,7,4,4,7,-4,8},
                                {6,-3,4,0,0,4,-3,6},
                                {6,-3,4,0,0,4,-3,6},
                                {8,-4,7,4,4,7,-4,8},
                                {-8,-24,-4,-3,-3,-4,-24,-8},
                                {99,-8,8,6,6,8,-8,99}};

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    my_board = Board();
    if(side == BLACK) {
        my_side = BLACK;
        opp_side = WHITE;
    }
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
    if (testingMinimax) {
        Move* my_move = Player::miniMaxNaive();
        if (my_move) {
            my_board.doMove(my_move, my_side);
        }
        return my_move;
    } else {
        Move *my_move = Player::miniMax();
        if (my_move) {
            my_board.doMove(my_move, my_side);
        }
        return my_move;
    }
}

/*
 * Computes best move based off of a single heuristic score
 */
Move* Player::singleHeuristic() {
    std::vector<Move*> moves = Player::possibleMoves(my_board, my_side);
    Move* best = NULL;
    if (moves.size() > 0) {
        best = moves[0];
        for (int i = 0; i < moves.size(); ++i) {
            if (Player::heuristic(my_board, moves[i], my_side) >
                    Player::heuristic(my_board, moves[0], my_side)) {
                best = moves[i];
            }
        }
    } return best;

}
/*
 * Gets the best move based on the minmax technique
 */
Move* Player::miniMax(){
    std::vector<Move*> ply1_moves = Player::possibleMoves(my_board, my_side);
    if (ply1_moves.size() == 0) {
        return NULL;
    } else {
        std::vector < Move * > ply2_moves;
        int values[ply1_moves.size()];
        for (int i = 0; i < ply1_moves.size(); i++) {
            values[i] = Player::heuristic(my_board, ply1_moves[i], my_side);
        }
        for (int i = 0; i < ply1_moves.size(); i++) {
            Board *potential_board = my_board.copy();
            potential_board->doMove(ply1_moves[i], my_side);
            ply2_moves = Player::possibleMoves(*potential_board, opp_side);
            for (int j = 0; j < ply2_moves.size(); ++j) {
                if (Player::heuristic(*potential_board, ply2_moves[j], opp_side)
                    < values[i]) {
                    values[i] =
                            Player::heuristic(*potential_board,
                                              ply2_moves[j], opp_side);
                }
            }
        }
        int highest = 0;
        for (int i = 0; i < ply1_moves.size(); ++i) {
            if (values[i] > values[highest]) {
                highest = i;
            }
        }
        return ply1_moves[highest];
    }
}

/*
 * Gets the best move based on the minmax technique using the naive heuristic
 */
Move* Player::miniMaxNaive(){
    std::vector<Move*> ply1_moves = Player::possibleMoves(my_board, my_side);
    if (ply1_moves.size() == 0) {
        return NULL;
    } else {
        std::vector < Move * > ply2_moves;
        int values[ply1_moves.size()];
        for (int i = 0; i < ply1_moves.size(); i++) {
            values[i] = Player::naiveHeuristic(my_board,
                                               ply1_moves[i], my_side);
        }
        for (int i = 0; i < ply1_moves.size(); i++) {
            Board *potential_board = my_board.copy();
            potential_board->doMove(ply1_moves[i], my_side);
            ply2_moves = Player::possibleMoves(*potential_board, opp_side);
            for (int j = 0; j < ply2_moves.size(); ++j) {
                if (Player::naiveHeuristic(*potential_board, ply2_moves[j],
                                           opp_side) < values[i]) {
                    values[i] =
                            Player::naiveHeuristic(*potential_board,
                                              ply2_moves[j], opp_side);
                }
            }
        }
        int highest = 0;
        for (int i = 0; i < ply1_moves.size(); ++i) {
            if (values[i] > values[highest]) {
                highest = i;
            }
        }
        return ply1_moves[highest];
    }
}

/*
 * Finds all possible moves for a given side
 *
 * Returns a vector of pointers to each move
 */
std::vector<Move *> Player::possibleMoves(Board board, Side side){
    std::vector<Move*> moves;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Move *m = new Move(i,j);
            if(board.checkMove(m, side)) {
                moves.push_back(m);
            }
        }
    }
    return moves;
}

/*
 * Calculates a score based on a given move played on the current board
 *
 * Returns an int value of the score--bigger is better!
 */
int Player::heuristic(Board board, Move *potential_move, Side side) {
    Board *potential_board = board.copy();
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

/*
 * Calculates a score based on the number of pieces on the board
 */
int Player::naiveHeuristic(Board board, Move *potential_move, Side side) {
    Board *potential_board = board.copy();
    potential_board->doMove(potential_move, side);

    return potential_board->countBlack() - potential_board->countWhite();
}
