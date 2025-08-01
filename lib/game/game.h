#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "lib/field/field.h"
#include "lib/strategy/strategy.h"
class Game {
public:
    void DoCommand (const std::string& cmd);
private:
    Field field_;
    Strategy* strategy_ = new CustomStrategy;
    void Ping() const;
    void Create(const std::string& mode);
    void Start(Strategy* strategy, Field& field);
    void Stop();
    bool isFinished(Field& field);
    bool isWinner(Field& field);
    bool isLoser(Field& field);
    void SetStrategy(const std::string& cmd, Strategy*& strategy);
    bool is_master_mode_ = false;
    bool is_stopped_ = true;
    bool is_winner_ = false;
    bool is_ordered_ = false;
    bool ships_placed_ = false;
};