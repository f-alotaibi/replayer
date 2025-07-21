#include "platform_x11.h"
#include <iostream>
#include <vector>
#include <X11/Xlib.h>


struct screen_info {
    int id;
    int width;
    int height;
};

static std::vector<screen_info> getConnectedMonitors() {
    std::vector<screen_info> monitors;

    Display *display;
    Screen *screen;

    display = XOpenDisplay(NULL);

    int count_screens = ScreenCount(display);

    for (int i = 0; i < count_screens; ++i) {
        screen = ScreenOfDisplay(display, i);
        screen_info info;

        info.id = i;
        info.width = screen->width;
        info.height = screen->height;

        monitors.push_back(info);
    }

    XCloseDisplay(display);

    return monitors;
}

obs_source_t* X11ReplayPlatform::getVideoCaptureSource() const {
    obs_data_t *monitorOpt = obs_data_create();

    std::vector<screen_info> screens = getConnectedMonitors();
    if (screens.empty()) {
        obs_data_release(monitorOpt);
        return nullptr;
    }
    screen_info display = screens[0];

    obs_data_set_int(monitorOpt, "screen", display.id);
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

    std::vector<screen_info> screens = getConnectedMonitors();
    if (screens.empty()) {
        return ovi;
    }

    screen_info display = screens[0];
    if (display.width == 0 || display.height == 0) {
        return ovi;
    }

    ovi.adapter = 0;
    ovi.graphics_module = "libobs-opengl";
    ovi.base_width = display.width;
    ovi.base_height = display.height;
    ovi.output_width = display.width;
    ovi.output_height = display.height;
    ovi.fps_num = 30;
    ovi.fps_den = 1;
    ovi.scale_type = OBS_SCALE_BICUBIC;
    ovi.colorspace = VIDEO_CS_709;
    ovi.output_format = VIDEO_FORMAT_NV12;
    ovi.range = VIDEO_RANGE_PARTIAL;
    ovi.gpu_conversion = true;

    return ovi;
}