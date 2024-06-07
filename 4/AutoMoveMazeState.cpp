#include <iostream>
#include <random>
#include "AutoMoveMazeState.hpp"

using State = AutoMoveMazeState;

State randomAction(const State &state) {
    State now_state = state;
    for (int character_id = 0; character_id < CHARACTER_N; character_id++) {
        int y = mt_for_action() % H;
        int x = mt_for_action() % W;
        now_state.setCharacter(character_id, y, x);
    }
    return now_state; 
}

void playGame(const int seed) {
    auto state = State(seed);
    state = randomAction(state);
    std::cout << state.toString() << std::endl;
    auto score = state.getScore(true);
    std::cout << "Score of randomAction" << ": " << score << std::endl; 
}

int main() {
    playGame(0);
    return 0;
}
