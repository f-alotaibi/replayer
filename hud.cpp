#include "hud.h"

#include <QLabel>
#include <QTimer>
#include <QPropertyAnimation>
#include <QVBoxLayout>
#include <QScreen>

OverlayHUD::OverlayHUD() {
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_ShowWithoutActivating);

    const int width = 150, height = 50, marginTop = 80;
    resize(width, height);

    QLabel *label = new QLabel("Captured", this);
    label->setStyleSheet("color: white; font-size: 16px; padding-left: 12px;");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(label);
    layout->setContentsMargins(0, 0, 0 ,0);

    setStyleSheet("background-color: black;");

    QRect screen = QGuiApplication::primaryScreen()->geometry();

    int x = screen.right();
    int y = screen.top() + marginTop;

    move(x, y);
}

void OverlayHUD::showEvent(QShowEvent* event) {
    QPoint startPos = this->pos();
    QPoint endPos(startPos.x() - width(), startPos.y());

    QPropertyAnimation *slideIn = new QPropertyAnimation(this, "pos");
    slideIn->setDuration(400);
    slideIn->setStartValue(startPos);
    slideIn->setEndValue(endPos);
    slideIn->setEasingCurve(QEasingCurve::OutCubic);
    slideIn->start(QAbstractAnimation::DeleteWhenStopped);

    QTimer::singleShot(3000, this, [=]() {
        QPropertyAnimation *slideOut = new QPropertyAnimation(this, "pos");
        slideOut->setDuration(400);
        slideOut->setStartValue(endPos);
        slideOut->setEndValue(startPos);
        slideOut->setEasingCurve(QEasingCurve::InCubic);
        connect(slideOut, &QPropertyAnimation::finished, this, &QWidget::close);
        slideOut->start(QAbstractAnimation::DeleteWhenStopped);
    });

}

OverlayHUD* OverlayHUD::instance() {
    static OverlayHUD* s_instance = new OverlayHUD();
    return s_instance;
}