#pragma once

#include <obs.h>
#include "platform.h"

class WindowsReplayPlatform : public ReplayPlatform {
    public:
        obs_source_t* getVideoCaptureSource() const override;
        obs_source_t* getAudioOutputSource() const override;
        obs_source_t* getAudioInputSource() const override;
        obs_video_info getVideoInfo() const override;
        std::string getDefaultReplayFolder() const override;
        std::vector<Display> getConnectedMonitors() const override;
};