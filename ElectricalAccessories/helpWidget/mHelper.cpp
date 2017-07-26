#include "mHelper.h"

#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPushButton>
#include <QStackedWidget>

#include <QDebug>

mHelper::mHelper(QWidget *parent)
    : QFrame(parent)
    , m_prevBtn(new QPushButton(this))
    , m_nextBtn(new QPushButton(this))
    , m_mainWidget(new QStackedWidget(this))
    , m_stack1(new mHelperWidget1(this))
    , m_stack2(new mHelperWidget2(this))
    , m_stack3(new mHelperWidget3(this))
    , m_stack4(new mHelperWidget4(this))
{
    initForm();
    initWidget();
    initConnect();

    m_prevBtn->setEnabled(false);
    m_mainWidget->setCurrentIndex(0);
}

mHelper::~mHelper()
{

}

void mHelper::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        emit changeStackedWidget(0);
        this->hide();
        m_mainWidget->setCurrentIndex(0);
    }
    QWidget::mousePressEvent(event);
}

void mHelper::initForm()
{
    setFixedSize(960, 633);

    m_prevBtn->setObjectName("helper_prevBtn");
    m_nextBtn->setObjectName("helper_nextBtn");

    m_prevBtn->setText("<");
    m_nextBtn->setText(">");

    m_prevBtn->setToolTip(tr(""));
    m_nextBtn->setToolTip(tr(""));
    m_mainWidget->setToolTip(tr("Right-click to close this window."));

    m_mainWidget->addWidget(m_stack1);
    m_mainWidget->addWidget(m_stack2);
    m_mainWidget->addWidget(m_stack3);
    m_mainWidget->addWidget(m_stack4);

    this->setStyleSheet(QString("QFrame {background: rgba(50,50,70,30%);border-image: url(:/image/image/help);}%1").arg(CHILDQSS));
}

void mHelper::initWidget()
{
    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addStretch();
    topLayout->addWidget(m_prevBtn);
    topLayout->addWidget(m_nextBtn);
    topLayout->addSpacing(10);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(m_mainWidget);

    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0,0,0,0);

    setLayout(mainLayout);
}

void mHelper::initConnect()
{
    connect(m_prevBtn, &QPushButton::clicked, [this]() {
        int ind = m_mainWidget->currentIndex() - 1;
        if (ind)
            m_mainWidget->setCurrentIndex(ind);
        else {
            m_mainWidget->setCurrentIndex(ind);
            m_prevBtn->setEnabled(false);
        }
        m_nextBtn->setEnabled(true);
        updateBackground();
    });
    connect(m_nextBtn, &QPushButton::clicked, [this]() {
        int ind = m_mainWidget->currentIndex() + 1;
        if (ind < m_mainWidget->count()) {
            m_mainWidget->setCurrentIndex(ind);
            if (ind+1 == m_mainWidget->count()) {
                m_nextBtn->setEnabled(false);
            }
        }
        m_prevBtn->setEnabled(true);
        updateBackground();
    });

    connect(m_stack1, &mHelperWidget1::buttonClicked, this, &mHelper::changeStackedWidget);
    connect(m_stack2, &mHelperWidget2::buttonClicked, this, &mHelper::changeStackedWidget);
    connect(m_stack3, &mHelperWidget3::buttonClicked, this, &mHelper::changeStackedWidget);
}

void mHelper::updateBackground()
{
    switch (m_mainWidget->currentIndex()) {
    case 0:
        setStyleSheet(QString("QFrame {background: rgba(50,50,70,30%);border-image: url(:/image/image/help);}%1").arg(CHILDQSS));
        break;
    case 1:
        setStyleSheet(QString("QFrame {background: rgba(50,50,70,30%);border-image: url(:/image/image/help_first);}%1").arg(CHILDQSS));
        break;
    case 2:
        setStyleSheet(QString("QFrame {background: rgba(50,50,70,30%);border-image: url(:/image/image/help_second);}%1").arg(CHILDQSS));
        break;
    case 3:
        setStyleSheet(QString("QFrame {background: rgba(50,50,70,30%);border-image: url(:/image/image/help_third);}%1").arg(CHILDQSS));
        break;
    default:
        break;
    }
    emit changeStackedWidget(0);
}

mHelperWidget1::mHelperWidget1(QWidget *parent)
    : QWidget(parent)
    , m_helpBtn(new QPushButton(this))
    , m_helpBtn1(new QPushButton(this))
    , m_helpBtn2(new QPushButton(this))
    , m_helpBtn3(new QPushButton(this))
    , m_helpBtn4(new QPushButton(this))
    , m_helpBtn5(new QPushButton(this))
    , m_helpBtn6(new QPushButton(this))
    , m_helpBtn7(new QPushButton(this))
{
    m_helpBtn->setGeometry(898, 587, 62, 22);
    m_helpBtn1->setGeometry(290, 363, 91, 51);
    m_helpBtn2->setGeometry(570, 363, 91, 51);
    m_helpBtn3->setGeometry(290, 470, 91, 51);
    m_helpBtn4->setGeometry(570, 470, 91, 51);
    m_helpBtn5->setGeometry(702, 167, 227, 20);
    m_helpBtn6->setGeometry(167, 62, 404, 34);
    m_helpBtn7->setGeometry(167, 195, 371, 34);

//    setStyleSheet("QPushButton {min-height: 20px;}");
    m_helpBtn->setToolTip("help");
    m_helpBtn1->setToolTip("temperature Rise");
    m_helpBtn2->setToolTip("breaking Capacity");
    m_helpBtn3->setToolTip("normal Operation");
    m_helpBtn4->setToolTip("temperature Rise After The Normal Operation");
    m_helpBtn5->setToolTip("dawdler Modul");
    m_helpBtn6->setToolTip("be Running modul");
    m_helpBtn7->setToolTip("be having modul");

    initConnect();
}



void mHelperWidget1::initConnect()
{
    connect(m_helpBtn, &QPushButton::clicked, [this](){emit buttonClicked(0);});
    connect(m_helpBtn1, &QPushButton::clicked, [this](){emit buttonClicked(1);});
    connect(m_helpBtn2, &QPushButton::clicked, [this](){emit buttonClicked(2);});
    connect(m_helpBtn3, &QPushButton::clicked, [this](){emit buttonClicked(3);});
    connect(m_helpBtn4, &QPushButton::clicked, [this](){emit buttonClicked(4);});
    connect(m_helpBtn5, &QPushButton::clicked, [this](){emit buttonClicked(5);});
    connect(m_helpBtn6, &QPushButton::clicked, [this](){emit buttonClicked(0);});
    connect(m_helpBtn7, &QPushButton::clicked, [this](){emit buttonClicked(0);});
}

mHelperWidget2::mHelperWidget2(QWidget *parent)
    : QWidget(parent)
    , m_helpBtn(new QPushButton(this))
    , m_helpBtn1(new QPushButton(this))
    , m_helpBtn2(new QPushButton(this))
    , m_helpBtn3(new QPushButton(this))
    , m_helpBtn4(new QPushButton(this))
    , m_helpBtn5(new QPushButton(this))
    , m_helpBtn6(new QPushButton(this))
    , m_helpBtn7(new QPushButton(this))
    , m_helpBtn8(new QPushButton(this))
{
    m_helpBtn->setGeometry(898, 587, 62, 22);
    m_helpBtn1->setGeometry(290, 363, 91, 51);
    m_helpBtn2->setGeometry(570, 363, 91, 51);
    m_helpBtn3->setGeometry(290, 470, 91, 51);
    m_helpBtn4->setGeometry(570, 470, 91, 51);
    m_helpBtn5->setGeometry(34, 7, 99, 28);
    m_helpBtn6->setGeometry(327, 82, 383, 34);
    m_helpBtn7->setGeometry(137, 320, 369, 34);
    m_helpBtn8->setGeometry(522, 233, 377, 34);

    m_helpBtn->setToolTip("help");
    m_helpBtn1->setToolTip("temperature Rise");
    m_helpBtn2->setToolTip("breaking Capacity");
    m_helpBtn3->setToolTip("normal Operation");
    m_helpBtn4->setToolTip("temperature Rise After The Normal Operation");
    m_helpBtn5->setToolTip("first");
    m_helpBtn6->setToolTip("b");
    m_helpBtn7->setToolTip("c");
    m_helpBtn8->setToolTip("a");

    initConnect();
}



void mHelperWidget2::initConnect()
{
    connect(m_helpBtn, &QPushButton::clicked, [this](){emit buttonClicked(0);});
    connect(m_helpBtn1, &QPushButton::clicked, [this](){emit buttonClicked(1);});
    connect(m_helpBtn2, &QPushButton::clicked, [this](){emit buttonClicked(2);});
    connect(m_helpBtn3, &QPushButton::clicked, [this](){emit buttonClicked(3);});
    connect(m_helpBtn4, &QPushButton::clicked, [this](){emit buttonClicked(4);});
    connect(m_helpBtn5, &QPushButton::clicked, [this](){emit buttonClicked(0);});
    connect(m_helpBtn6, &QPushButton::clicked, [this](){emit buttonClicked(0);});
    connect(m_helpBtn7, &QPushButton::clicked, [this](){emit buttonClicked(0);});
    connect(m_helpBtn8, &QPushButton::clicked, [this](){emit buttonClicked(6);});
}

mHelperWidget3::mHelperWidget3(QWidget *parent)
    : QWidget(parent)
    , m_helpBtn(new QPushButton(this))
    , m_helpBtn1(new QPushButton(this))
    , m_helpBtn2(new QPushButton(this))
    , m_helpBtn3(new QPushButton(this))
    , m_helpBtn4(new QPushButton(this))
    , m_helpBtn5(new QPushButton(this))
    , m_helpBtn6(new QPushButton(this))
    , m_helpBtn7(new QPushButton(this))
    , m_helpBtn8(new QPushButton(this))
    , m_helpBtn9(new QPushButton(this))
    , m_helpBtn10(new QPushButton(this))
{
    m_helpBtn->setGeometry(898, 587, 62, 22);
    m_helpBtn1->setGeometry(563, 201, 31, 38);
    m_helpBtn2->setGeometry(594, 201, 31, 38);
    m_helpBtn3->setGeometry(625, 201, 31, 38);
    m_helpBtn4->setGeometry(656, 201, 31, 38);
    m_helpBtn5->setGeometry(34, 7, 99, 28);
    m_helpBtn6->setGeometry(171, 135, 391, 34);
    m_helpBtn7->setGeometry(175, 201, 388, 38);
    m_helpBtn8->setGeometry(219, 379, 546, 36);
    m_helpBtn9->setGeometry(609, 528, 344, 36);
    m_helpBtn10->setGeometry(701, 166, 230, 25);

    m_helpBtn->setToolTip("help");
    m_helpBtn1->setToolTip("temperature Rise");
    m_helpBtn2->setToolTip("breaking Capacity");
    m_helpBtn3->setToolTip("normal Operation");
    m_helpBtn4->setToolTip("temperature Rise After The Normal Operation");
    m_helpBtn5->setToolTip("second");
    m_helpBtn6->setToolTip("a-1");
    m_helpBtn7->setToolTip("a-2");
    m_helpBtn8->setToolTip("b");
    m_helpBtn9->setToolTip("c");
    m_helpBtn10->setToolTip("dawdler Modul");

    initConnect();
}

void mHelperWidget3::initConnect()
{
    connect(m_helpBtn, &QPushButton::clicked, [this](){emit buttonClicked(0);});
    connect(m_helpBtn1, &QPushButton::clicked, [this](){emit buttonClicked(1);});
    connect(m_helpBtn2, &QPushButton::clicked, [this](){emit buttonClicked(2);});
    connect(m_helpBtn3, &QPushButton::clicked, [this](){emit buttonClicked(3);});
    connect(m_helpBtn4, &QPushButton::clicked, [this](){emit buttonClicked(4);});
    connect(m_helpBtn5, &QPushButton::clicked, [this](){emit buttonClicked(0);});
    connect(m_helpBtn6, &QPushButton::clicked, [this](){emit buttonClicked(0);});
    connect(m_helpBtn7, &QPushButton::clicked, [this](){emit buttonClicked(0);});
    connect(m_helpBtn8, &QPushButton::clicked, [this](){emit buttonClicked(0);});
    connect(m_helpBtn9, &QPushButton::clicked, [this](){emit buttonClicked(0);});
    connect(m_helpBtn10, &QPushButton::clicked, [this](){emit buttonClicked(5);});
}

mHelperWidget4::mHelperWidget4(QWidget *parent)
    : QWidget(parent)
{

}

void mHelperWidget4::initConnect()
{

}
