#include "titlebar.h"
#include <QLabel>
#include <QToolButton>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QDebug>

#ifdef Q_OS_WIN
#pragma comment(lib, "user32.lib")
#include <qt_windows.h>
#endif

TitleBar::TitleBar(QWidget *parent, int flags) :
    QWidget(parent),
    flag(flags),
    mIconLabel(new QLabel(this)),
    mTitleLabel(new QLabel(this)),
    mMinimizeButton(new QToolButton(this)),
    mMaximizeButton(new QToolButton(this)),
    mRestoreButton(new QToolButton(this)),
    mCloseButton(new QToolButton(this))
{
    setFixedHeight(TITLE_HEIGHT);

    mIconLabel->setFixedSize(TITLE_ICON_WIDTH, TITLE_HEIGHT);
    mIconLabel->setScaledContents(true);

    mTitleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    mTitleLabel->setMinimumHeight(TITLE_HEIGHT);

    mMinimizeButton->setFixedSize(TITLE_BUTTON_WIDTH, TITLE_HEIGHT);
    mMaximizeButton->setFixedSize(TITLE_BUTTON_WIDTH, TITLE_HEIGHT);
    mRestoreButton->setFixedSize(TITLE_BUTTON_WIDTH, TITLE_HEIGHT);
    mCloseButton->setFixedSize(TITLE_BUTTON_WIDTH, TITLE_HEIGHT);

    mMinimizeButton->setAutoRaise(true);
    mMaximizeButton->setAutoRaise(true);
    mRestoreButton->setAutoRaise(true);
    mCloseButton->setAutoRaise(true);

    this->setObjectName("titleBar");
    mIconLabel->setObjectName("iconLabel");
    mTitleLabel->setObjectName("titleLabel");
    mMinimizeButton->setObjectName("minimizeButton");
    mMaximizeButton->setObjectName("maximizeButton");
    mRestoreButton->setObjectName("restoreButton");
    mCloseButton->setObjectName("closeButton");

    mMinimizeButton->setToolTip(tr("Minimize"));
    mMaximizeButton->setToolTip(tr("Maximize"));
    mRestoreButton->setToolTip(tr("Restore"));
    mCloseButton->setToolTip(tr("Close"));

    mMinimizeButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Down));
    mMaximizeButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Up));

    QHBoxLayout *hBoxLayout = new QHBoxLayout(this);
    hBoxLayout->addWidget(mIconLabel);
    hBoxLayout->addSpacing(0);
    hBoxLayout->addWidget(mTitleLabel);
    hBoxLayout->addWidget(mMinimizeButton);
    hBoxLayout->addWidget(mMaximizeButton);
    hBoxLayout->addWidget(mRestoreButton);
    hBoxLayout->addWidget(mCloseButton);
    hBoxLayout->setSpacing(0);
    hBoxLayout->setContentsMargins(0, 0, 0, 0);

    if (flags) {
        mRestoreButton->hide();
    } else {
        mMaximizeButton->hide();
        mRestoreButton->hide();
    }

    connect(mMinimizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(mMaximizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(mRestoreButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(mCloseButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));

    this->setLayout(hBoxLayout);
}

TitleBar::~TitleBar()
{
}

void TitleBar::mousePressEvent(QMouseEvent *event)
{
#ifdef Q_OS_WIN
    if (ReleaseCapture()) {
        QWidget *window = this->window();
        if (window->isTopLevel()) SendMessage(HWND(window->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
    }
    event->ignore();
#else
#endif
}

bool TitleBar::eventFilter(QObject *watched, QEvent *event)
{
    switch (event->type()) {
    case QEvent::WindowTitleChange:
    {
        QWidget *widget = qobject_cast<QWidget *>(watched);
        if (widget){
            QFont font(mTitleLabel->font());
            QFontMetrics fm(font);
            mTitleLabel->setText(fm.elidedText(widget->windowTitle(),
                                               Qt::ElideMiddle, widget->width()*7/10));
            mTitleLabel->setToolTip(widget->windowTitle());
            //            mTitleLabel->setText(widget->windowTitle());
            return true;
        }
    }
    case QEvent::WindowIconChange:
    {
        QWidget *widget = qobject_cast<QWidget *>(watched);
        if (widget) {
            QIcon icon = widget->windowIcon();
            mIconLabel->setPixmap(icon.pixmap(mIconLabel->size()));
            return true;
        }
    }
    case QEvent::WindowStateChange:
        return true;
    }
    return QWidget::eventFilter(watched, event);
}

void TitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        QWidget *window = this->window();
        if (!mMaximizeButton->isHidden() && flag) {
            mRestoreButton->show();
            mMaximizeButton->hide();
            window->showMaximized();
            emit maximizeClicked();
        } else if (!mRestoreButton->isHidden() && flag) {
            mMaximizeButton->show();
            mRestoreButton->hide();
            window->showNormal();
            emit restoreClicked();
        }
    }
}

void TitleBar::onClicked()
{
    QToolButton *button = qobject_cast<QToolButton *>(sender());
    QWidget *window = this->window();
    if (window->isTopLevel()) {
        if (button == mMinimizeButton) {
            window->showMinimized();
        } else if (button == mMaximizeButton && flag) {
            mRestoreButton->show();
            mMaximizeButton->hide();
            window->showMaximized();
            emit maximizeClicked();
        } else if (button == mRestoreButton && flag) {
            mMaximizeButton->show();
            mRestoreButton->hide();
            window->showNormal();
            emit restoreClicked();
        } else if (button == mCloseButton) {
            emit closeClicked();
        }
    }
}
