#pragma once

#include <obs.h>
#include <uiohook.h>
#include "platform.h"

class Replay {
    public:
        Replay();
        bool init();
        void start();
        bool save();
        void stop();
        static Replay* instance;
    private:
        static void handle_hook_event(uiohook_event *const event);
        static void hook_loop();
        std::thread hook_thread;
        obs_scene_t *m_scene;
        obs_source_t *m_audioSource;
        obs_source_t *m_micSource;
        obs_source_t *m_captureSource;
        obs_encoder_t *m_videoEncoder;
        obs_encoder_t *m_audioEncoder;
        obs_output_t *m_output;
        ReplayPlatform *m_ReplayPlatform;
};