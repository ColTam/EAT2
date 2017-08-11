#include "mtrcrthread.h"
#include "Collect.h"
#include "formdisplay.h"

#include <QMutex>
#include <QTimer>
#include <QThread>
#include <QDebug>
#include <QEvent>
#include <QTcpSocket>

mTRCRThread::mTRCRThread(QObject *parent)
    : QObject(parent)
    ,_data("")
    ,oldTc("")
    ,_trClient(new QTcpSocket(this))
    ,_tr2Client(new QTcpSocket(this))
    ,_trIsConnected(false)
    ,_tr2IsConnected(false)
{
    connect(_trClient, &QTcpSocket::connected, [this](){ _trIsConnected = true; });
    connect(_tr2Client, &QTcpSocket::connected, [this](){ _tr2IsConnected = true; });
    connect(_trClient, &QTcpSocket::disconnected, [this](){ _trIsConnected = false; });
    connect(_tr2Client, &QTcpSocket::disconnected, [this](){ _tr2IsConnected = false; });
    connect(_trClient, &QTcpSocket::readyRead, [this](){ _data = _trClient->readAll(); });
    connect(_tr2Client, &QTcpSocket::readyRead, [this](){ _data = _tr2Client->readAll(); });
}

mTRCRThread::~mTRCRThread()
{
}

QString mTRCRThread::getData(const QString &text) const
{
    _trClient->write(text.toLatin1().data());
    if (_trClient->waitForReadyRead(1000)) {
        return _data;
    }

    return NULL;
}

QString mTRCRThread::getData2(const QString &text) const
{
    _tr2Client->write(text.toLatin1().data());
    if (_tr2Client->waitForReadyRead(1000)) {
        return _data;
    }

    return NULL;
}

void mTRCRThread::updateTRCR()
{
    QMutexLocker locker(&mutex);
    QString tc;
    readList.clear();

    if (FormDisplay::mTRType == FormDisplay::YOKOGAWA_GP10) {
        tc = getData("FData,0,0001,0110");
        if (tc.size() != 723 || oldTc.isEmpty()) {
            tc = oldTc;
        }
        oldTc = tc;
        readList << tc.split("TT");
        Collect::getTemperatureRise(&readList);
    } else {
        tc = getData2("RDWD030603");
        if (tc.size() != 723 || oldTc.isEmpty()) {
            tc = oldTc;
        }
        oldTc = tc;
        readList << tc.split("TT");
        Collect::getTemperatureRise(&readList);
    }
//    emit tRCRData(readList);
}

void mTRCRThread::connectZQWL()
{
    if (!_trIsConnected)
        _trClient->connectToHost(ZQWL_IP, ZQWL_PORT + devInformation.at(1).com);
    if (!_tr2IsConnected)
        _tr2Client->connectToHost(ZQWL_IP, ZQWL_PORT + devInformation.at(8).com);
}

void mTRCRThread::disConnectZQWL()
{
    if (_trIsConnected)
        _trClient->disconnectFromHost();
    if (_tr2IsConnected)
        _tr2Client->disconnectFromHost();

    _trIsConnected = false;
    _tr2IsConnected = false;
}

