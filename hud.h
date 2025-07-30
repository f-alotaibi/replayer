#pragma once

#include <QWidget>

class OverlayHUD : public QWidget {
    public:
        OverlayHUD();
        static OverlayHUD* instance();
    protected:
        void showEvent(QShowEvent* event) override;
};