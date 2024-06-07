#include <iostream>
#include <random>
#include "AutoMoveMazeState.hpp"

using State = AutoMoveMazeState;

State hillClimb(const State &state, int number) {
    State now_state = state;
    now_state.init();
    ScoreType best_score = now_state.getScore();
    for (int i = 0; i < number; i++) {
        auto next_state = now_state;
        next_state.transition();
        auto next_score = next_state.getScore();
        if (next_score > best_score) {
            best_score = next_score;
            now_state = next_state;
        }
    }
    return now_state; 
}

void playGame(const int seed) {
    auto state = State(seed);
    state = hillClimb(state, 10000);
    std::cout << state.toString() << std::endl;
    auto score = state.getScore(true);
    std::cout << "Score of HillClimb" << ": " << score << std::endl; 
}

void testAiScore(const int simulate_number, const int game_number) {
    std::mt19937 mt_for_construct(0);
    double score_mean = 0;
    for (int i = 0; i < game_number; i++) {
        auto state = State(mt_for_construct());
        state = hillClimb(state, simulate_number);
        auto score = state.getScore(false);
        score_mean += score;
    }
    score_mean /= (double)game_number;
    std::cout << "Score of Simulated Annealing : " << score_mean << std::endl; 
}

int main() {
    std::cout << "0: run, Otherwise: test" << std::endl;
    bool isTest = false;
    std::cin >> isTest;
    int simulate_number = 10000; // limit number of evaluation
    int game_number = 1000;      // number of game sinario
    if (!isTest) playGame(0);
    else testAiScore(simulate_number, game_number);
    return 0;
}
