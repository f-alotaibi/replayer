#include <obs.h>
#include <thread>
#include <chrono>
#include <iostream>
#include "replay.h"
#include <uiohook.h>

int main() {
    Replay replay = Replay();
    if (!replay.init()) {
        std::cout << "Could not init replay" << std::endl;
    };
    replay.start();
    
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    replay.stop();

    return 0;
}