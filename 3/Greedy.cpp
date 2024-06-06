#include <iostream>
#include <random>
#include "MazeState.hpp"

using State = MazeState;
std::mt19937 mt_for_action(0);

constexpr const ScoreType INF = 1000000000LL;

int greedyAction(const State &state) {
    auto legal_actions = state.legalActions();
    ScoreType best_score = -INF;
    int best_action = -1;
    
    for (const auto action : legal_actions) {
        State now_state = state;
        now_state.advance(action);
        now_state.evaluateScore();
        if (now_state.evaluated_score_ > best_score) {
            best_score = now_state.evaluated_score_;
            best_action = action;
        }
    }
    return best_action;
}

void playGame(const int seed) {
    auto state = State(seed);
    std::cout << state.toString() << std::endl;

    while(!state.isDone()) {
        state.advance(greedyAction(state));
        std::cout << state.toString() << std::endl;
    }
}

void testAiScore(const int game_number) {
    std::mt19937 mt_for_construct(0);
    double score_mean = 0;

    for (int i = 0; i < game_number; i++) {
        auto state = State(mt_for_construct());

        while(!state.isDone()) {
            state.advance(greedyAction(state));
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