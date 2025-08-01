#pragma once
#include <cstdint>
#include <iostream>
#include <vector>
class Ship {
public:
    Ship() {
        x_ = 0;
        y_ = 0;
        length_ = 0;
        direction_ = false;
    }
    Ship (uint64_t x, uint64_t y, int length, bool direction) {
        x_ = x;
        y_ = y;
        length_ = length;
        hits_.resize(length, false);
        direction_ = direction;
    }
    void Hit(uint64_t x, uint64_t y);
    bool isDead() const;
    void print() const;
    uint64_t GetX() const;
    uint64_t GetY() const;
    int GetLength();
    friend std::ostream& operator<<(std::ostream& stream, const Ship& ship);
private:    
    uint64_t x_;
    uint64_t y_;
    int length_;
    bool direction_;
    std::vector<bool> hits_;
};