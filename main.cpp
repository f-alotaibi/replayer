#include <obs.h>
#include <thread>
#include <chrono>
#include <iostream>
#include "replay.h"
#include "hud.h"
#include "overlay.h"
#include "config.h"
#include <uiohook.h>

#include <QApplication>

#ifdef PLATFORM_LINUX
#include "platform_x11.h"
#endif
#ifdef PLATFORM_WINDOWS
#include "platform_win.h"
#endif

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

    ReplayPlatform *platformInstance;
    #ifdef PLATFORM_LINUX
    platformInstance = new X11ReplayPlatform();
    #endif
    #ifdef PLATFORM_WINDOWS
    platformInstance = new WindowsReplayPlatform();
    #endif

    ReplayPlatform::instance(platformInstance);

    // Ensure hud is created b4 thread
    OverlayMenu::instance();
    OverlayHUD::instance();

    std::thread replayThread(lambdaReplay);

    int result = a.exec();    

    replayThread.join();

    return result;
}