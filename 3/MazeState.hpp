#pragma once

#include <random>
#include <vector>
#include <sstream>

struct Coord {
    int y_;
    int x_;
    Coord(const int y = 0, const int x = 0): y_(y), x_(x) {}
};

constexpr const int H = 30;
constexpr const int W = 30;
constexpr const int END_TURN = 100;

using ScoreType = int64_t;

class MazeState {
private:
    int points_[H][W] = {};
    int turn_ = 0;

public:
    Coord character_ = Coord();
    int game_score_ = 0;
    ScoreType evaluated_score_ = 0;
    int first_action_ = -1; 

    MazeState() {}

    static constexpr const int dx[4] = {1, -1, 0, 0};
    static constexpr const int dy[4] = {0, 0, 1, -1};

    MazeState(const int seed) {
        auto mt_for_construct = std::mt19937(seed);
        this->character_.y_ = mt_for_construct() % H;
        this->character_.x_ = mt_for_construct() % W;
    

        for (int y = 0; y < H; y++) {
            for (int x = 0; x < W; x++) {
                if (y == character_.y_ && x == character_.x_) continue;
                this-> points_[y][x] = mt_for_construct() % 10;
            }
        }
    }

    bool isDone() const {
        return this->turn_ == END_TURN;
    }

    void advance(const int action) {
        this->character_.x_ += dx[action];
        this->character_.y_ += dy[action];
        auto &point = this->points_[this->character_.y_][this->character_.x_];

        if (point > 0) {
            this->game_score_ += point;
            point = 0;
        }
        this->turn_++;
    }

    std::vector<int> legalActions() const {
        std::vector<int> actions;
        for (int action = 0; action < 4; action++) {
            int ty = this->character_.y_ + dy[action];
            int tx = this->character_.x_ + dx[action];
        
            if(ty >= 0 && ty < H && tx >= 0 && tx < W) {
                actions.emplace_back(action);
            }
        }
        return actions;
    }

    std::string toString() const {
        std::stringstream ss;
        ss << "turn:\t" << this->turn_ << "\n";
        ss << "score:\t" << this->game_score_ << "\n";
        
        for (int h = 0; h < H; h++) {
            for (int w = 0; w < W; w++) {
                if (this->character_.y_ == h && this->character_.x_ == w) ss << "@";
                else if (this->points_[h][w] > 0) ss << points_[h][w];
                else ss << ".";
            }
            ss << "\n";
        }
        return ss.str();
    }

    void evaluateScore() {
        this->evaluated_score_ = this->game_score_;
    } 

    bool operator < (const MazeState& other) const {
        return this->evaluated_score_ < other.evaluated_score_;
    }
};