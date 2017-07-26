#include "Collect.h"
#include "zqwl.h"

#include <QThread>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>
#include <QIcon>

QList<UART_t> devInformation;
bool connectSTAS = false;
unsigned int comLoadA           = 2;
unsigned int comLoadB           = 1;
unsigned int comLoadC           = 0;
unsigned int comVolt            = 4;
unsigned int comTR_YOKOGAWAGP10 = 5;
unsigned int comTR_AGILENT34970 = 7;
unsigned int comTest            = 6;

bool Collect::IsTestServoA = false;
bool Collect::IsTestServoB = false;
bool Collect::IsTestServoC = false;

QString Collect::registrNumber = "";
QString Collect::voltageA = "";
QString Collect::currentA = "";
QString Collect::currentB = "";
QString Collect::currentC = "";
QString Collect::loadPowerFactorA = "";
QString Collect::loadPowerFactorB = "";
QString Collect::loadPowerFactorC = "";

Collect::AlarmState Collect::mAlarm = Collect::open;
//Collect::devState Collect::d_s;

ZQWL *Collect::mZQWL = new ZQWL;

Collect::Collect(QObject *parent) : QObject(parent)
{
    if (!mZQWL->connectSocket()){
        QMessageBox msg(QMessageBox::Warning, tr("connect"), tr("connect failed"));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setButtonText(QMessageBox::Ok,tr("Ok"));
        msg.setWindowIcon(QIcon(WINDOW_ICON));

        msg.show();
    }
}

bool Collect::ConnectToSTAS()
{
    QSettings *configIniRead = new QSettings("EATconfig.ini", QSettings::IniFormat);

    QString registrN = configIniRead->value("Registration Code").toString();
    delete configIniRead;

    if (!registrN.isEmpty()) {
        registrNumber = registrN;
    }

    return CollectControl::HardConnet(registrNumber.toLatin1().data());
    //    CollectControl::OnBnClickedButtonHome();
    //    CollectControl::OnBnClickedButtonClearlimit();
}

QString Collect::setLoadLPF275_6(int com_t, const QString &current)
{
    writeCommon(com_t, "SET%110%0%\r");
    if (current == "2.5") {
        writeCommon(com_t, "SET%102%3%\r");
        return "SET%101%30%\r";
    } else if (current == "6") {
        writeCommon(com_t, "SET%102%9%\r");
        return "SET%101%73%\r";
    } else if (current == "20") {
        writeCommon(com_t, "SET%102%25%\r");
        return "SET%101%195%\r";
    }

    return QString("SET%101%%1%\r").arg(current.toDouble()*10+1);
}

QString Collect::setLoadLPF250_8(int com_t, const QString &current)
{
    writeCommon(com_t, "SET%101%0%\r");
    if (current == "1") {
        writeCommon(com_t, "SET%102%2%\r");
        return "SET%101%11%\r";
    } else if (current == "2.5") {
        writeCommon(com_t, "SET%102%6%\r");
        return "SET%101%28%\r";
    } else if (current == "3") {
        writeCommon(com_t, "SET%102%7%\r");
        return "SET%101%34%\r";
    } else if (current == "4") {
        writeCommon(com_t, "SET%102%9%\r");
        return "SET%101%42%\r";
    } else if (current == "6") {
        writeCommon(com_t, "SET%102%15%\r");
        return "SET%101%63%\r";
    } else if (current == "10") {
        writeCommon(com_t, "SET%102%26%\r");
        return "SET%101%101%\r";
    } else if (current == "12") {
        writeCommon(com_t, "SET%102%32%\r");
        return "SET%101%122%\r";
    } else if (current == "13") {
        writeCommon(com_t, "SET%102%36%\r");
        return "SET%101%133%\r";
    } else if (current == "16") {
        writeCommon(com_t, "SET%102%48%\r");
        return "SET%101%164%\r";
    } else if (current == "22") {
        writeCommon(com_t, "SET%102%79%\r");
        return "SET%101%238%\r";
    } else if (current == "25") {
        writeCommon(com_t, "SET%102%109%\r");
        return "SET%101%265%\r";
    } else if (current == "31") {
        writeCommon(com_t, "SET%102%190%\r");
        return "SET%101%354%\r";
    } else if (current == "32") {
        writeCommon(com_t, "SET%102%192%\r");
        return "SET%101%369%\r";
    }
    return QString("SET%101%%1%\r").arg(current.toDouble()*10+1);
}

void Collect::ComponentsShortCut(const QString &servo)
{
    if (servo == "A") {
        CollectControl::SetIoStatus(1, 7, 1);
    } else if (servo == "B") {
        CollectControl::SetIoStatus(2, 1, 1);
    } else if (servo == "C") {
        CollectControl::SetIoStatus(2, 0, 1);
    }
}

void Collect::ComponentsNoShortCut(const QString &servo)
{
    if (servo == "A") {
        CollectControl::SetIoStatus(1, 7, 0);
    } else if (servo == "B") {
        CollectControl::SetIoStatus(2, 1, 0);
    } else if (servo == "C") {
        CollectControl::SetIoStatus(2, 0, 0);
    }
}

void Collect::CutToLN(const QString &servo)
{
    if (servo == "A") {
        CollectControl::SetIoStatus(0, 4, 0);
    } else if (servo == "B") {
        CollectControl::SetIoStatus(0, 5, 0);
    } else if (servo == "C") {
        CollectControl::SetIoStatus(0, 6, 0);
    }
}

void Collect::CutToLE(const QString &servo)
{
    if (servo == "A") {
        CollectControl::SetIoStatus(0, 4, 1);
    } else if (servo == "B") {
        CollectControl::SetIoStatus(0, 5, 1);
    } else if (servo == "C") {
        CollectControl::SetIoStatus(0, 6, 1);
    }
}

void Collect::SetVolt(const QString &volt)
{
    QString _volt = "WRW VF%" + volt.left(3) + "0%500%";
    voltageA = volt;

    writeCommon(devInformation.at(0).com, "WRW P:0");
    writeCommon(devInformation.at(0).com, _volt);
    writeCommon(devInformation.at(0).com, _volt);

}

void Collect::SetLoad(const QString LPFactor, const QString current, const QString servo, bool record)
{
    QString _load;
    QString lpf;
    if (LPFactor == "104") {
        if (servo == "A") {
            _load = setLoadLPF275_6(devInformation.at(2).com, current);
        } else if (servo == "B") {
            _load = setLoadLPF275_6(devInformation.at(3).com, current);
        } else if (servo == "C") {
            _load = setLoadLPF275_6(devInformation.at(4).com, current);
        }
        lpf = "0.6";
    } else if (LPFactor == "108") {
        if (servo == "A") {
            _load = setLoadLPF250_8(devInformation.at(2).com, current);
        } else if (servo == "B") {
            _load = setLoadLPF250_8(devInformation.at(3).com, current);
        } else if (servo == "C") {
            _load = setLoadLPF250_8(devInformation.at(4).com, current);
        }
        lpf = "0.8";
    } else if (LPFactor == "106") {
        if (current.size() < 3) {
            if (current.left(2).toInt() > 16) {
                _load = "SET%106%" + current.left(2) + QString("%1%\r").arg(current.left(2).toInt()-16+3);
            } else {
                _load = "SET%106%" + current.left(2) + "1%\r";
            }
        } else {
            if (current.left(2).toInt() > 16) {
                _load = "SET%106%" + QString::number(current.left(4).toDouble()*10+current.left(2).toInt()-16+3) + "%\r";
            } else {
                _load = "SET%106%" + QString::number(current.left(4).toDouble()*10+1) + "%\r";
            }
        }
        lpf = "1.0";
    } else if (LPFactor == "109") {
        if (current.toInt() == 20) {//In == 16A  //额定电流*1.25
            _load = "SET%106%184%\r";
        } else {                    //In == 10A
            _load = "SET%106%112%\r";
        }
        lpf = "1.0";
    }

    if (servo == "A") {
        if (record) {
            currentA = current.left(4);
            loadPowerFactorA = lpf;
        }
        writeCommon(devInformation.at(2).com, _load);
    } else if (servo == "B") {
        if (record) {
            currentB = current.left(4);
            loadPowerFactorB = lpf;
        }
        writeCommon(devInformation.at(3).com, _load);
    } else if (servo == "C") {
        if (record) {
            currentC = current.left(4);
            loadPowerFactorC = lpf;
        }
        writeCommon(devInformation.at(3).com, _load);
    }
}

void Collect::SetTestNumber(const QString number, const QString servo)
{
    if (servo == "A") {
        QString _number = "WRWD020601" + number;
        writeCommon(devInformation.at(5).com, _number);
    } else if (servo == "B") {
        QString _number = "WRWD030601" + number;
        writeCommon(devInformation.at(6).com, _number);
    } else if (servo == "C") {
        QString _number = "WRWD040601" + number;
        writeCommon(devInformation.at(7).com, _number);
    }
}

void Collect::SetTestCycleNumber(const QString number, const QString servo)
{
    if (servo == "A") {
        QString _number = "WRWD020801" + QString("%1").arg(number.toInt()/2,4,16,QLatin1Char('0')).toUpper();
        writeCommon(devInformation.at(5).com, _number);
    } else if (servo == "B") {
        QString _number = "WRWD030801" + QString("%1").arg(number.toInt()/2,4,16,QLatin1Char('0')).toUpper();
        writeCommon(devInformation.at(6).com, _number);
    } else if (servo == "C") {
        QString _number = "WRWD040801" + QString("%1").arg(number.toInt()/2,4,16,QLatin1Char('0')).toUpper();
        writeCommon(devInformation.at(7).com, _number);
    }
}

void Collect::SetTest(const QString number, const QString rate, const QString position, const QString time, const QString servo)
{
    if (servo == "A") {
        QString _rate1 = rate+"0";
        _rate1 = "WRWD020201" + QString("%1").arg(_rate1.toInt(),4,16,QLatin1Char('0')).toUpper();

        QString _rate2 = rate+"0";
        _rate2 = "WRWD021201" + QString("%1").arg(_rate2.toInt(),4,16,QLatin1Char('0')).toUpper();

        QString _position = position + "000";
        _position = "WRWD020001" + QString("%1").arg(_position.toInt(),4,16,QLatin1Char('0')).toUpper();

        float rate_15 = 800;
        float rate_30 = 400;
        QString _time1, _time2;
        if (time.toInt() == 15) {
            _time1 = "WRWD020401012C";
            _time2 = "WRWD021401" + QString("%1").arg(QString::number(rate_15-300-position.toInt()*200/rate.toInt()).toInt(),4,16,QLatin1Char('0')).toUpper();
        } else if (time.toInt() == 30) {
            _time1 = "WRWD0204010096";
            _time2 = "WRWD021401" + QString("%1").arg(QString::number(rate_30-150-position.toInt()*200/rate.toInt()).toInt(),4,16,QLatin1Char('0')).toUpper();
        }

        writeCommon(devInformation.at(5).com, _rate1);
        writeCommon(devInformation.at(5).com, _rate2);
        writeCommon(devInformation.at(5).com, _position);
        writeCommon(devInformation.at(5).com, _time1);
        writeCommon(devInformation.at(5).com, _time2);

        QString isRun = writeReturn(devInformation.at(5).com, "RDWM011001");
        QString isReady = writeReturn(devInformation.at(5).com, "RDWD020601");

        if (isRun.right(2).toInt(nullptr,16) > 0 && isReady.toInt(nullptr, 16) == 0){
            IsTestServoB = true;
        }
    } else if (servo == "B") {
        QString _rate1 = rate+"0";
        _rate1 = "WRWD030201" + QString("%1").arg(_rate1.toInt(),4,16,QLatin1Char('0')).toUpper();

        QString _rate2 = rate+"0";
        _rate2 = "WRWD031201" + QString("%1").arg(_rate2.toInt(),4,16,QLatin1Char('0')).toUpper();

        QString _position = position + "000";
        _position = "WRWD030001" + QString("%1").arg(_position.toInt(),4,16,QLatin1Char('0')).toUpper();

        float rate_15 = 800;
        float rate_30 = 400;
        QString _time1, _time2;
        if (time.toInt() == 15) {
            _time1 = "WRWD030401012C";
            _time2 = "WRWD031401" + QString("%1").arg(QString::number(rate_15-300-position.toInt()*200/rate.toInt()).toInt(),4,16,QLatin1Char('0')).toUpper();
        } else if (time.toInt() == 30) {
            _time1 = "WRWD0304010096";
            _time2 = "WRWD031401" + QString("%1").arg(QString::number(rate_30-150-position.toInt()*200/rate.toInt()).toInt(),4,16,QLatin1Char('0')).toUpper();
        }

        writeCommon(devInformation.at(6).com, _rate1);
        writeCommon(devInformation.at(6).com, _rate2);
        writeCommon(devInformation.at(6).com, _position);
        writeCommon(devInformation.at(6).com, _time1);
        writeCommon(devInformation.at(6).com, _time2);

        QString isRun = writeReturn(devInformation.at(6).com, "RDWM012001");
        QString isReady = writeReturn(devInformation.at(6).com, "RDWD030601");

        if (isRun.right(2).toInt(nullptr,16) > 0 && isReady.toInt(nullptr, 16) == 0){
            IsTestServoC = true;
        }
    } else if (servo == "C") {
        QString _rate1 = rate+"0";
        _rate1 = "WRWD040201" + QString("%1").arg(_rate1.toInt(),4,16,QLatin1Char('0')).toUpper();

        QString _rate2 = rate+"0";
        _rate2 = "WRWD041201" + QString("%1").arg(_rate2.toInt(),4,16,QLatin1Char('0')).toUpper();

        QString _position = position + "000";
        _position = "WRWD040001" + QString("%1").arg(_position.toInt(),4,16,QLatin1Char('0')).toUpper();

        float rate_15 = 800;
        float rate_30 = 400;
        QString _time1, _time2;
        if (time.toInt() == 15) {
            _time1 = "WRWD040401012C";
            _time2 = "WRWD041401" + QString("%1").arg(QString::number(rate_15-300-position.toInt()*200/rate.toInt()).toInt(),4,16,QLatin1Char('0')).toUpper();
        } else if (time.toInt() == 30) {
            _time1 = "WRWD0404010096";
            _time2 = "WRWD041401" + QString("%1").arg(QString::number(rate_30-150-position.toInt()*200/rate.toInt()).toInt(),4,16,QLatin1Char('0')).toUpper();
        }

        writeCommon(devInformation.at(7).com, _rate1);
        writeCommon(devInformation.at(7).com, _rate2);
        writeCommon(devInformation.at(7).com, _position);
        writeCommon(devInformation.at(7).com, _time1);
        writeCommon(devInformation.at(7).com, _time2);

    }

//    TestComeBackToOrigin(servo);
    SetTestCycleNumber(number, servo);
}

void Collect::PowerStart()
{
    writeCommon(devInformation.at(0).com, "WRW P:1");

}

void Collect::LoadStart(const QString servo)
{
    if (servo == "A") {
        writeCommon(devInformation.at(2).com, "SET%107%1%\r");
    } else if (servo == "B") {
        writeCommon(devInformation.at(3).com, "SET%107%1%\r");
    } else if (servo == "C") {
        writeCommon(devInformation.at(4).com, "SET%107%1%\r");
    }
}

void Collect::TestStart(const QString servo)
{
    if (servo == "A") {
        IsTestServoA = true;
        if (IsTestServoB) {
            writeCommon(devInformation.at(5).com, "WRWM0100010401");
        } else {
            writeCommon(devInformation.at(5).com, "WRWM0100010001");
        }
    } else if (servo == "B") {
        IsTestServoB = true;
        if (IsTestServoC) {
            writeCommon(devInformation.at(6).com, "WRWM0110010401");
        } else {
            writeCommon(devInformation.at(6).com, "WRWM0110010001");
        }
    } else if (servo == "C"){
        IsTestServoC = true;
        writeCommon(devInformation.at(7).com, "WRWM0120010001");
    }
}

void Collect::PowerStop()
{
    writeCommon(devInformation.at(0).com, "WRW P:0");
}

void Collect::LoadStop(const QString servo)
{
    if (servo == "A") {
        writeCommon(devInformation.at(2).com, "SET%107%0%\r");
    } else if (servo == "B") {
        writeCommon(devInformation.at(3).com, "SET%107%0%\r");
    } else if (servo == "C") {
        writeCommon(devInformation.at(4).com, "SET%107%0%\r");
    }
}

void Collect::TestStop(const QString servo)
{
    if (servo == "A") {
        IsTestServoA = false;
        if (IsTestServoB) {
            writeCommon(devInformation.at(5).com, "WRWM0100010400");
        } else {
            writeCommon(devInformation.at(5).com, "WRWM0100010000");
        }
    } else if (servo == "B") {
        IsTestServoB = false;
        if (IsTestServoC) {
            writeCommon(devInformation.at(6).com, "WRWM0110010400");
        } else {
            writeCommon(devInformation.at(6).com, "WRWM0110010000");
        }
    } else if (servo == "C"){
        IsTestServoC = false;
        writeCommon(devInformation.at(7).com, "WRWM0120010000");
    }
}

QString Collect::GetTestNumber(const QString servo)
{
    if (servo == "A") {
        return writeReturn(devInformation.at(5).com, "RDWD020601");
    } else if (servo == "B") {
        return writeReturn(devInformation.at(6).com, "RDWD030601");
    } else if (servo == "C") {
        return writeReturn(devInformation.at(7).com, "RDWD040601");
    }
    return NULL;
}

QString Collect::GetTestPosition(const QString servo)
{
    if (servo == "A") {
        return writeReturn(devInformation.at(5).com, "RDWD003001");
    } else if (servo == "B") {
        return writeReturn(devInformation.at(6).com, "RDWD003201");
    } else if (servo == "C") {
        return writeReturn(devInformation.at(7).com, "RDWD003401");
    }
    return NULL;
}

void Collect::TestComeBackToOrigin(const QString servo)
{
    if (servo == "A") {
        writeCommon(devInformation.at(5).com, "WRWM0050010001");
    } else if (servo == "B") {
        writeCommon(devInformation.at(6).com, "WRWM0052010001");
    } else if (servo == "C") {
        writeCommon(devInformation.at(7).com, "WRWM0054010001");
    }
}

void Collect::TestClearNumber(const QString servo)
{
    if (servo == "A") {
        writeCommon(devInformation.at(5).com, "WRWD0206010000");
    } else if (servo == "B") {
        writeCommon(devInformation.at(6).com, "WRWD0306010000");
    } else if (servo == "C") {
        writeCommon(devInformation.at(7).com, "WRWD0406010000");
    }
}

void Collect::AlarmOpen()
{
    if (!mAlarm) {
        CollectControl::SetIoStatus(0, 7, 1);
        mAlarm = Collect::open;
    }
}

void Collect::AlarmClose()
{
    if (mAlarm) {
        CollectControl::SetIoStatus(0, 7, 0);
        mAlarm = Collect::close;
    }
}

void Collect::TestAlarmClose(QString servo)
{
    if (servo == "A") {
        writeCommon(devInformation.at(5).com, "WRWD0206010000");
    } else if (servo == "B") {
        writeCommon(devInformation.at(6).com, "WRWD0306010000");
    } else if (servo == "C") {
        writeCommon(devInformation.at(7).com, "WRWD0406010000");
    }
}

void Collect::TestDisplayOpen()
{
    CollectControl::SetIoStatus(2, 2, 0);
}

void Collect::TestDisplayClose()
{
    CollectControl::SetIoStatus(2, 2, 1);
}

void Collect::getTemperatureRise(QStringList *list)
{
    int i = 0;
    while (1)
    {
        if (i == list->count()) break;

        QString str = list->at(i);
        if (str.size() == 32) {//N 0101    掳C       +00000374E-01TT //' '-12 'N0101oC'-7 '+00000374E-01'-13 'TT'-2
            ++i;
            continue;
        } else {
            list->removeAt(i);
            continue;
        }
    }
}

void Collect::stateStand()
{
    CollectControl::SetIoStatus(1, 5, 0);
    CollectControl::SetIoStatus(1, 6, 0);
    CollectControl::SetIoStatus(1, 4, 1);
    Collect::AlarmClose();
}

void Collect::stateRunning()
{
    CollectControl::SetIoStatus(1, 4, 0);
    CollectControl::SetIoStatus(1, 6, 0);
    CollectControl::SetIoStatus(1, 5, 1);
    Collect::AlarmClose();
}

void Collect::stateWarning()
{
    CollectControl::SetIoStatus(1, 4, 0);
    CollectControl::SetIoStatus(1, 5, 0);
    CollectControl::SetIoStatus(1, 6, 1);
    Collect::AlarmOpen();
}

void Collect::stateAllStop()
{
    CollectControl::SetIoStatus(1, 4, 0);
    CollectControl::SetIoStatus(1, 5, 0);
    CollectControl::SetIoStatus(1, 6, 0);
    Collect::AlarmClose();
}

void Collect::writeCommon(int comIndex, const QString &text)
{
    if (comIndex < 0 || comIndex > 7)//默认8个客户端
        return;

    mZQWL->setValueCommond(comIndex, text);
}

QString Collect::writeReturn(int comIndex, const QString &text, time_t msec)
{
    if (comIndex < 0 || comIndex > 7)//默认8个客户端
        return NULL;

    return mZQWL->setValueReturn(comIndex, text, msec);
}
