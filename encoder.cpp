#include "encoder.h"

#include <set>
#include <string>
#include "config.h"

using namespace std;

// Parts of this code is taken from https://github.com/clowd/obs-express

void updateVideoEncoder_X264(obs_encoder_t* encoder);
void updateVideoEncoder_NVENC(obs_encoder_t* encoder);
void updateVideoEncoder_QSV11(obs_encoder_t* encoder);
void updateVideoEncoder_AMD(obs_encoder_t* encoder);

obs_encoder_t* Encoder::createVideoEncoder() {
    obs_encoder_t* encoder = nullptr;

    if (ReplaySettings::instance()->m_enableHardwareEncoders) {
        set<string> allEncoders;

        const char *encoderName;
        for (int i = 0; obs_enum_encoder_types(i, &encoderName); i++) {
            allEncoders.insert(encoderName);
        }

        if (allEncoders.find("jim_nvenc") != allEncoders.end()) {
            encoder = obs_video_encoder_create("jim_nvenc", "video_encoder_jim_nvenc", nullptr, nullptr);
            updateVideoEncoder_NVENC(encoder);
        } else if (allEncoders.find("amd_hmf_h264") != allEncoders.end()) {
            encoder = obs_video_encoder_create("amd_hmf_h264", "video_encoder_amd_hmf_h264", nullptr, nullptr);
            updateVideoEncoder_AMD(encoder);
        } else if (allEncoders.find("obs_qsv11") != allEncoders.end()) {
            encoder = obs_video_encoder_create("obs_qsv11", "video_encoder_qsv11", nullptr, nullptr);
            updateVideoEncoder_QSV11(encoder);
        }
    }
    
    if (!encoder) {
        encoder = obs_video_encoder_create("obs_x264", "video_encoder_x264", nullptr, nullptr);
        updateVideoEncoder_X264(encoder);
    }
    return encoder;
}

obs_encoder_t* Encoder::createAudioEncoder() {
    return obs_audio_encoder_create("ffmpeg_aac", "audio_encoder", nullptr, 0, nullptr);
}

void updateVideoEncoder_X264(obs_encoder_t* encoder) {
    obs_data_t* settings = obs_data_create();
    obs_data_set_int(settings, "bitrate", 2500);
    obs_data_set_bool(settings, "use_bufsize", false);
    obs_data_set_string(settings, "rate_control", "CBR");
    obs_data_set_string(settings, "profile", "");
    obs_data_set_string(settings, "preset", "veryfast");
    obs_encoder_update(encoder, settings);
    obs_data_release(settings);
}

void updateVideoEncoder_NVENC(obs_encoder_t* encoder) {
    obs_data_t* settings = obs_data_create();
    obs_data_set_int(settings, "bitrate", 2500);
    obs_data_set_bool(settings, "use_bufsize", false);
    obs_data_set_string(settings, "rate_control", "CBR");
    obs_data_set_string(settings, "profile", "high");
    obs_data_set_string(settings, "preset", "slow");
    obs_encoder_update(encoder, settings);
    obs_data_release(settings);
}

void updateVideoEncoder_QSV11(obs_encoder_t* encoder) {
    obs_data_t* settings = obs_data_create();
    obs_data_set_string(settings, "profile", "high");
    obs_data_set_string(settings, "rate_control", "CQP");
    obs_data_set_int(settings, "qpi", 23);
    obs_data_set_int(settings, "qpp", 23);
    obs_data_set_int(settings, "qpb", 23);
    obs_encoder_update(encoder, settings);
    obs_data_release(settings);
}

void updateVideoEncoder_AMD(obs_encoder_t* encoder) {
    obs_data_t* settings = obs_data_create();
    obs_data_set_int(settings, "Usage", 0);
    obs_data_set_int(settings, "Profile", 100);    
    obs_data_set_int(settings, "RateControlMethod", 0);
    obs_data_set_int(settings, "QP.IFrame", 23);
    obs_data_set_int(settings, "QP.PFrame", 23);
    obs_data_set_int(settings, "QP.BFrame", 23);
    obs_data_set_int(settings, "VBVBuffer", 1);
    obs_data_set_int(settings, "VBVBuffer.Size", 100000);
    obs_data_set_double(settings, "KeyframeInterval", 2.0);
    obs_data_set_int(settings, "BFrame.Pattern", 0);
    obs_encoder_update(encoder, settings);
    obs_data_release(settings);
}