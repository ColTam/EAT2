#include "countdown.h"
#include "titlebar.h"
#include "Collect.h"

#include <QLCDNumber>
#include <QTimer>
#include <QTime>
#include <QDebug>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QIcon>
#include <QMessageBox>
#include <QLabel>

CountDown::CountDown(const QString &object, QWidget *parent) :
    m_object(object),
    QDialog(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);

    titleBar = new TitleBar(this);
    this->installEventFilter(titleBar);

    this->resize(400, 300);
    this->setWindowTitle(tr(" Electrical Accessories Test Automation Program"));
    this->setWindowIcon(QIcon(WINDOW_ICON));
    this->setStyleSheet("#titleLabel {color: rgba(212,212,212, 80%);}");

    QPalette mPalette(palette());
    mPalette.setColor(QPalette::Background, QColor(64,66,68));
    this->setAutoFillBackground(true);
    this->setPalette(mPalette);

    m_label = new QLabel(this);
    m_label->setText(m_object);
    m_label->setMinimumHeight(27);
    m_label->setMaximumHeight(27);
    m_label->setAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
//    m_label->setFont(QFont());
//    m_label->setStyleSheet("");

    _lcdNumber = new QLCDNumber(this);
    _lcdNumber->setDigitCount(5);
    _lcdNumber->setMode(QLCDNumber::Dec);
    _lcdNumber->setSegmentStyle(QLCDNumber::Flat);
    _lcdNumber->setStyleSheet("border: 1px solid white; color: rgb(230,230,230); background: silver;");
    _lcdNumber->setGeometry(1, 54, 398, 245);

    minute = MACHINE_COOL_MINUTE;
    second = MACHINE_COOL_SECOND;
    duration = QTime(0, minute, second);
    _lcdNumber->display(duration.toString("mm:ss"));

    QVBoxLayout *titleVBoxLayout = new QVBoxLayout();
    titleVBoxLayout->addWidget(titleBar);
//    titleVBoxLayout->addStretch(0);
    titleVBoxLayout->setSpacing(0);
    titleVBoxLayout->addWidget(m_label);
//    titleVBoxLayout->addStretch(0);
    titleVBoxLayout->setSpacing(0);
    titleVBoxLayout->addWidget(_lcdNumber);
    titleVBoxLayout->setSpacing(0);
    titleVBoxLayout->setContentsMargins(0, 0, 0, 0);

    connect(titleBar, SIGNAL(closeClicked()), this, SLOT(closeWidget()));

    this->setLayout(titleVBoxLayout);

    _timer = new QTimer();
    _timer->setInterval(1000);
    connect(_timer, SIGNAL(timeout()), this, SLOT(onTimeUpdate()));

    _timer->start();
}

CountDown::~CountDown()
{
    delete _timer;
}

void CountDown::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Escape:
        break;
    default:
        QDialog::keyPressEvent(event);
    }
}

void CountDown::onTimeUpdate()
{
    --second;
    if (-1 == second && 0 == minute) {
        _timer->stop();
        this->close();
    }

    if (second < 0) {
        second=59;
        --minute;
    }

    duration = QTime(0, minute, second);
    _lcdNumber->display(duration.toString("mm:ss"));
}

void CountDown::closeWidget()
{
    if (minute > MACHINE_COOL_MINUTE - 2) {
        QMessageBox msgBox(QMessageBox::Warning,tr("Error"),tr("The Machine Needed Cooling!\nPlease be patient."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setButtonText(QMessageBox::Ok,tr("Ok"));
        msgBox.setWindowIcon(QIcon(WINDOW_ICON));

        msgBox.exec();
        return;
    } else {
        QMessageBox msgBox(QMessageBox::Question,tr("close"),tr("The Machine Needed Cooling!\nWhether or not to continue to leave?"));
        msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
        msgBox.setButtonText(QMessageBox::Ok,tr("Ok"));
        msgBox.setButtonText(QMessageBox::Cancel,tr("Cancel"));
        msgBox.setWindowIcon(QIcon(WINDOW_ICON));

        int ret = msgBox.exec();

        if (ret == QMessageBox::Cancel) {
            return;
        }
    }

    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(618);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();

    connect(animation, SIGNAL(finished()), this, SLOT(close()));
}
