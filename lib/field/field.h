#pragma once
#include <climits>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "lib/ship/ship.h"
class Field {
public:
    void SetWidth(const std::string& width);
    uint64_t GetWidth() const;
    void SetHeight(const std::string& height);
    uint64_t GetHeight() const;
    void SetCount(int type, const std::string& count);
    uint64_t GetCount (int type);
    void Shot(const std::string& x, const std::string& y);
    bool SetShip(uint64_t x, uint64_t y, int length, bool direction);
    void SetAliveShips();
    uint64_t GetAliveShips() const;
    std::vector<uint64_t> GetShipTypes();
    void SetSettings();
    void DecreaseEnemyAliveShips();
    uint64_t GetEnemyAliveShips() const;
    void SetVectorSize();
    void AddShip(Ship& ship);
    void ClearField();
    Ship GetShip(uint64_t x, uint64_t y);
    void DumpFile(const std::string& path);
    void LoadFile(const std::string& path);
private:
    std::map<std::pair<uint64_t,uint64_t>,uint64_t> field_;
    std::vector<Ship> ship_field_;
    uint64_t width_ = 0;
    uint64_t height_ = 0 ;
    std::vector<uint64_t> ship_types_ = std::vector<uint64_t>(4,0);
    uint64_t alive_ships_;
    uint64_t enemy_alive_ships_;
    uint64_t setted_ships_ = 0;
};