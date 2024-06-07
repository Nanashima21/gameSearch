#include <iostream>
#include <random>
#include <sstream>
#include <functional>
#include <string>
#include <utility>

struct Coord {
    int y_;
    int x_;
    Coord(const int y = 0, const int x = 0): y_(y), x_(x) {}
};

constexpr const int H = 5;
constexpr const int W = 5;
constexpr const int END_TURN = 5;
constexpr const int CHARACTER_N = 3;

using ScoreType = int64_t;

constexpr const ScoreType INF = 1000000000LL;

std::mt19937 mt_for_action(0);

class AutoMoveMazeState {
private:
    int points_[H][W] = {};
    int turn_;
    Coord characters_[CHARACTER_N] = {};
public:
    int game_score_;
    ScoreType evaluated_score_;

    static constexpr const int dx[4] = {1, -1, 0, 0};
    static constexpr const int dy[4] = {0, 0, 1, -1};

    AutoMoveMazeState(const int seed): turn_(0), game_score_(0), evaluated_score_(0) {
        auto mt_for_construct = std::mt19937(seed);
        for (int y = 0; y < H; y++) {
            for (int x = 0; x < W; x++) {
                points_[y][x] = mt_for_construct() % 9 + 1;
            }
        }
    }

    void setCharacter(const int character_id, const int y, const int x) {
        this->characters_[character_id].y_ = y;
        this->characters_[character_id].x_ = x;
    }

    bool isDone() const {
        return this->turn_ == END_TURN;
    }

    std::string toString() const {
        std::stringstream ss;
        ss << "turn:\t" << this->turn_ << "\n";
        ss << "score:\t" << this->game_score_ << "\n";
        
        for (int h = 0; h < H; h++) {
            for (int w = 0; w < W; w++) {
                int id = -1;
                for (int i = 0; i < CHARACTER_N; i++) {
                    if (this->characters_[i].y_ == h && this->characters_[i].x_ == w) id = i;
                }  
                if (id != -1) ss << char(id + 'A');
                else if (this->points_[h][w] > 0) ss << points_[h][w];
                else ss << ".";
            }
            ss << "\n";
        }
        return ss.str();
    }

    ScoreType getScore(bool is_print = false) const {
        auto tmp_state = *this;
        for (auto &character : this->characters_) {
            auto &point = tmp_state.points_[character.y_][character.x_];
            point = 0;
        }
        while (!tmp_state.isDone()) {
            tmp_state.advance();
            if (is_print) std::cout << tmp_state.toString() << std::endl;
        }
        return tmp_state.game_score_;
    }

    void movePlayer(const int character_id) {
        Coord &character = this->characters_[character_id];
        int best_point = -INF;
        int best_action_index = 0;

        for (int action = 0; action < 4; action++) {
            int ty = character.y_ + dy[action];
            int tx = character.x_ + dx[action];

            if (ty >= 0 && ty < H && tx >= 0 && tx < W) {
                auto point = this->points_[ty][tx];
                if (point > best_point) {
                    best_point = point;
                    best_action_index = action;
                }
            }
        }

        character.y_ += dy[best_action_index];
        character.x_ += dx[best_action_index];
    }

    void advance() {
        for (int character_id = 0; character_id < CHARACTER_N; character_id++) {
            movePlayer(character_id);
        } 
        for (auto &character : this->characters_) {
            auto &point = this->points_[character.y_][character.x_];
            this->game_score_ += point;
            point = 0;
        }
        this->turn_++;
    }

    void init() {
        for (auto &character : this->characters_) {
            character.y_ = mt_for_action() % H;
            character.x_ = mt_for_action() % W;
        }
    }

    void transition() {
        auto &character = this->characters_[mt_for_action() % CHARACTER_N];
        character.y_ = mt_for_action() % H;
        character.x_ = mt_for_action() % W;
    }
};