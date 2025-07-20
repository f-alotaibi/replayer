#pragma once

#include <obs.h>

class Encoder {
    public:
        static obs_encoder_t* createVideoEncoder();
        static obs_encoder_t* createAudioEncoder();
};