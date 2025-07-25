#pragma once
#include <obs.h>

#include <string>

class ReplayPlatform {
    public:
        virtual obs_source_t* getVideoCaptureSource() const = 0;
        virtual obs_source_t* getAudioOutputSource() const = 0;
        virtual obs_source_t* getAudioInputSource() const = 0;
        virtual obs_video_info getVideoInfo() const = 0;
        virtual std::string getDefaultReplayFolder() const = 0;
};