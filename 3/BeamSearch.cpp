#include <iostream>
#include <queue>
#include "MazeState.hpp"
#include "TimeKeeper.hpp"

using State = MazeState;

int beamSearchAction(
    const State &state, 
    const int beam_width, 
    const int beam_depth, 
    const int64_t time_threshold = -1
) {
    std::priority_queue<State> now_beam;
    State best_state;
    now_beam.push(state);

    auto time_keeper = TimeKeeper(time_threshold);

    for (int t = 0;; t++) {
        if (time_threshold == -1 && t == beam_depth) break;
        std::priority_queue<State> next_beam;
        
        for(int i = 0; i < beam_width; i++) {
            if (now_beam.empty()) break;
            if (time_threshold != -1 && time_keeper.isTimeOver()) return best_state.first_action_;
            
            State now_state = now_beam.top();
            now_beam.pop();
            auto legal_actions = now_state.legalActions();
            
            for (const auto &action : legal_actions) {
                State next_state = now_state;
                next_state.advance(action);
                next_state.evaluateScore();
                if (t == 0) next_state.first_action_ = action;
                next_beam.push(next_state);
            }
        }

        now_beam = next_beam;
        best_state = now_beam.top();

        if (best_state.isDone()) break;
    }
    return best_state.first_action_;
}

void playGame(const int seed) {
    auto state = State(seed);
    std::cout << state.toString() << std::endl;

    while(!state.isDone()) {
        state.advance(beamSearchAction(state, 2, END_TURN));
        std::cout << state.toString() << std::endl;
    }
}

void testAiScore(const int game_number) {
    std::mt19937 mt_for_construct(0);
    double score_mean = 0;

    for (int i = 0; i < game_number; i++) {
        auto state = State(mt_for_construct());

        while(!state.isDone()) {
            state.advance(beamSearchAction(state, 5, END_TURN));
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