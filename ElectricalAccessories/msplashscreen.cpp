#include "msplashscreen.h"

#include <QProgressBar>
#include <QLabel>
#include <QDebug>

mSplashScreen::mSplashScreen(const QPixmap &pixmap) : QSplashScreen(pixmap)
{
    mProgressBar = new QProgressBar(this);
    mProgressBar->setGeometry(0,this->height()-17,this->width(),17);
    mProgressBar->setRange(0, 100);
    mProgressBar->setValue(0);

    mProgressBar->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    mProgressBar->setTextVisible(false);
    mProgressBar->setObjectName("screenProgressBar");

    connect(mProgressBar, &QProgressBar::valueChanged, [this]() {repaint();});//更新进度条显示

    mLabel = new QLabel(this);
    mLabel->setGeometry(1,this->height()-16,this->width()-2,15);
    mLabel->setAlignment(Qt::AlignCenter);
    mLabel->setObjectName("screenLabel");
}

mSplashScreen::~mSplashScreen()
{
}

void mSplashScreen::setProgress(int value)
{
    mProgressBar->setValue(value);
}

void mSplashScreen::setLabelTest(QString str)
{
    mLabel->setText(str);
}
