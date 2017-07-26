#include "mtestthread.h"
#include "Collect.h"

#include <QTimer>
#include <QDebug>
#include <QMutexLocker>
#include <QEvent>

mTestThread::mTestThread(QObject *parent)
    : QObject(parent)
    , _data("")
    , oldNumberA("000000000064\r\n")
    , oldNumberB("000000000064\r\n")
    , oldNumberC("000000000064\r\n")
    , _testClient(new QTcpSocket(this))
{
    _testClient->connectToHost(ZQWL_IP, ZQWL_PORT + devInformation.at(5).com);
    connect(_testClient, &QTcpSocket::readyRead, [this](){ _data = _testClient->readAll(); });
}

mTestThread::~mTestThread()
{
}

QString mTestThread::getData(const QString &text) const
{
    _testClient->write(text.toLatin1().data());
    if (_testClient->waitForReadyRead(1000)) {
        return _data;
    }

    return NULL;
}

void mTestThread::updateTest()
{
    QMutexLocker locker(&mutex);
    QString number;

    number = getData("RDWD020603");
    if (number.size() != 13 || number.isEmpty()) {
        number = oldNumberA;
    }
    oldNumberA = number;
    number = getData("RDWD030603");
    if (number.size() != 13 || number.isEmpty()) {
        number = oldNumberB;
    }
    oldNumberB = number;
    number = getData("RDWD040603");
    if (number.size() != 13 || number.isEmpty()) {
        number = oldNumberC;
    }
    oldNumberC = number;

    emit testData(oldNumberA, oldNumberB, oldNumberC);
}
