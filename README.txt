Throughout the process of creating this 'AI' of sorts, I decided it best to
create a heuristic centered around tracking and tabulating the different
values of each cell on the board--for example, giving the highest possible
individual score to the corners, an elevated score to tiles adjacent to the
corners, the lowest possible scores to those diagonal to the corners, etc. In
doing so, we can then attempt to find which moves would result in the 'highest'
score, so to say, or which ones would generally be the most prudent. Using the
minimax paradigm to expand our foresight, we could use this heuristic to choose
the 'best' move that would result in the highest future score. Overall, after a
few plies of depth, this would overall enhance the efficacy of our AI and would
reap the most favorable outcome.