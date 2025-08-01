#include <string>
#include <iostream>
#include "lib/game/game.h"

int main() {
    Game game;
    while (true) {
        std::string cmd;
        std::getline(std::cin, cmd);
        if (cmd == "exit") {
            return 0;
        } else {
            game.DoCommand(cmd);
        }
    }
}