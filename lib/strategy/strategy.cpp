#include "strategy.h"

bool Strategy::PlaceType(Field& field, int length) {
    uint64_t height = field.GetHeight();
    uint64_t width = field.GetWidth();
    std::vector<uint64_t> ship_types = field.GetShipTypes();
    bool set_last_ship;
    if (width > height) {
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                if (ship_types[length] == 0) {
                    return true;
                }
                set_last_ship = field.SetShip(x, y, length + 1, true);
                if (!set_last_ship) {
                    set_last_ship = field.SetShip(x, y, length + 1, false);
                }
                if (set_last_ship) {
                    ship_types[length]--;
                }
            }
        }
    } else {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (ship_types[length] == 0) {
                    return true;
                }
                set_last_ship = field.SetShip(x, y, length + 1, false);
                if (!set_last_ship) {
                    set_last_ship = field.SetShip(x, y, length + 1, true);
                }
                if (set_last_ship) {
                    ship_types[length]--;
                }
            }
        }
    }
    if (ship_types[length] != 0) {
        return false;
    }
    return true;
}

bool Strategy::DefaultPLaceShips(Field& field) {
    std::vector<uint64_t> ship_types = field.GetShipTypes();
    int max_ship = -1;
    for (int i = 0; i < 4; i++) {
        if (ship_types[i] != 0) {
            max_ship = i;
        }
    }
    bool can_place_ships;
    while (max_ship != -1) {
        can_place_ships = PlaceType(field, max_ship);
        if (!can_place_ships) {
            return false;
        }
        max_ship--;
    }
    return true;
}

bool OrderedStrategy::PlaceShips(Field& field) {
    return DefaultPLaceShips(field);
}

std::pair<uint64_t, uint64_t> OrderedStrategy::Shot(Field& field) {
    if (last_x_shot_ == -1 && last_y_shot_ == -1) {
        last_x_shot_ = 0;
        last_y_shot_ = 0;
        return {last_x_shot_, last_y_shot_};
    }
    if (last_x_shot_ == field.GetWidth() - 1) {
        last_x_shot_ = 0;
        last_y_shot_++;
        if (last_y_shot_ == field.GetWidth()) {
            last_y_shot_ = 0;
        }
        return {last_x_shot_, last_y_shot_};
    }
    last_x_shot_++;
    return {last_x_shot_, last_y_shot_};
}

void OrderedStrategy::SetResult(std::string cmd, Field& field) {
    if (cmd == "kill") {
        field.DecreaseEnemyAliveShips();
    }
    std::cout << "ok\n";
}

int CustomStrategy::PlaceLeft(Field& field, int max_ship, std::vector<uint64_t>& ship_types) {
    if (max_ship == -1) {
        return -1;
    }
    uint64_t height = field.GetHeight();
    bool set_last_ship;
    for (int y = 0; y < height; y++) {
        set_last_ship = field.SetShip(0, y, max_ship + 1, true);
        if (set_last_ship) {
            ship_types[max_ship]--;
        }
        while (max_ship != -1 && ship_types[max_ship] == 0) {
            max_ship--;
        }
    }
    return max_ship;
}

int CustomStrategy::PlaceTop(Field& field, int max_ship, std::vector<uint64_t>& ship_types) {
    if (max_ship == -1) {
        return -1;
    }
    uint64_t width = field.GetWidth();
    uint64_t height = field.GetWidth();
    bool set_last_ship;
    for (int x = 0; x < width; x++) {
        set_last_ship = field.SetShip(x, 0, max_ship + 1, false);
        if (set_last_ship) {
            ship_types[max_ship]--;
        }
        while (max_ship != -1 && ship_types[max_ship] == 0) {
            max_ship--;
        }
        if (max_ship == -1) {
            return true;
        }
    }
    return max_ship;
}

int CustomStrategy::PlaceRight(Field& field, int max_ship, std::vector<uint64_t>& ship_types) {
    if (max_ship == -1) {
        return -1;
    }
    uint64_t width = field.GetWidth();
    uint64_t height = field.GetWidth();
    bool set_last_ship;
    for (int y = 0; y < height; y++) {
        set_last_ship = field.SetShip(width - 1, y, max_ship + 1, true);
        if (set_last_ship) {
            ship_types[max_ship]--;
        }
        while (max_ship != -1 && ship_types[max_ship] == 0) {
            max_ship--;
        }
        if (max_ship == -1) {
            return true;
        }
    }
    return max_ship;
}

int CustomStrategy::PlaceBottom(Field& field, int max_ship, std::vector<uint64_t>& ship_types) {
    if (max_ship == -1) {
        return -1;
    }
    uint64_t width = field.GetWidth();
    uint64_t height = field.GetWidth();
    bool set_last_ship;
    for (int x = 0; x < width; x++) {
        set_last_ship = field.SetShip(x, height - 1, max_ship + 1, false);
        if (set_last_ship) {
            ship_types[max_ship]--;
        }
        while (max_ship != -1 && ship_types[max_ship] == 0) {
            max_ship--;
        }
        if (max_ship == -1) {
            return true;
        }
    }
    return max_ship;
}

bool CustomStrategy::PlaceMiddle(Field& field, int max_ship, std::vector<uint64_t>& ship_types) {
    uint64_t width = field.GetWidth();
    uint64_t height = field.GetHeight();
    bool set_last_ship;
    uint64_t max_area = 18 * ship_types[3] + 15 * ship_types[2] + 12 * ship_types[1] + 9 * ship_types[0];
    if (max_area <= (width - 4) * (height - 4)) {
        srand(time(nullptr));
        for (int x = 0; x < width - 4; x++) {
            for (int y = 0; y < height - 4; y++) {
                int rand_x = rand() % (width - 4) + 2;
                int rand_y = rand() % (height - 4) + 2;
                bool rand_direction = rand_x & 2;
                set_last_ship = field.SetShip(rand_x, rand_y, max_ship + 1, rand_direction);
                if (set_last_ship) {
                    ship_types[max_ship]--;
                }
                while (max_ship != -1 && ship_types[max_ship] == 0) {
                    max_ship--;
                }
                if (max_ship == -1) {
                    return true;
                }
            }
        }
    }
    for (int x = 2; x < width - 2; x++) {
        for (int y = 2; y < height - 2; y++) {
            set_last_ship = field.SetShip(x, y, max_ship + 1, true);
            if (!set_last_ship) {
                set_last_ship = field.SetShip(x, y, max_ship + 1, false);
            }
            if (set_last_ship) {
                ship_types[max_ship]--;
            }
            while (max_ship != -1 && ship_types[max_ship] == 0) {
                max_ship--;
            }
            if (max_ship == -1) {
                return true;
            }
        }
    }
    return false;
}

void CustomStrategy::ClearShip(Field& field) {
    uint64_t width = field.GetWidth();
    uint64_t height = field.GetHeight();
    uint64_t x = last_x_shot_;
    uint64_t y = last_y_shot_;
    uint64_t x_start;
    uint64_t y_start;
    uint64_t x_end;
    uint64_t y_end;
    bool direction = true;
    if (x != 0) {
        if (enemy_field_.find({x - 1, y}) != enemy_field_.end() && enemy_field_[{x - 1, y}] == 1) {
            direction = false;
        }
    }
    if (x != width - 1) {
        if (enemy_field_.find({x + 1, y}) != enemy_field_.end() && enemy_field_[{x + 1, y}] == 1) {
            direction = false;
        }
    }
    if (y != 0) {
        if (enemy_field_.find({x, y - 1}) != enemy_field_.end() && enemy_field_[{x - 1, y}] == 1) {
            direction = true;
        }
    }
    if (y != height - 1) {
        if (enemy_field_.find({x, y + 1}) != enemy_field_.end() && enemy_field_[{x + 1, y}] == 1) {
            direction = true;
        }
    }
    while (enemy_field_.find({x, y}) != enemy_field_.end() && x > 0 && enemy_field_[{x, y}] == 1) {
        x--;
    }
    if (x != 0 || (x == 0 && (enemy_field_.find({x, y}) == enemy_field_.end() || enemy_field_[{x, y}] == 0))) {
        x++;
    }
    while (enemy_field_.find({x, y}) != enemy_field_.end() && y > 0 && enemy_field_[{x, y}] == 1) {
        y--;
    }
    if (y != 0 || (y == 0 && enemy_field_.find({x, y}) == enemy_field_.end())) {
        y++;
    }
    int length = 0;
    if (direction) {
        while (enemy_field_.find({x, y}) != enemy_field_.end() && enemy_field_[{x, y}] == 1) {
            y++;
            length++;
        }
        y -= length;
    } else {
        while (enemy_field_.find({x, y}) != enemy_field_.end() && enemy_field_[{x, y}] == 1) {
            x++;
            length++;
        }
        x -= length;
    }
    x_start = x > 0 ? x - 1 : x;
    y_start = y > 0 ? y - 1 : y;
    if (direction) {
        x_end = x + 1 == width ? x : x + 1;
        y_end = y + length == height ? y + length - 1 : y + length;
    } else {
        y_end = y + 1 == height ? y : y + 1;
        x_end = x + length == width ? x + length - 1: x + length;
    }
    for (uint64_t i = x_start; i <= x_end; i++) {
        for (uint64_t j = y_start; j <= y_end; j++) {
            enemy_field_[{i, j}] = false;
        }
    }
    enemy_ship_types_[length - 1]--;
}

bool CustomStrategy::PlaceShips(Field& field) {
    uint64_t height = field.GetHeight();
    uint64_t width = field.GetWidth();
    std::vector<uint64_t> ship_types = field.GetShipTypes();
    enemy_ship_types_ = ship_types;
    int max_ship = -1;
    for (int i = 0; i < 4; i++) {
        if (ship_types[i] != 0) {
            max_ship = i;
        }
    }
    if (max_ship == -1) {
        return true;
    }
    bool can_place = false;
    if (width < height) {
        max_ship = PlaceLeft(field, max_ship, ship_types);
        max_ship = PlaceBottom(field, max_ship, ship_types);
        max_ship = PlaceRight(field, max_ship, ship_types);
        max_ship = PlaceTop(field, max_ship, ship_types);
        can_place = PlaceMiddle(field, max_ship, ship_types);
    } else {
        max_ship = PlaceTop(field, max_ship, ship_types);
        max_ship = PlaceRight(field, max_ship, ship_types);
        max_ship = PlaceBottom(field, max_ship, ship_types);
        max_ship = PlaceLeft(field, max_ship, ship_types);
        can_place = PlaceMiddle(field, max_ship, ship_types);
    }
    if (can_place) {
        return true;
    } else {
        field.ClearField();
        return DefaultPLaceShips(field);
    }
}

int CustomStrategy::CountNewCells(Field& field, uint64_t x, uint64_t y) {
    uint64_t width = field.GetWidth();
    uint64_t height = field.GetHeight();
    int counter = 0;
    for (uint64_t x_shift = 0; x_shift <= 1; x_shift++) {
        for (uint64_t y_shift = 0; y_shift <= 1; y_shift++) {
            if (x_shift == 0 && y_shift == 0) {
                continue;
            }
            if (x + x_shift >= width & y + y_shift >= height) {
                continue;
            }
            if (enemy_field_.find({x + x_shift, y + y_shift}) == enemy_field_.end()) {
                counter++;
            }
            if (x < x_shift && y < y_shift) {
                continue;
            }
            if (enemy_field_.find({x - x_shift, y - y_shift}) == enemy_field_.end()) {
                counter++;
            }
        }
    }
    return counter;
}

std::pair<uint64_t, uint64_t> CustomStrategy::Shot(Field& field) {
    uint64_t width = field.GetWidth();
    uint64_t height = field.GetHeight();
    while (enemy_field_.find({last_x_shot_, last_y_shot_}) != enemy_field_.end()) {
        if (last_x_shot_ == width - 1) {
            last_x_shot_ = 0;
            last_y_shot_ = last_y_shot_ == field.GetHeight() ? 0 : last_y_shot_ + 1;
        } else {
            last_x_shot_++;
        }
    }
    return {last_x_shot_, last_y_shot_};
}

void CustomStrategy::SetResult(std::string cmd, Field& field) {
    if (cmd == "kill") {
        field.DecreaseEnemyAliveShips();
        enemy_field_[{last_x_shot_, last_y_shot_}] = true;
        ClearShip(field);
        prev_is_hit = false;
    } else if (cmd == "hit") {
        enemy_field_[{last_x_shot_, last_y_shot_}] = true;
        prev_is_hit = true;
    } else {
        enemy_field_[{last_x_shot_, last_y_shot_}] = false;
        prev_is_hit = false;
    }
    std::cout << "ok\n";
}