#include <iostream>
#include "platform_x11.h"

obs_source_t* X11ReplayPlatform::getVideoCaptureSource() const {
    obs_data_t *monitorOpt = obs_data_create();

    obs_data_set_int(monitorOpt, "screen", 0);
    obs_data_set_bool(monitorOpt, "show_cursor", true);
    obs_data_set_bool(monitorOpt, "advanced", false);
    obs_data_set_int(monitorOpt, "cut_top", 0);
    obs_data_set_int(monitorOpt, "cut_left", 0);
    obs_data_set_int(monitorOpt, "cut_right", 0);
    obs_data_set_int(monitorOpt, "cut_bot", 0);

    obs_source_t *screenCapture = obs_source_create("xshm_input_v2", "Capture", monitorOpt, nullptr);
    if (!screenCapture) {
        return nullptr;
    }
    obs_data_release(monitorOpt);
    return screenCapture;
}

obs_source_t* X11ReplayPlatform::getAudioOutputSource() const {
    obs_data_t *audioOutOpt = obs_data_create();
    obs_data_set_string(audioOutOpt, "device_id", "default");

    obs_source_t *audioOutSource = obs_source_create("pulse_output_capture", "AudioOutput", audioOutOpt, nullptr);
    if (!audioOutSource) {
        return nullptr;
    }
    obs_data_release(audioOutOpt);
    return audioOutSource;
}

obs_source_t* X11ReplayPlatform::getAudioInputSource() const {
    obs_data_t *audioInOpt = obs_data_create();
    obs_data_set_string(audioInOpt, "device_id", "default");

    obs_source_t *audioInSource = obs_source_create("pulse_input_capture", "AudioInput", audioInOpt, nullptr);
    if (!audioInSource) {
        return nullptr;
    }
    obs_data_release(audioInOpt);
    return audioInSource;
}

obs_video_info X11ReplayPlatform::getVideoInfo() const {
    obs_video_info ovi = {};
    ovi.adapter = 0;
    ovi.graphics_module = "libobs-opengl";
    ovi.base_width = 1024;
    ovi.base_height = 768;
    ovi.output_width = 960;
    ovi.output_height = 720;
    ovi.fps_num = 60;
    ovi.fps_den = 1;
    ovi.scale_type = OBS_SCALE_BICUBIC;
    ovi.colorspace = VIDEO_CS_709;
    ovi.output_format = VIDEO_FORMAT_NV12;
    ovi.range = VIDEO_RANGE_PARTIAL;
    ovi.gpu_conversion = true;

    return ovi;
}