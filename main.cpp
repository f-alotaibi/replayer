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
    
    //std::thread input_hook(hook_loop);

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    /*
    
    std::this_thread::sleep_for(std::chrono::seconds(10));
    if (!replay.save()) {
        std::cout << "Could not save replay" << std::endl;
    };
    std::this_thread::sleep_for(std::chrono::seconds(5));
    */
    replay.stop();

    //input_hook.join();

    return 0;
}