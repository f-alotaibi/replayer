#include "overlay.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScreen>
#include <QGroupBox>
#include <QFormLayout>
#include <QPushButton>

#include "config.h"

OverlayMenu::OverlayMenu() {
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);

    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();

    setMinimumSize(screenGeometry.width(), screenGeometry.height());
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto background = new QWidget(this);
    background->setStyleSheet("background-color: rgba(0, 0, 0, 170);");
    background->setGeometry(this->rect());

    QVBoxLayout *mainLayout = new QVBoxLayout(background);
    mainLayout->setContentsMargins(0, height()/6, 0 ,0);

    auto settingsBox = new QGroupBox();
    settingsBox->setStyleSheet("background-color: black; border: 0px; color: white;");
    auto settingsLayout = new QVBoxLayout(settingsBox);

    settingsLayout->setContentsMargins(width()/8, height()/12, width()/8, height()/8);

    auto formLayout = new QFormLayout(settingsBox);

    ReplaySettings *settings = ReplaySettings::instance();

    m_dirEdit = new QLineEdit(settingsBox);
    m_dirEdit->setText(settings->m_directory);

    formLayout->addRow("Directory:", m_dirEdit);

    m_maxTimeSpin = new QSpinBox(settingsBox);
    m_maxTimeSpin->setRange(0, 99999);
    m_maxTimeSpin->setValue(settings->m_maxTimeSeconds);

    formLayout->addRow("Max Time in seconds:", m_maxTimeSpin);

    m_formatCombo = new QComboBox(settingsBox);
    m_formatCombo->addItems({"mp4", "mkv"});
    int settingIndex = m_formatCombo->findText(settings->m_extension, Qt::MatchCaseSensitive);
    if (settingIndex > -1) {
        m_formatCombo->setCurrentIndex(settingIndex);
    }

    formLayout->addRow("Video Format:", m_formatCombo);

    m_filenameEdit = new QLineEdit(settingsBox);
    m_filenameEdit->setText(settings->m_format);

    formLayout->addRow("Filename Format:", m_filenameEdit);

    m_frameSpin = new QSpinBox(settingsBox);
    m_frameSpin->setRange(1, 240);
    m_frameSpin->setValue(settings->m_framerate);

    formLayout->addRow("FPS:", m_frameSpin);

    auto save = new QPushButton("Save");
    connect(save, &QPushButton::clicked, this, [this] {
        ReplaySettings *settings = ReplaySettings::instance();
        settings->m_directory = this->m_dirEdit->text();
        settings->m_maxTimeSeconds = this->m_maxTimeSpin->value();
        settings->m_extension = this->m_formatCombo->currentText();
        settings->m_format = this->m_filenameEdit->text();
        settings->m_framerate = this->m_frameSpin->value();

        settings->Save();
        this->close();
    });

    formLayout->addRow(save);
    

    QLabel *label = new QLabel("Overlay Settings", settingsBox);
    label->setStyleSheet("color: white; font-size: 16px;");

    QLabel *noteLabel = new QLabel("Note: You have to restart the appliaction in order for options to apply", settingsBox);
    noteLabel->setStyleSheet("color: white; font-size: 12px;");

    settingsLayout->addWidget(label);
    settingsLayout->addLayout(formLayout);
    settingsLayout->addWidget(noteLabel);

    auto centerLayout = new QHBoxLayout();
    centerLayout->addStretch();
    centerLayout->addWidget(settingsBox);
    centerLayout->addStretch();

    mainLayout->addLayout(centerLayout);
    mainLayout->addStretch();

    setLayout(mainLayout);
}

OverlayMenu* OverlayMenu::instance() {
    static OverlayMenu* s_instance = new OverlayMenu();
    return s_instance;
}

void OverlayMenu::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        close();
    } else {
        QWidget::keyPressEvent(event);
    }
}

void OverlayMenu::showEvent(QShowEvent* event) {
    activateWindow();
}