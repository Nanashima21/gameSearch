#include <iostream>
#include <random>
#include "MazeState.hpp"

using State = MazeState;
std::mt19937 mt_for_action(0);

int randomAction(const State &state) {
    auto legal_actions = state.legalActions();
    return legal_actions[mt_for_action() % (legal_actions.size())];    
}

void playGame(const int seed) {
    auto state = State(seed);
    std::cout << state.toString() << std::endl;

    while(!state.isDone()) {
        state.advance(randomAction(state));
        std::cout << state.toString() << std::endl;
    }
}

void testAiScore(const int game_number) {
    std::mt19937 mt_for_construct(0);
    double score_mean = 0;

    for (int i = 0; i < game_number; i++) {
        auto state = State(mt_for_construct());

        while(!state.isDone()) {
            state.advance(randomAction(state));
        }
        auto score = state.game_score_;
        score_mean += score;
    }
    score_mean /= (double)game_number;
    std::cout << "Score:\t" << score_mean << std::endl;
}

int main() {
    std::cout << "0: run, Otherwise: test" << std::endl;
    bool isTest = false;
    std::cin >> isTest;
    if (!isTest) playGame(121321);
    else testAiScore(100);
    return 0;
}