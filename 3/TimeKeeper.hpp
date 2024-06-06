#include <chrono>

class TimeKeeper {
private:
    std::chrono::high_resolution_clock::time_point start_time_;
    int64_t time_threshold_;

public:
    TimeKeeper(const int64_t &time_threshold):
        start_time_(std::chrono::high_resolution_clock::now()), 
        time_threshold_(time_threshold) {}
    
    bool isTimeOver() const {
        using std::chrono::duration_cast;
        using std::chrono::milliseconds;
        auto diff = std::chrono::high_resolution_clock::now() - this->start_time_;
        return duration_cast<milliseconds>(diff).count() >= time_threshold_;
    }
};