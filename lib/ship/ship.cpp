#include "ship.h"

void Ship::Hit(uint64_t x, uint64_t y) {
    if (direction_) {
        hits_[y - y_] = true;
    } else {
        hits_[x - x_] = true;
    }
}

bool Ship::isDead() const {
    for (bool hit: hits_) {
        if (!hit) {
            return false;
        }
    }
    return true;
}

void Ship::print() const {
    std::cout << "Ship at (" << x_ << ", " << y_ << "), length: " << length_
              << ", direction: " << (direction_ ? "Vertical" : "Horizontal") << "\n";
}

std::ostream& operator<<(std::ostream& stream, const Ship& ship) {
    stream << ship.length_ << ' ';
    stream << (ship.direction_ ? 'v' : 'h') << ' ';
    stream << ship.x_ << ' ' << ship.y_;
    return stream;
}

uint64_t Ship::GetX() const {
    return x_;
}

uint64_t Ship::GetY() const {
    return y_;
}

int Ship::GetLength() {
    return length_;
}