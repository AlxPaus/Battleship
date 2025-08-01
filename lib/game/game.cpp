#include "game.h"

void SplitString(std::vector<std::string>& split_cmd, std::string cmd) {
    int n = cmd.size();
    std::string res = "";
    for (int i = 0; i < n; i++) {
        if (cmd[i] != ' ') {
            res += cmd[i];
        } else {
            split_cmd.push_back(res);
            res = "";
        }
    }
    if (res != "") {
        split_cmd.push_back(res);
    }
}

void Game::DoCommand(const std::string& cmd) {
    std::vector<std::string> split_cmd;
    SplitString(split_cmd, cmd);
    if (cmd == "ping") {
        Ping();
    } else if (split_cmd[0] == "create") {
        Create(split_cmd[1]);
        if (is_master_mode_) {
            field_.SetSettings();
        }
    } else if (cmd == "start") {
        Start(strategy_, field_);
    } else if (cmd == "stop") {
        Stop();
    } else if (split_cmd[0] == "set" && split_cmd[1] == "width") {
        field_.SetWidth(split_cmd[2]);
    } else if (split_cmd[0] == "set" && split_cmd[1] == "height") {
        field_.SetHeight(split_cmd[2]);
    } else if (split_cmd[0] == "get" && split_cmd[1] == "width") {
        std::cout << field_.GetWidth() << "\n";
    } else if (split_cmd[0] == "get" && split_cmd[1] == "height") {
        std::cout << field_.GetHeight() << "\n";
    } else if (split_cmd[0] == "set" && split_cmd[1] == "count") {
        field_.SetCount(stoll(split_cmd[2]), split_cmd[3]);
    } else if (split_cmd[0] == "get" && split_cmd[1] == "count") {
        std::cout << field_.GetCount(stoll(split_cmd[2])) << "\n";
    } else if (split_cmd[0] == "set" && split_cmd[1] == "strategy") {
        SetStrategy(split_cmd[2], strategy_);
    } else if (cmd == "shot") {
        std::pair<uint64_t, uint64_t> tmp = strategy_->Shot(field_);
        std::cout << tmp.first << " " << tmp.second << "\n";
    } else if (split_cmd[0] == "shot") {
        field_.Shot(split_cmd[1], split_cmd[2]);
    } else if (cmd == "finished") {
        bool tmp = isFinished(field_);
        std::cout << (tmp ? "yes\n" : "no\n");
    } else if (cmd == "win") {
        bool tmp = isWinner(field_);
        std::cout << (tmp ? "yes\n" : "no\n");
    } else if (cmd == "lose") {
        bool tmp = isLoser(field_);
        std::cout << (tmp ? "yes\n" : "no\n");
    } else if (split_cmd[0] == "dump") {
        field_.DumpFile(split_cmd[1]);
    } else if (split_cmd[0] == "load") {
        field_.LoadFile(split_cmd[1]);
        ships_placed_ = true;
    } else if (split_cmd[0] == "set" && split_cmd[1] == "result") {
        strategy_->SetResult(split_cmd[2], field_);
    }
}

void Game::Ping() const{
    std::cout << "pong\n";
}

void Game::Create(const std::string& mode) {
    if (mode == "master") {
        is_master_mode_ = true;
    } else {
        is_master_mode_ = false;
    }
    std::cout << "ok\n";
}

void Game::Start(Strategy* strategy, Field& field) {
    is_stopped_ = false;
    if (!ships_placed_) {
        field.SetAliveShips();
        field.SetVectorSize();
        strategy->PlaceShips(field);
        ships_placed_ = true;
    }
    std::cout << "ok\n";
}

void Game::Stop() {
    is_stopped_ = true;
    std::cout << "ok\n";
}

bool Game::isFinished(Field& field) {
    if (field.GetAliveShips() == 0) {
        is_winner_ = false;
        return true;
    }
    if (field.GetEnemyAliveShips() == 0) {
        is_winner_ = true;
        return true;
    }
    return false;
}

bool Game::isLoser(Field& field) {
    if (!isFinished(field)) {
        return false;
    }
    if (!is_winner_) {
        return true;
    }
    return false;
}

bool Game::isWinner(Field& field) {
    if (!isFinished(field)) {
        return false;
    }
    if (is_winner_) {
        return true;
    }
    return false;
}

void Game::SetStrategy(const std::string& cmd, Strategy*& strategy) {
    if (cmd == "ordered") {
        if (!is_ordered_) {
            strategy = new OrderedStrategy;
            is_ordered_ = true;
        }
    } else {
        if (is_ordered_) {
            strategy = new CustomStrategy;
            is_ordered_ = false;
        }
    }
    std::cout << "ok\n";
}

