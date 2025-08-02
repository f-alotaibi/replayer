#include <iostream>
#include <thread>

#include "encoder.h"
#include "replay.h"
#include "hud.h"
#include "platform.h"
#include "config.h"
#include "overlay.h"

Replay* Replay::instance = nullptr;

Replay::Replay() {
}

void Replay::handle_hook_event(uiohook_event *const event) {
    switch (event->type) {
        case EVENT_KEY_PRESSED:
            if (event->data.keyboard.keycode == VC_F9 && (event->mask & MASK_ALT)) {
                Replay::instance->save();
                QMetaObject::invokeMethod(OverlayHUD::instance(), "show", Qt::QueuedConnection);
            } else if (event->data.keyboard.keycode == VC_Z && (event->mask & MASK_ALT)) {
                QMetaObject::invokeMethod(OverlayMenu::instance(), "show", Qt::QueuedConnection);
            }
            break;
        default:
            break;
    }
}

void Replay::hook_loop() {
    hook_set_dispatch_proc(Replay::handle_hook_event);
    std::cout << "Starting HOOK" << std::endl;
    if (hook_run() != UIOHOOK_SUCCESS) {
        std::cerr << "Failed to start hook!" << std::endl;
    }
}

bool Replay::init() {
    if (obs_initialized()) {
        std::cout << "Started a replay while already initialized!" << std::endl;
        return false;
    }

    if (!obs_startup("en-US", NULL, NULL)) {
        std::cout << "Could not startup obs" << std::endl;
        return false;
    }

    this->m_ReplayPlatform = ReplayPlatform::instance(nullptr);

    obs_video_info ovi = this->m_ReplayPlatform->getVideoInfo();
 
    if (obs_reset_video(&ovi) != OBS_VIDEO_SUCCESS) {
        std::cout << "Video init failed!" << std::endl;
        return false;
    }

    obs_audio_info aai = {};
    aai.samples_per_sec = 48000;
    aai.speakers = SPEAKERS_STEREO;
 
    if (!obs_reset_audio(&aai)) {
        std::cout << "Audio init failed!" << std::endl;
        return false;
    }

    struct obs_module_failure_info mfi;
    obs_load_all_modules2(&mfi);
    obs_log_loaded_modules();
    obs_post_load_modules();
 
    if (!obs_initialized()) {
        std::cout << "Failed to initalize!" << std::endl;
        return false;
    }

    int channel = 0;
    this->m_scene = obs_scene_create("main");
    obs_source_t *sceneSource = obs_scene_get_source(this->m_scene);
    obs_set_output_source(channel++, sceneSource);
 
    // Audio
    this->m_audioSource = this->m_ReplayPlatform->getAudioOutputSource();
    if (this->m_audioSource) {
        obs_set_output_source(channel++, this->m_audioSource);
    } else {
        std::cout << "Failed to init audio output source!" << std::endl;
    }

    // Microphone    
    this->m_micSource = this->m_ReplayPlatform->getAudioInputSource();
    if (this->m_micSource) {
        obs_set_output_source(channel++, this->m_micSource);
    } else {
        std::cout << "Failed to init audio input source!" << std::endl;
    }
    
    // Capture
    this->m_captureSource = this->m_ReplayPlatform->getVideoCaptureSource();
    if (!this->m_captureSource) {
        std::cout << "Failed to create monitor capture source. Is the plugin available?" << std::endl;
        return false;
    }

    obs_sceneitem_t* monitorItem = obs_scene_add(this->m_scene, this->m_captureSource);
    vec2 pos{ 0,0 };
    obs_sceneitem_set_pos(monitorItem, &pos);
 
    this->m_videoEncoder = Encoder::createVideoEncoder();
    if (!this->m_videoEncoder) {
        std::cout << "Failed to create obs_x264 encoder. Is the plugin available?" << std::endl;
        return false;
    }

    this->m_audioEncoder = Encoder::createAudioEncoder();
    if (!this->m_audioEncoder) {
        std::cout << "Failed to create ffmpeg_aac encoder. Is the plugin available?" << std::endl;
        return false;
    }

    obs_data_t *outputOpt = obs_data_create();

    ReplaySettings *config = ReplaySettings::instance();

    obs_data_set_string(outputOpt, "directory", config->m_directory.toStdString().c_str());
    obs_data_set_string(outputOpt, "format", config->m_format.toStdString().c_str());
    obs_data_set_string(outputOpt, "extension", config->m_extension.toStdString().c_str());
    obs_data_set_int(outputOpt, "max_time_sec", config->m_maxTimeSeconds);

    this->m_output = obs_output_create("replay_buffer", "ReplayBuffer", outputOpt, nullptr);
    if (!this->m_output) {
        std::cout << "Failed to create replay_buffer!" << std::endl;
        return false;
    }
 
    obs_encoder_set_video(this->m_videoEncoder, obs_get_video());
    obs_encoder_set_audio(this->m_audioEncoder, obs_get_audio());
    obs_output_set_video_encoder(this->m_output, this->m_videoEncoder);
    obs_output_set_audio_encoder(this->m_output, this->m_audioEncoder, 0);

    return true;
}

void Replay::start() {
    if (!obs_initialized()) {
        std::cout << "Started a replay while not init!" << std::endl;
        return;
    }

    if (!obs_output_start(this->m_output)) {
        std::cout << "Failed to start output!" << std::endl;
        return;
    }
    Replay::instance = this;

    this->hook_thread = std::thread(Replay::hook_loop);
}

bool Replay::save() {
    if (!obs_initialized()) {
        std::cout << "Started a replay while not init!" << std::endl;
        return false;
    }

    calldata_t cd = {0};
    proc_handler_t *proc = obs_output_get_proc_handler(this->m_output);
    if (!proc) {
        std::cout << "Failed to get proc handler of output" << std::endl;
        return false;
    }
    proc_handler_call(proc, "save", &cd);
    calldata_free(&cd);
    return true;
}

void Replay::stop() {
    if (!obs_initialized()) {
        std::cout << "Started a replay while not init!" << std::endl;
        return;
    }

    obs_output_stop(this->m_output);
    obs_output_release(this->m_output);
    hook_stop();
    obs_shutdown();
}