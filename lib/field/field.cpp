#include "field.h"
void Field::SetWidth(const std::string& width) {
    uint64_t tmp = strtoull(width.c_str(), nullptr, 10);
    if ((width != "0" && tmp == 0) || (tmp == UINT64_MAX && width != std::to_string(UINT64_MAX))) {
        std::cout << "failed\n";
        return;
    }
    width_ = tmp;
    std::cout << "ok\n";\
}

uint64_t Field::GetWidth() const {
    return width_;
}

void Field::SetHeight(const std::string& height) {
    uint64_t tmp = strtoull(height.c_str(), nullptr, 10);
    if ((height != "0" && tmp == 0) || (tmp == UINT64_MAX && height != std::to_string(UINT64_MAX))) {
        std::cout << "failed\n";
        return;
    }
    height_ = tmp;
    std::cout << "ok\n";
}

uint64_t Field::GetHeight() const {
    return height_;
}

void Field::SetCount(int type, const std::string& count) {
    uint64_t tmp = strtoull(count.c_str(), nullptr, 10);
    if ((count != "0" && tmp == 0) || (tmp == UINT64_MAX && count != std::to_string(UINT64_MAX))) {
        std::cout << "failed\n";
        return;
    }
    ship_types_[type - 1] = tmp;
    std::cout << "ok\n";
}

uint64_t Field::GetCount (int type) {
    return ship_types_[type - 1];
}

void Field::Shot(const std::string& x, const std::string& y) {
    uint64_t tmp_x = strtoull(x.c_str(), nullptr, 10);
    uint64_t tmp_y = strtoull(y.c_str(), nullptr, 10);
    uint64_t n = field_.size();
    if (field_.find({tmp_x,tmp_y}) != field_.end()) {
        ship_field_[(field_[{tmp_x, tmp_y}])].Hit(tmp_x,tmp_y);
        if (ship_field_[(field_[{tmp_x, tmp_y}])].isDead()) {
            alive_ships_--;
            field_.erase({tmp_x,tmp_y});
            std::cout << "kill\n";
            return;
        }
        field_.erase({tmp_x,tmp_y});
        std::cout << "hit\n";
        return;
    }
    std::cout << "miss\n";
}

std::vector<uint64_t> Field::GetShipTypes() {
    return ship_types_;
}

bool Field::SetShip(uint64_t x, uint64_t y, int length, bool direction) {
    uint64_t x_start = x > 0 ? x - 1 : 0;
    uint64_t y_start = y > 0 ? y - 1 : 0;
    uint64_t x_end;
    uint64_t y_end;
    if (direction) {
        x_end = x == width_ - 1 ? x : x + 1;
        if (y + length > height_) {
            return false;
        }
        y_end = y + length == height_ ? y + length - 1: y + length;
    } else {
        y_end = y == height_ - 1 ? y : y + 1;
        if (x + length > width_) {
            return false;
        }
        x_end = x + length == width_ ? x + length - 1: x + length;
    }
    for (uint64_t i = x_start; i <= x_end; i++) {
        for (uint64_t j = y_start; j <= y_end; j++) {
            if(field_.find({i,j}) != field_.end())  {
                return false;
            }
        }
    }
    Ship ship(x,y,length,direction);
    for (int i = 0; i < length; i++) {
        if (direction) {
            field_[{x,y + i}] = setted_ships_;
        } else {
            field_[{x + i,y}] = setted_ships_;
        }
    }
    AddShip(ship);
    return true;
}

void Field::SetSettings() {
    ship_types_[0] = 1;
    ship_types_[1] = 0;
    ship_types_[2] = 0;
    ship_types_[3] = 4;
    width_ = 6;
    height_= 6;
}

void Field::SetAliveShips() {
    alive_ships_ = ship_types_[0] + ship_types_[1] + ship_types_[2] + ship_types_[3];
    enemy_alive_ships_ = alive_ships_;
}

uint64_t Field::GetAliveShips() const {
    return alive_ships_;
}

void Field::DecreaseEnemyAliveShips() {
    enemy_alive_ships_--;
}

uint64_t Field::GetEnemyAliveShips() const {
    return enemy_alive_ships_;
}

void Field::SetVectorSize() {
    ship_field_.resize(alive_ships_);
}

void Field::AddShip(Ship& ship) {
    ship_field_[setted_ships_] = ship;
    setted_ships_++;
}

void Field::ClearField() {
    field_.clear();
    ship_field_.clear();
    ship_field_.resize(alive_ships_);
    setted_ships_ = 0;
}

Ship Field::GetShip(uint64_t x, uint64_t y) {
    return ship_field_[field_[{x,y}]];
}

void Field::DumpFile(const std::string& path) {
    std::ofstream file;
    file.open(path);
    if (!file.is_open()) {
        std::cout << "Can't open file\n";
        return;
    }
    file << width_ << ' ' << height_ << "\n";
    for (Ship ship : ship_field_) {
        file << ship << "\n";
    }
    file.close();
    std::cout << "ok\n";
}

void Field::LoadFile(const std::string& path) {
    std::ifstream file;
    file.open(path);
    if (!file.is_open()) {
        std::cout << "Can't open file\n";
        return;
    }
    int length;
    char direction_char;
    bool direction;
    uint64_t x;
    uint64_t y;
    file >> width_ >> height_;
    for (int i = 0; i < 4; i++) {
        ship_types_[i] = 0;
    }
    field_.clear();
    while(!file.eof()) {
        file >> length >> direction_char >> x >> y;
        direction = direction_char == 'v';
        ship_field_.resize(ship_field_.size() + 1);
        SetShip(x,y,length,direction);
        ship_types_[length - 1]++;
    }
    if (ship_field_[ship_field_.size() - 1].GetLength() == 0) {
        ship_types_[length - 1]--;
        ship_field_.resize(ship_field_.size() - 1);
    }
    std::cout << "ok\n";
}
