#include <obs.h>
#include <thread>
#include <chrono>
#include <iostream>
#include "replay.h"
#include "hud.h"
#include <uiohook.h>

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    auto lambdaReplay = []() {
        Replay replay = Replay();
        if (!replay.init()) {
            std::cout << "Could not init replay" << std::endl;
        }
        replay.start();
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    };

    // Ensure hud is created b4 thread
    OverlayHUD::instance();
    
    std::thread replayThread(lambdaReplay);

    int result = a.exec();    

    replayThread.join();

    return result;
}