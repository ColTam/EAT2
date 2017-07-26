#include "lifetesterdialog.h"
#include "ui_lifetesterdialog.h"
#include "Collect.h"
#include "countdown.h"
#include "mloadthread.h"
#include "qmessagewidget.h"
#include "mtrcrthread.h"

#include <QPropertyAnimation>
#include <QVBoxLayout>
#include <QDebug>
#include <QMouseEvent>
#include <QMessageBox>
#include <QTimer>
#include <QThread>

LifeTesterDialog::LifeTesterDialog(QWidget *parent) :
    QDialog(parent)
    ,ui(new Ui::LifeTesterDialog)
  ,loadCount(0)
  ,judgeCount1(0)
  ,judgeCount2(0)
  ,errorCount(0)
  ,oldPosition("")
{
    ui->setupUi(this);

    initDialogTitle();

    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    ui->stackedWidget->setCurrentIndex(TEST_STACKWIDGET_LOAD);
//    on_toolButton_load_clicked();

    _timer = new QTimer;
    connect(_timer, SIGNAL(timeout()), this, SLOT(isOrigin()));

    _timer->start(3000);

    mMessage = new QMessageWidget(this);
    mMessage->setMinimumSize(718,26);
    mMessage->setGeometry(1,510,718,26);
    mMessage->show();
}

LifeTesterDialog::~LifeTesterDialog()
{
    delete _loadController;
    delete _timer;
    delete ui;
}

void LifeTesterDialog::initDialogTitle()
{
    this->setWindowFlags(Qt::FramelessWindowHint);

    _titleBar = new TitleBar(this);
    this->installEventFilter(_titleBar);

    this->resize(720, 540);
    this->setWindowIcon(QIcon(WINDOW_ICON));

    QPalette mPalette(palette());
    mPalette.setColor(QPalette::Background, QColor(64,66,68));
    this->setAutoFillBackground(true);
    this->setPalette(mPalette);

    QVBoxLayout *titleVBoxLayout = new QVBoxLayout();
    titleVBoxLayout->addWidget(_titleBar);
    titleVBoxLayout->addStretch(0);
    titleVBoxLayout->setSpacing(0);
    titleVBoxLayout->setContentsMargins(0, 0, 0, 0);

    connect(_titleBar, SIGNAL(closeClicked()), this, SLOT(closeDialog()));

    this->setLayout(titleVBoxLayout);
}

void LifeTesterDialog::closeEffect()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(999);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();

    connect(animation, SIGNAL(finished()), this, SLOT(close()));
}

bool LifeTesterDialog::judgement(QString newCurrent)
{
#if 1
    if (50 == ui->lineEdit_O->text().toInt() || 1000 == ui->lineEdit_O->text().toInt()) {
        if (oldCurrent.toDouble() > 0 && newCurrent.toDouble() > 0){
            ++judgeCount1;
            judgeCount2 = 1;
            if (TEST_JUDGEMENT_TIME == judgeCount1) {
                emit testTimerStop();
                emit loadStop();
                if (ui->lineEdit_T->text().toInt() >= ui->lineEdit_O->text().toInt()) {
                    Collect::TestStop(servo);
//                    Collect::PowerStop();
                    Collect::LoadStop(servo);

                    //The Machine Needed Cooling 30min
                    emit testTimerStart();
                    if (50 == ui->lineEdit_O->text().toInt()) {
                        Collect::TestClearNumber(servo);
//                        Collect::TestComeBackToOrigin(servo);
                        CountDown countDown("");
                        countDown.exec();
                        Collect::TestDisplayOpen();
                        return true;
                    } else if (1000 == ui->lineEdit_O->text().toInt()){
                        emit testTimerStop();
                        ui->tableWidget->clear();
                        loadCount = 0;
                        judgeCount1 = 0;
                        Collect::PowerStop();
                        Collect::TestClearNumber(servo);
                        Collect::SetTestCycleNumber(CLAUSE_21_HANDLENUMBER_320_NOTONLOAD, servo);
                        Collect::TestStart(servo);
                        emit testTimerStart();
//                        emit loadStart(TEST_READTIME);
                    }
                } else {
                    emit errorStart(servo);
                    Collect::TestStop(servo);
                    Collect::PowerStop();
                    Collect::LoadStop(servo);
                    QString testNumber = Collect::GetTestNumber(servo);
                    //                    emit testTimerStart();
                    {
                        Collect::TestDisplayOpen();
                        Collect::stateWarning();
                        ui->lineEdit_W->setText(QString::number(++errorCount));
                        QMessageBox msgBox(QMessageBox::Warning,tr(" Electrical Accessories Test Automation Program"),
                                           tr("Servo(%1) Current often connected!\nContinue test on 'OK'!").arg(servo));
                        msgBox.setStandardButtons(QMessageBox::Ok);
                        msgBox.setButtonText(QMessageBox::Ok,tr("Ok"));
                        msgBox.setWindowIcon(QIcon(WINDOW_ICON));

                        msgBox.exec();
                        Collect::TestDisplayClose();
                        Collect::stateRunning();
                    }

                    //                    emit testTimerStop();
                    judgeCount1 = 0;
                    resetTest(servo);
                    Collect::SetTestNumber(testNumber, servo);
                    Collect::PowerStart();
                    Collect::LoadStart(servo);
                    Collect::TestStart(servo);
                    emit testTimerStart();
                    emit loadStart(TEST_READTIME);
                    emit errorStop(servo);
                }
            }
        } else if (0 == oldCurrent.toDouble() && 0 == newCurrent.toDouble()){
            ++judgeCount2;
            judgeCount1 = 1;
            if (TEST_JUDGEMENT_TIME == judgeCount2) {
                emit testTimerStop();
                emit loadStop();
                if (ui->lineEdit_T->text().toInt() >= ui->lineEdit_O->text().toInt()) {
                    Collect::TestStop(servo);
//                    Collect::PowerStop();
                    Collect::LoadStop(servo);
                    emit testTimerStart();

                    //The Machine Needed Cooling 30min
                    if (50 == ui->lineEdit_O->text().toInt()){
                        Collect::TestClearNumber(servo);
//                        Collect::TestComeBackToOrigin(servo);
                        CountDown countDown("");
                        countDown.exec();
                        Collect::TestDisplayOpen();
                        return true;
                    } else if (1000 == ui->lineEdit_O->text().toInt()){
                        emit testTimerStop();
                        ui->tableWidget->clear();
                        loadCount = 0;
                        judgeCount2 = 0;
                        Collect::PowerStop();
                        Collect::TestClearNumber(servo);
                        Collect::SetTestCycleNumber(CLAUSE_21_HANDLENUMBER_320_NOTONLOAD, servo);
                        Collect::TestStart(servo);
                        emit testTimerStart();
//                        emit loadStart(TEST_READTIME);
//                        emit errorStop(servo);
                    }
                } else {
                    emit errorStart(servo);
                    Collect::TestStop(servo);
                    Collect::PowerStop();
                    Collect::LoadStop(servo);
                    QString testNumber = Collect::GetTestNumber(servo);
                    //                    emit testTimerStart();

                    {
                        Collect::TestDisplayOpen();
                        Collect::stateWarning();
                        ui->lineEdit_W->setText(QString::number(++errorCount));
                        QMessageBox msgBox(QMessageBox::Warning,tr(" Electrical Accessories Test Automation Program"),
                                           tr("Servo(%1)Current often borken!\nContinue test on 'OK'!").arg(servo));
                        msgBox.setStandardButtons(QMessageBox::Ok);
                        msgBox.setButtonText(QMessageBox::Ok,tr("Ok"));
                        msgBox.setWindowIcon(QIcon(WINDOW_ICON));

                        msgBox.exec();
                        Collect::TestDisplayClose();
                        Collect::stateRunning();
                    }

                    //                    emit testTimerStop();
                    judgeCount2 = 0;
                    resetTest(servo);
                    Collect::SetTestNumber(testNumber, servo);
                    Collect::PowerStart();
                    Collect::LoadStart(servo);
                    Collect::TestStart(servo);
                    emit testTimerStart();
                    emit loadStart(TEST_READTIME);
                    emit errorStop(servo);
                }
            }
        } else {
            judgeCount1 = 1;
            judgeCount2 = 1;
        }
        oldCurrent = newCurrent;
    } else if (2000 == ui->lineEdit_O->text().toInt() || 3000 == ui->lineEdit_O->text().toInt()) {
        if (ui->lineEdit_T->text().toInt() == ui->lineEdit_O->text().toInt()) {
            emit testTimerStop();
            emit loadStop();
//            Collect::TestStop(servo);
            Collect::TestClearNumber(servo);
//            Collect::TestComeBackToOrigin(servo);
            emit testTimerStart();

            Collect::TestDisplayOpen();
            return true;
        }
    } else if (2250 == ui->lineEdit_O->text().toInt()) {
        if (oldCurrent.toDouble() > 0&& newCurrent.toDouble() > 0){
            ++judgeCount1;
            judgeCount2 = 1;
            if (TEST_JUDGEMENT_TIME == judgeCount1) {
                emit testTimerStop();
                emit loadStop();
                if (ui->lineEdit_T->text().toInt() >= ui->lineEdit_O->text().toInt()) {
                    Collect::TestStop(servo);
//                    Collect::PowerStop();
                    Collect::LoadStop(servo);
//                    Collect::TestComeBackToOrigin(servo);
                    emit testTimerStart();

                    {
                        Collect::TestDisplayOpen();
                        Collect::stateWarning();
                        ui->lineEdit_W->setText(QString::number(++errorCount));
                        QMessageBox msgBox(QMessageBox::Warning,tr(" Electrical Accessories Test Automation Program"),
                                           tr("Please replace the test gauge or plug socket(Servo%1)!\nContinue test on 'OK'!").arg(servo));
                        msgBox.setStandardButtons(QMessageBox::Ok);
                        msgBox.setButtonText(QMessageBox::Ok,tr("Ok"));
                        msgBox.setWindowIcon(QIcon(WINDOW_ICON));

                        msgBox.exec();
                        Collect::TestDisplayClose();
                        Collect::stateRunning();
                    }

                    emit testTimerStop();
                    judgeCount1 = 0;
                    Collect::SetTestCycleNumber(CLAUSE_21_HANDLENUMBER_884_LAST, servo);
//                    Collect::PowerStart();
                    Collect::LoadStart(servo);
                    Collect::TestStart(servo);
                    emit testTimerStart();
                    emit loadStart(TEST_READTIME);
                } else {
                    Collect::TestStop(servo);
                    Collect::PowerStop();
                    Collect::LoadStop(servo);
                    emit errorStart(servo);
                    QString testNumber = Collect::GetTestNumber(servo);
                    //                    emit testTimerStart();

                    {
                        Collect::TestDisplayOpen();
                        Collect::stateWarning();
                        ui->lineEdit_W->setText(QString::number(++errorCount));
                        QMessageBox msgBox(QMessageBox::Warning,tr(" Electrical Accessories Test Automation Program"),
                                           tr("Servo(%1) Current often connected!\nContinue test on 'OK'!").arg(servo));
                        msgBox.setStandardButtons(QMessageBox::Ok);
                        msgBox.setButtonText(QMessageBox::Ok,tr("Ok"));
                        msgBox.setWindowIcon(QIcon(WINDOW_ICON));

                        msgBox.exec();
                        Collect::TestDisplayClose();
                        Collect::stateRunning();
                    }

                    //                    emit testTimerStop();
                    judgeCount1 = 0;
                    resetTest(servo);
                    Collect::SetTestNumber(testNumber, servo);
                    Collect::PowerStart();
                    Collect::LoadStart(servo);
                    Collect::TestStart(servo);
                    emit testTimerStart();
                    emit loadStart(TEST_READTIME);
                    emit errorStop(servo);
                }
            }
        } else if (0 == oldCurrent.toDouble() && 0 == newCurrent.toDouble()){
            ++judgeCount2;
            judgeCount1 = 1;
            if (TEST_JUDGEMENT_TIME == judgeCount2) {
                emit testTimerStop();
                emit loadStop();
                if (ui->lineEdit_T->text().toInt() >= ui->lineEdit_O->text().toInt()) {
                    Collect::TestStop(servo);
//                    Collect::PowerStop();
                    Collect::LoadStop(servo);
//                    Collect::TestComeBackToOrigin(servo);
                    emit testTimerStart();

                    //The Machine Needed Cooling 30min
                    {
                        Collect::TestDisplayOpen();
                        Collect::stateWarning();
                        ui->lineEdit_W->setText(QString::number(++errorCount));
                        QMessageBox msgBox(QMessageBox::Warning,tr(" Electrical Accessories Test Automation Program"),
                                           tr("Please replace the test gauge or plug socket(Servo%1)!\nContinue test on 'OK'!").arg(servo));
                        msgBox.setStandardButtons(QMessageBox::Ok);
                        msgBox.setButtonText(QMessageBox::Ok,tr("Ok"));
                        msgBox.setWindowIcon(QIcon(WINDOW_ICON));

                        msgBox.exec();
                        Collect::TestDisplayClose();
                        Collect::stateRunning();
                    }

                    emit testTimerStop();
                    judgeCount2 = 0;
                    Collect::SetTestCycleNumber(CLAUSE_21_HANDLENUMBER_884_LAST, servo);
//                    Collect::TestStart(servo);
                    Collect::LoadStart(servo);
                    Collect::TestStart(servo);
                    emit testTimerStart();
                    emit loadStart(TEST_READTIME);
                } else {
                    Collect::TestStop(servo);
                    Collect::PowerStop();
                    Collect::LoadStop(servo);
                    emit errorStart(servo);
                    QString testNumber = Collect::GetTestNumber(servo);
                    //                    emit testTimerStart();

                    {
                        Collect::TestDisplayOpen();
                        Collect::stateWarning();
                        ui->lineEdit_W->setText(QString::number(++errorCount));
                        QMessageBox msgBox(QMessageBox::Warning,tr(" Electrical Accessories Test Automation Program"),
                                           tr("Servo(%1)Current often borken!\nContinue test on 'OK'!").arg(servo));
                        msgBox.setStandardButtons(QMessageBox::Ok);
                        msgBox.setButtonText(QMessageBox::Ok,tr("Ok"));
                        msgBox.setWindowIcon(QIcon(WINDOW_ICON));

                        msgBox.exec();
                        Collect::TestDisplayClose();
                        Collect::stateRunning();
                    }

                    //                    emit testTimerStop();
                    judgeCount2 = 0;
                    resetTest(servo);
                    Collect::SetTestNumber(testNumber, servo);
                    Collect::PowerStart();
                    Collect::TestStart(servo);
                    Collect::LoadStart(servo);
                    emit testTimerStart();
                    emit loadStart(TEST_READTIME);
                    emit errorStop(servo);
                }
            }
        } else {
            judgeCount1 = 1;
            judgeCount2 = 1;
        }
        oldCurrent = newCurrent;
    } else if (5000 == ui->lineEdit_O->text().toInt()) {
        if (0 < oldCurrent.toDouble() && 0 < newCurrent.toDouble()){
            ++judgeCount1;
            judgeCount2 = 1;
            if (TEST_JUDGEMENT_TIME == judgeCount1) {
                emit testTimerStop();
                emit loadStop();
                if (ui->lineEdit_T->text().toInt() >= ui->lineEdit_O->text().toInt()) {
                    Collect::TestStop(servo);
//                    Collect::PowerStop();
                    Collect::LoadStop(servo);
                    Collect::TestClearNumber(servo);
                    emit testTimerStart();

                    //The Machine Needed Cooling 30min
                    {
                        CountDown countDown("");
                        countDown.exec();
                    }

                    Collect::TestDisplayOpen();
                    return true;
                } else {
                    Collect::TestStop(servo);
                    //Collect::PowerStop();
                    Collect::LoadStop(servo);
                    emit errorStart(servo);
                    QString testNumber = Collect::GetTestNumber(servo);
                    //                    emit testTimerStart();

                    {
                        Collect::TestDisplayOpen();
                        Collect::stateWarning();
                        ui->lineEdit_W->setText(QString::number(++errorCount));
                        QMessageBox msgBox(QMessageBox::Warning,tr(" Electrical Accessories Test Automation Program"),
                                           tr("Servo(%1) Current often connected!\nContinue test on 'OK'!").arg(servo));
                        msgBox.setStandardButtons(QMessageBox::Ok);
                        msgBox.setButtonText(QMessageBox::Ok,tr("Ok"));
                        msgBox.setWindowIcon(QIcon(WINDOW_ICON));

                        msgBox.exec();
                        Collect::TestDisplayClose();
                        Collect::stateRunning();
                    }

                    //                    emit testTimerStop();
                    judgeCount1 = 0;
                    resetTest(servo);
                    Collect::SetTestNumber(testNumber, servo);
                    //Collect::PowerStart();
                    Collect::TestStart(servo);
                    Collect::LoadStart(servo);
                    emit testTimerStart();
                    emit loadStart(TEST_READTIME);
                    emit errorStop(servo);
                }
            }
        } else if (0 == oldCurrent.toDouble() && 0 == newCurrent.toDouble()){
            ++judgeCount2;
            judgeCount1 = 1;
            if (TEST_JUDGEMENT_TIME == judgeCount2) {
                emit testTimerStop();
                emit loadStop();
                if (ui->lineEdit_T->text().toInt() >= ui->lineEdit_O->text().toInt()) {
                    Collect::TestStop(servo);
                    //Collect::PowerStop();
                    Collect::LoadStop(servo);
                    Collect::TestClearNumber(servo);
                    emit testTimerStart();

                    //The Machine Needed Cooling 30min
                    {
                        CountDown countDown("");
                        countDown.exec();
                    }

                    Collect::TestDisplayOpen();
                    return true;
                } else {
                    Collect::TestStop(servo);
                    //Collect::PowerStop();
                    Collect::LoadStop(servo);
                    emit errorStart(servo);
                    QString testNumber = Collect::GetTestNumber(servo);
                    //                    emit testTimerStart();

                    {
                        Collect::TestDisplayOpen();
                        Collect::stateWarning();
                        ui->lineEdit_W->setText(QString::number(++errorCount));
                        QMessageBox msgBox(QMessageBox::Warning,tr(" Electrical Accessories Test Automation Program"),
                                           tr("Servo(%1)Current often borken!\nContinue test on 'OK'!").arg(servo));
                        msgBox.setStandardButtons(QMessageBox::Ok);
                        msgBox.setButtonText(QMessageBox::Ok,tr("Ok"));
                        msgBox.setWindowIcon(QIcon(WINDOW_ICON));

                        msgBox.exec();
                        Collect::TestDisplayClose();
                        Collect::stateRunning();
                    }

                    //                    emit testTimerStop();
                    judgeCount2 = 0;
                    resetTest(servo);
                    Collect::SetTestNumber(testNumber, servo);
                    //Collect::PowerStart();
                    Collect::LoadStart(servo);
                    Collect::TestStart(servo);
                    emit testTimerStart();
                    emit loadStart(TEST_READTIME);
                    emit errorStop(servo);
                }
            }
        } else {
            judgeCount1 = 1;
            judgeCount2 = 1;
        }
        oldCurrent = newCurrent;
    }
#else


#endif

    return false;
}

void LifeTesterDialog::resetTest(QString servo)
{
    if (servo == "A") {
        Collect::SetLoad(devInformation.at(5).value.Pf, devInformation.at(5).value.In, servo);
        Collect::SetTest(devInformation.at(5).value.Nu, devInformation.at(5).value.Ra, devInformation.at(5).value.Po, devInformation.at(5).value.Ti, servo);
    } else if (servo == "B") {
        Collect::SetLoad(devInformation.at(6).value.Pf, devInformation.at(6).value.In, servo);
        Collect::SetTest(devInformation.at(6).value.Nu, devInformation.at(6).value.Ra, devInformation.at(6).value.Po, devInformation.at(6).value.Ti, servo);
    } else if (servo == "C") {
        Collect::SetLoad(devInformation.at(7).value.Pf, devInformation.at(7).value.In, servo);
        Collect::SetTest(devInformation.at(7).value.Nu, devInformation.at(7).value.Ra, devInformation.at(7).value.Po, devInformation.at(7).value.Ti, servo);
    }
}

void LifeTesterDialog::closeDialog()
{
    QMessageBox msgBox(QMessageBox::Warning,tr("close"),tr("In the test!\nAre you sure break off test?"));
    msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
    msgBox.setButtonText(QMessageBox::Ok,tr("Ok"));
    msgBox.setButtonText(QMessageBox::Cancel,tr("Cancel"));
    msgBox.setWindowIcon(QIcon(WINDOW_ICON));

    int ret = msgBox.exec();

    if (ret == QMessageBox::Cancel) {
        return;
    }

    emit loadStop();

//    Collect::PowerStop();
    Collect::LoadStop(servo);
    Collect::TestStop(servo);
    closeEffect();
}

void LifeTesterDialog::isOrigin()
{
    servo = windowTitle().right(2).mid(0, 1);
    emit testTimerStop();
//    emit tRcrTimerStop();
    QString position;
    position = Collect::GetTestPosition(servo);
    mMessage->setText(tr("The machine is back to the origin. The current position: %1mm").arg(QString::number(position.right(5).toInt(nullptr,16)).toDouble()/1000));

    if (oldPosition == position && position.right(5).toInt(nullptr,16) != 0) {
        Collect::TestComeBackToOrigin(servo);
    } else {
        oldPosition = position;
    }
    if (position.right(5).toInt(nullptr,16) == 0 && position.size() == 5) {
        this->_timer->stop();
//        emit tRcrTimerStart();
        mMessage->setText(tr("The machine has been back to the origin, Please wait while the machine is starting."));
        Collect::TestStart(servo);
        Collect::TestDisplayClose();
        emit testTimerStart();

        int uart = 0;
        if (servo == "A") {
            uart = 2;
        } else if (servo == "B") {
            uart = 3;
        } else if (servo == "C") {
            uart = 4;
        }
        _loadController = new loadController(uart);

        connect(_loadController, &loadController::threadLoadData, this, &LifeTesterDialog::updateLoad);
        connect(this, &LifeTesterDialog::loadStart, _loadController, &loadController::threadLoadTimerStart);
        connect(this, &LifeTesterDialog::loadStop, _loadController, &loadController::threadLoadTimerStop);
    }

    emit this->loadStart(TEST_READTIME);
}

void LifeTesterDialog::updateLoad(QString volt, QString load)
{
    if (!volt.isEmpty()) {
        QString _voltage = QString::number(volt.right(11).left(4).toInt(nullptr, 16)/10) + " V";
        QString _frequency = QString::number(volt.right(6).mid(0,4).toInt(nullptr, 16)/10) + " Hz";

        ui->lineEdit_volt->setText(_voltage);
        ui->lineEdit_frequency->setText(_frequency);
    }

    QStringList load_list;
    if (!load.isEmpty()) {
        load.replace("\r\n", '\0');
        load_list << load.split(", ");

        ui->tableWidget->setRowCount(loadCount);
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());

        QString vol = load_list.at(0);
        vol = QString::number(vol.toDouble());

        QString cur = load_list.at(1);
        cur = QString::number(cur.toDouble());

        QString lpf = load_list.at(2);
        lpf = QString::number(lpf.toDouble()/10,'f',2);

        QTableWidgetItem *Volt      = new QTableWidgetItem(vol+" V");
        QTableWidgetItem *Current   = new QTableWidgetItem(cur+" A");
        QTableWidgetItem *LPF       = new QTableWidgetItem(lpf);

        Volt->setTextAlignment(     Qt::AlignHCenter | Qt::AlignVCenter);
        LPF->setTextAlignment(      Qt::AlignHCenter | Qt::AlignVCenter);
        Current->setTextAlignment(  Qt::AlignHCenter | Qt::AlignVCenter);

        ui->tableWidget->setItem(loadCount,0,Volt);
        ui->tableWidget->setItem(loadCount,1,Current);
        ui->tableWidget->setItem(loadCount,2,LPF);

        ui->tableWidget->scrollToBottom();
    }

    if (judgement(load_list.at(1))) {
        Collect::TestStop(servo);
        Collect::TestAlarmClose(servo);
//        Collect::PowerStart();
        emit testTimerStart();
        closeEffect();
    } else {
        mMessage->setText(tr("Is receiving data..."));
        mMessage->show();
        loadCount++;
    }
}

void LifeTesterDialog::updateTest(const QString &list1, const QString &list2, const QString &list3)
{
    if (servo == "A") {
        QString num = HexTodec(list1.left(4));
        if (!num.isEmpty()){
            ui->lineEdit_T->setText(num);
        }

        QString cycle = HexTodec(list1.mid(8, 4));
        if (!cycle.isEmpty()){
            ui->lineEdit_O->setText(cycle);
        }
    } else if (servo == "B") {
        QString num = HexTodec(list2.left(4));
        if (!num.isEmpty()){
            ui->lineEdit_T->setText(num);
        }

        QString cycle = HexTodec(list2.mid(8, 4));
        if (!cycle.isEmpty()){
            ui->lineEdit_O->setText(cycle);
        }
    } else if (servo == "C") {
        QString num = HexTodec(list3.left(4));
        if (!num.isEmpty()){
            ui->lineEdit_T->setText(num);
        }

        QString cycle = HexTodec(list3.mid(8, 4));
        if (!cycle.isEmpty()){
            ui->lineEdit_O->setText(cycle);
        }
    }
}

void LifeTesterDialog::STOPTEST()
{
    Collect::TestStop(servo);
}

void LifeTesterDialog::voltageChange(QString v)
{
    emit testTimerStop();
    emit loadStop();
    Collect::SetVolt(v);

    Collect::PowerStart();
    emit testTimerStart();
    emit loadStart(TEST_READTIME);
}

void LifeTesterDialog::errorLoadStart()
{
    emit loadStart(TEST_READTIME);
}

void LifeTesterDialog::errorLoadStop()
{
    emit loadStop();
}

void LifeTesterDialog::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    Collect::TestDisplayOpen();

    if (servo == "A")       {
        emit clauseFinishedA(0);
    } else if (servo == "B")  {
        emit clauseFinishedB(0);
    } else if (servo == "C")  {
        emit clauseFinishedC(0);
    }

    this->~LifeTesterDialog();
}

void LifeTesterDialog::on_lineEdit_W_textChanged(const QString &arg1)
{
    if (arg1.toInt() != 0) {
        ui->lineEdit_W->setStyleSheet("color: red");
    }
}
