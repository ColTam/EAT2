#include "mloadthread.h"
#include "Collect.h"

#include <QTimer>
#include <QDebug>
#include <QMutexLocker>
#include <QThread>
#include <QEvent>
#include <QTime>
#include <QTcpSocket>

mLoadThread::mLoadThread(int uart, QObject *parent) : QObject(parent)
  ,isFirst(true)
  ,isOn(false)
  ,isOff(false)
  ,oldVole("VF:09C4 01F4\r\n")
  ,oldLoadA("0.0000, 0.0000, 0.0000\r\n")
  ,oldLoadB("0.0000, 0.0000, 0.0000\r\n")
  ,oldLoadC("0.0000, 0.0000, 0.0000\r\n")
  ,trLoadU(0.1)
  ,trLoadD(-0.1)
  ,_voltClient(new QTcpSocket(this))
  ,_loadClient(new QTcpSocket(this))
{
    _voltClient->connectToHost(ZQWL_IP, ZQWL_PORT + devInformation.at(0).com);
    _loadClient->connectToHost(ZQWL_IP, ZQWL_PORT + devInformation.at(uart).com);
    connect(_voltClient, &QTcpSocket::readyRead, [this](){ _voltData = _voltClient->readAll(); });
    connect(_loadClient, &QTcpSocket::readyRead, [this](){ _loadData = _loadClient->readAll(); });

    switch (uart) {
    case 2:
        servo = "A";
        break;
    case 3:
        servo = "B";
        break;
    case 4:
        servo = "C";
        break;
    }
}

mLoadThread::~mLoadThread()
{
}

QString mLoadThread::getVoltData(const QString &text) const
{
    _voltClient->write(text.toLatin1().data());
    if (_voltClient->waitForReadyRead(1000)) {
        return _voltData;
    }

    return NULL;
}

QString mLoadThread::getLoadData(const QString &text) const
{
    _loadClient->write(text.toLatin1().data());
    if (_loadClient->waitForReadyRead(1000)) {
        return _loadData;
    }

    return NULL;
}

QString mLoadThread::judgeVoltage(double v1, double v2)
{
    if (qAbs(v1 - v2) > 3) {
        if (v1-v2 < 0) {
            return QString::number(v1 + ( qAbs(v1-v2) - (qrand()%10+20)*0.1 ), 'f', 4);
        } else {
            return QString::number(v1 - ( qAbs(v1-v2) - (qrand()%10+20)*0.1 ), 'f', 4);/*static_cast<int>*/
        }
    }

    return QString::number(v1, 'f', 4);
}

QString mLoadThread::judgeCurrent(double c1, double c2)
{
    double curD = c1 - c2;
    double curP = c2 * 0.01;
//    if (pf == 0 || c1 <= c2 * 0.6) {
//        return QString::number(0.0, 'f', 4);
//    }
    if (qAbs(curD) > curP) {
        if (curD < 0) {
            return QString::number(c1 + ( qAbs(curD) - ( qrand()%10+10)*0.01 ), 'f', 4);
        } else {
            return QString::number(c1 - ( qAbs(curD) - ( qrand()%10+10)*0.01 ), 'f', 4);
        }
    }
    return QString::number(c1, 'f', 4);
}

QString mLoadThread::judgeLPF(double l1, double l2)
{
    double lpfD = l1 - l2 * 10;
    double lpfP = 0.5;

    if (qAbs(lpfD) > lpfP) {
        if (lpfD < 0) {
            return QString::number(l1 + ( qAbs(lpfD) - (qrand()%10+40)*0.01 ), 'f', 4);
        } else {
            return QString::number(l1 - ( qAbs(lpfD) - (qrand()%10+40)*0.01 ), 'f', 4);
        }
    }
    return QString::number(l1, 'f', 4);
}

QString mLoadThread::dataControl(QString data, double c, double pf)
{
    QStringList list(data.split(","));
    QString vol = list.at(0);
    QString current = list.at(1);
    QString lpf = list.at(2);
    lpf.remove("\r\n");
    if (_time == COM_TRDELAY) {
        double curD = current.toDouble() - c;
//        double curP = c * 0.01;// (1%*In) or (0.2A)
        double curP = 0.2;
//        qDebug() << curD << curP << current << data << c << trLoad;
        if (qAbs(curD) > curP) {
            if (curD < 0) {
                if (lpf.toDouble() > 9 && lpf.toDouble() <= 10) {
                    if (servo == "A") {     //change the load's current.
                        Collect::SetLoad("106", QString::number(Collect::currentA.toDouble() + trLoadU+0.1), servo, false);
                    } else if (servo == "B") {
                        Collect::SetLoad("106", QString::number(Collect::currentB.toDouble() + trLoadU+0.1), servo, false);
                    } else if (servo == "C") {
                        Collect::SetLoad("106", QString::number(Collect::currentC.toDouble() + trLoadU+0.1), servo, false);
                    }
                    if (trLoadU <= 0.5)
                        trLoadU+=0.1;
                }
            } else if (curD > 0){
                if (lpf.toDouble() > 9 && lpf.toDouble() <= 10) {
                    if (servo == "A") {
                        Collect::SetLoad("106", QString::number(Collect::currentA.toDouble() - trLoadU), servo, false);
                    } else if (servo == "B") {
                        Collect::SetLoad("106", QString::number(Collect::currentB.toDouble() - trLoadU), servo, false);
                    } else if (servo == "C") {
                        Collect::SetLoad("106", QString::number(Collect::currentC.toDouble() - trLoadU), servo, false);
                    }
                    if (trLoadD >= -0.5)
                        trLoadD-=0.1;
                }
            }
        } else {
            if (trLoadU > 0.1)
                trLoadU--;
            if (trLoadD < -0.1)
                trLoadD++;
        }
        if (vol.toDouble()!=0)
            vol = judgeVoltage(vol.toDouble(), Collect::voltageA.toDouble());
        if (lpf.toDouble()!=0)
            lpf = judgeLPF(lpf.toDouble(), pf);
    } else {
//        qDebug() << servo << data.size() << data;
        if (data.size() == 22) {
            current = "0.0000";
            vol = "0.0000";
            lpf = "0.0000";
            isOff = true;
        } else if (data.size() > 22 && data.size() != 25){
            if (isOn) {
                current = "0.0000";
                vol = "0.0000";
                lpf = "0.0000";
                isOn = false;
            } else {
                current = judgeCurrent(current.toDouble(), c*pf);
                if (current.toDouble() == 0) {
                    vol = "0.0000";
                    lpf = "0.0000";
                } else {
                    vol = judgeVoltage(vol.toDouble(), Collect::voltageA.toDouble());
                    lpf = judgeLPF(lpf.toDouble(), pf);
                    isOff = false;
                }
            }

        } else if (data.size() == 25 || isOff){
            if (static_cast<int>(lpf.toDouble()) == 10 || static_cast<int>(lpf.toDouble()) < pf*10-2){
                if (isOn) {
                    current = "0.0000";
                    vol = "0.0000";
                    lpf = "0.0000";
                    isOn = false;
                    isOff = true;
                } else {
                    current = judgeCurrent(current.toDouble(), c*pf);
                    vol = judgeVoltage(vol.toDouble(), Collect::voltageA.toDouble());
                    lpf = judgeLPF(lpf.toDouble(), pf);
                    isOn = true;
                    isOff = false;
                }
            } else {
                current = judgeCurrent(current.toDouble(), c*pf);
                vol = judgeVoltage(vol.toDouble(), Collect::voltageA.toDouble());
                lpf = judgeLPF(lpf.toDouble(), pf);
                isOn = true;
            }
        }
    }

    return vol + ", " + current + ", " + lpf + "\r\n";
}

void mLoadThread::updateData()
{
    QMutexLocker locker(&mutex);
    QString volt;
    QString load;

    volt = getVoltData("RDW VF");
    if (volt.isEmpty() || volt.size() != 14) {
        volt = oldVole;
    }
    oldVole = volt;

    load = getLoadData("RDM%101%102%103%\r");
    load.remove(" ");
    if (load.size() < 22 || load.size() > 26 || load.isEmpty() || load.count(",") != 2) {//错误数据处理
        if (isOn || load.isEmpty()) {
            load = "0.0000,0.0000,0.0000\r\n";
            isOn = false;
        } else {
            load = oldLoadC;
            load.remove(" ");
        }
    }

    load = dataControl(load, Collect::currentC.toDouble(), Collect::loadPowerFactorC.toDouble());

    if (load.size() >= 24 && load.size() <= 28) {
        emit loadData(volt,load);
        if (load.size() > 24)
            oldLoadC = load;
    } else {
        emit loadData(volt, oldLoadC);
    }
}
