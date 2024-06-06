#include <iostream>
#include <queue>
#include "MazeState.hpp"
#include "TimeKeeper.hpp"

using State = MazeState;

int chokudaiSearchAction(
    const State &state, 
    const int beam_width, 
    const int beam_depth, 
    const int beam_number, 
    const int64_t time_threshold = -1
) {
    auto beam = std::vector<std::priority_queue<State> >(beam_depth + 1);

    auto time_keeper = TimeKeeper(time_threshold);

    for (int t = 0; t < beam_depth + 1; t++) beam[t] = std::priority_queue<State>();
    beam[0].push(state);

    for (int cnt = 0;; cnt++) {
        if (time_threshold == -1 && cnt == beam_number) break;

        for (int t = 0; t < beam_depth; t++) {
            auto &now_beam = beam[t];
            auto &next_beam = beam[t+1];

            for (int i = 0; i < beam_width; i++) {
                if (now_beam.empty()) break;
                State now_state = now_beam.top();

                if (now_state.isDone()) break;
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

            if (time_keeper.isTimeOver()) break;
        }
    }
    for (int t = beam_depth; t >= 0; t--) {
        const auto &now_beam = beam[t];
        if (!now_beam.empty()) return now_beam.top().first_action_;
    }
    std::cout << "1" << std::endl;
    return -1;
}

void playGame(const int seed) {
    auto state = State(seed);
    std::cout << state.toString() << std::endl;

    while(!state.isDone()) {
        state.advance(chokudaiSearchAction(state, 1, END_TURN, 2));
        std::cout << state.toString() << std::endl;
    }
}

void testAiScore(const int game_number) {
    std::mt19937 mt_for_construct(0);
    double score_mean = 0;

    for (int i = 0; i < game_number; i++) {
        auto state = State(mt_for_construct());

        while(!state.isDone()) {
            state.advance(chokudaiSearchAction(state, 1, END_TURN, 0, 1));
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