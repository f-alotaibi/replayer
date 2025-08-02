#pragma once

#include <QWidget>
#include <QKeyEvent>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>

class OverlayMenu : public QWidget {
    public:
        OverlayMenu();
        static OverlayMenu* instance();
    protected:
        void keyPressEvent(QKeyEvent *event) override;
        void showEvent(QShowEvent* event) override;
    private:
        QLineEdit* m_dirEdit;
        QSpinBox* m_maxTimeSpin;
        QComboBox* m_formatCombo;
        QLineEdit* m_filenameEdit;
        QSpinBox* m_frameSpin;
};