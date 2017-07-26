#include "qmessagewidget.h"

#include <QLabel>
#include <QStyleOption>
#include <QPainter>
#include <QToolButton>
#include <QHBoxLayout>

QMessageWidget::QMessageWidget(QWidget *parent) : QWidget(parent)
{
    setFixedWidth(MESSAGE_HEIGHT);
    setAutoFillBackground(true);

    mMessageLabel = new QLabel(this);
    pCloseButton = new QToolButton(this);

    QFont ft;
    ft.setPointSize(12);
    mMessageLabel->setFont(ft);

    mMessageLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    mMessageLabel->setFixedHeight(MESSAGE_HEIGHT);
    mMessageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mMessageLabel->setObjectName("tipMessageLabel");

    pCloseButton->setFixedSize(MESSAGE_BUTTON_WIDTH/2, MESSAGE_HEIGHT);
    pCloseButton->setText("x");
    pCloseButton->setAutoRaise(true);
    pCloseButton->setObjectName("closeTipButton");

    pLayout = new QHBoxLayout(this);
    pLayout->addWidget(mMessageLabel);
    pLayout->addWidget(pCloseButton);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0,0,0,0);

    setLayout(pLayout);

    connect(pCloseButton, SIGNAL(clicked(bool)), this, SLOT(hide()));
}

QMessageWidget::~QMessageWidget()
{
}

void QMessageWidget::setText(const QString &text)
{
    mMessageLabel->setText(text);
}
