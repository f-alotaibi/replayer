#include "platform_win.h"
#include <vector>

#include <windows.h>
#include <shlobj.h>

#include "config.h"

struct screen_info {
    std::string monitor_id; // e.g., "\\?\DISPLAY#GSM5C19#..."
    int width;
    int height;
};

static std::vector<screen_info> getConnectedMonitors() {
    std::vector<screen_info> monitors;

    DISPLAY_DEVICE adapter = {0};
    adapter.cb = sizeof(DISPLAY_DEVICE);
    int adapterIndex = 0;

    while (EnumDisplayDevices(NULL, adapterIndex++, &adapter, 0)) {
        // Only interested in active adapters
        if (!(adapter.StateFlags & DISPLAY_DEVICE_ACTIVE))
            continue;

        DISPLAY_DEVICE monitor = {0};
        monitor.cb = sizeof(DISPLAY_DEVICE);
        int monitorIndex = 0;

        while (EnumDisplayDevices(adapter.DeviceName, monitorIndex++, &monitor, EDD_GET_DEVICE_INTERFACE_NAME)) {
            if (!(monitor.StateFlags & DISPLAY_DEVICE_ACTIVE))
                continue;

            screen_info info;

            // Convert WCHAR to std::string
            int bSize = WideCharToMultiByte(CP_ACP, 0, monitor.DeviceID, -1, NULL, 0, NULL, NULL);
            std::string nString(bSize, '\0');
            WideCharToMultiByte(CP_ACP, 0, monitor.DeviceID, -1, &nString[0], bSize, NULL, NULL);

            info.monitor_id = nString;

            DEVMODE devMode = {};
            devMode.dmSize = sizeof(DEVMODE);
            if (EnumDisplaySettings(adapter.DeviceName, ENUM_CURRENT_SETTINGS, &devMode)) {
                info.width = devMode.dmPelsWidth;
                info.height = devMode.dmPelsHeight;
            } else {
                info.width = 0;
                info.height = 0;
            }

            monitors.push_back(info);
        }
    }

    return monitors;
}

obs_source_t* WindowsReplayPlatform::getVideoCaptureSource() const {
    obs_data_t *monitorOpt = obs_data_create();

    std::vector<screen_info> screens = getConnectedMonitors();
    if (screens.empty()) {
        obs_data_release(monitorOpt);
        return nullptr;
    }
    screen_info display = screens[0];

    obs_data_set_bool(monitorOpt, "capture_cursor", true);
    obs_data_set_string(monitorOpt, "monitor_id", display.monitor_id.c_str());
    
    obs_source_t *screenCapture = obs_source_create("monitor_capture", "Capture", monitorOpt, nullptr);
    if (!screenCapture) {
        return nullptr;
    }
    obs_data_release(monitorOpt);
    return screenCapture;
}

obs_source_t* WindowsReplayPlatform::getAudioOutputSource() const {
    obs_data_t *audioOutOpt = obs_data_create();
    obs_data_set_string(audioOutOpt, "device_id", "default");

    obs_source_t *audioOutSource = obs_source_create("wasapi_output_capture", "AudioOutput", audioOutOpt, nullptr);
    if (!audioOutSource) {
        return nullptr;
    }
    obs_data_release(audioOutOpt);
    return audioOutSource;
}

obs_source_t* WindowsReplayPlatform::getAudioInputSource() const {
    obs_data_t *audioInOpt = obs_data_create();
    obs_data_set_string(audioInOpt, "device_id", "default");

    obs_source_t *audioInSource = obs_source_create("wasapi_input_capture", "AudioInput", audioInOpt, nullptr);
    if (!audioInSource) {
        return nullptr;
    }
    obs_data_release(audioInOpt);
    return audioInSource;
}

obs_video_info WindowsReplayPlatform::getVideoInfo() const {
    obs_video_info ovi = {};

    std::vector<screen_info> screens = getConnectedMonitors();
    if (screens.empty()) {
        return ovi;
    }

    screen_info display = screens[0];
    if (display.width == 0 || display.height == 0) {
        return ovi;
    }

    ReplaySettings *config = ReplaySettings::instance();
    
    ovi.adapter = 0;
    ovi.graphics_module = "libobs-d3d11";
    ovi.base_width = display.width;
    ovi.base_height = display.height;
    ovi.output_width = display.width;
    ovi.output_height = display.height;
    ovi.fps_num = config->m_framerate;
    ovi.fps_den = 1;
    ovi.scale_type = OBS_SCALE_BICUBIC;
    ovi.colorspace = VIDEO_CS_709;
    ovi.output_format = VIDEO_FORMAT_NV12;
    ovi.range = VIDEO_RANGE_PARTIAL;
    ovi.gpu_conversion = true;

    return ovi;
}

std::string WindowsReplayPlatform::getDefaultReplayFolder() const {
    char videosPath[MAX_PATH];
    char replaysPath[MAX_PATH];
    
	SHGetFolderPathA(NULL, CSIDL_MYVIDEO, NULL, SHGFP_TYPE_CURRENT, videosPath);

    snprintf(replaysPath, MAX_PATH, "%s\\Replays", videosPath);
    
    return std::string(replaysPath);
}