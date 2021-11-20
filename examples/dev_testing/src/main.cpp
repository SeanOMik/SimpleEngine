//
// Created by SeanOMik on 7/2/2020.
// Github: https://github.com/SeanOMik
//

#include <simpleengine/game.h>

#include <chrono>
#include <iostream>

int main(int argc, char *argv[]) {
    simpleengine::Game game(1280, 720, "SimpleEngine - Developer Testing", false);

    return game.run();
}