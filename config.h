#pragma once

#include <QSettings>

#include "platform.h"
#ifdef PLATFORM_LINUX
#include "platform_x11.h"
#endif
#ifdef PLATFORM_WINDOWS
#include "platform_win.h"
#endif


class ReplaySettings {
    public:
        ReplaySettings() {
            ReplayPlatform *platform;
            #ifdef PLATFORM_LINUX
            platform = new X11ReplayPlatform();
            #endif
            #ifdef PLATFORM_WINDOWS
            platform = new WindowsReplayPlatform();
            #endif

            this->m_settings = new QSettings("Replayer", "Replayer");
            this->m_settings->beginGroup("General");

            this->m_directory = this->m_settings->value("Directory", QString::fromStdString(platform->getDefaultReplayFolder())).toString();
            this->m_maxTimeSeconds = this->m_settings->value("MaxTimeSeconds", 30).toInt();
            this->m_format = this->m_settings->value("FormatName", "Replay_%CCYY-%MM-%DD_%hh-%mm-%ss").toString();
            this->m_extension = this->m_settings->value("Extension", "mp4").toString();
            this->m_framerate = this->m_settings->value("Framerate", 30).toInt();

            this->m_settings->endGroup();
            this->m_settings->sync();
            Save();
        }

        void Save() {
            this->m_settings->beginGroup("General");

            this->m_settings->setValue("Directory", this->m_directory);
            this->m_settings->setValue("MaxTimeSeconds", this->m_maxTimeSeconds);
            this->m_settings->setValue("FormatName", this->m_format);
            this->m_settings->setValue("Extension", this->m_extension);
            this->m_settings->setValue("Framerate", this->m_framerate);
            
            this->m_settings->endGroup();
        }

        static ReplaySettings* instance() {
            static ReplaySettings* s_instance = new ReplaySettings();
            return s_instance;
        }
        QString m_directory;
        int m_maxTimeSeconds;
        QString m_format;
        QString m_extension;
        int m_framerate;
    private:
        QSettings *m_settings;
};