#pragma once
#include <ctime>
#include <vector>
#include "lib/field/field.h"
#include "lib/ship/ship.h"

class Strategy{
public:
    bool DefaultPLaceShips(Field& field);
    virtual bool PlaceShips(Field& field) = 0;
    virtual std::pair<uint64_t, uint64_t> Shot(Field& field) = 0; 
    virtual void SetResult (std::string cmd, Field& field) = 0;
private:
    bool PlaceType(Field& field,int length);
};

class OrderedStrategy : public Strategy {
public:
    bool PlaceShips(Field& field);
    std::pair<uint64_t, uint64_t> Shot(Field& field);
    void SetResult (std::string cmd, Field& field);
private:
    uint64_t last_x_shot_ = UINT64_MAX;
    uint64_t last_y_shot_ = UINT64_MAX;
};

class CustomStrategy : public Strategy {
public:
    bool PlaceShips(Field& field);
    std::pair<uint64_t, uint64_t> Shot(Field& field);
    void SetResult (std::string cmd, Field& field);
private:
    uint64_t last_x_shot_ = 0;
    uint64_t last_y_shot_ = 0;
    char last_direction = 'n';
    bool prev_is_hit = false;
    uint64_t diagonal_num = 0;
    std::vector<uint64_t> enemy_ship_types_ = std::vector<uint64_t>(4,0);
    std::map<std::pair<uint64_t,uint64_t>,bool> enemy_field_;
    int PlaceLeft(Field& field,int max_ship, std::vector<uint64_t>& ship_types);
    int PlaceTop(Field& field,int max_ship, std::vector<uint64_t>& ship_types);
    int PlaceRight(Field& field,int max_ship, std::vector<uint64_t>& ship_types);
    int PlaceBottom(Field& field,int max_ship, std::vector<uint64_t>& ship_types);
    bool PlaceMiddle(Field& field,int max_ship, std::vector<uint64_t>& ship_types);
    void ClearShip(Field& field);
    int CountNewCells(Field& field, uint64_t x, uint64_t y);
};
