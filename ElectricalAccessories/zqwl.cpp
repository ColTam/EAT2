#include "zqwl.h"
#include "Collect.h"

#include <QTcpSocket>
#include <QDebug>

ZQWL::ZQWL(QObject *parent)
    : QObject(parent)
    , socketPort(1030)
    , socketCom1(new QTcpSocket(this))
    , socketCom2(new QTcpSocket(this))
    , socketCom3(new QTcpSocket(this))
    , socketCom4(new QTcpSocket(this))
    , socketCom5(new QTcpSocket(this))
    , socketCom6(new QTcpSocket(this))
    , socketCom7(new QTcpSocket(this))
    , socketCom8(new QTcpSocket(this))
{
    socketList << socketCom1 << socketCom2 << socketCom3 << socketCom4
               << socketCom5 << socketCom6 << socketCom7 << socketCom8;

    socketData << "" << "" << "" << "" << "" << "" << "" << "";
}

ZQWL::~ZQWL()
{

}

void ZQWL::readData(int comIndex)
{
    QString str = socketList.at(comIndex)->readAll();
    if (!str.isEmpty()) {
        socketData.removeAt(comIndex);
        socketData.insert(comIndex, str);
    }
}

void ZQWL::initConnect()
{
    connect(socketCom1, &QTcpSocket::readyRead, [this](){ readData(1); });
    connect(socketCom2, &QTcpSocket::readyRead, [this](){ readData(2); });
    connect(socketCom3, &QTcpSocket::readyRead, [this](){ readData(3); });
    connect(socketCom4, &QTcpSocket::readyRead, [this](){ readData(4); });
    connect(socketCom5, &QTcpSocket::readyRead, [this](){ readData(5); });
    connect(socketCom6, &QTcpSocket::readyRead, [this](){ readData(6); });
    connect(socketCom7, &QTcpSocket::readyRead, [this](){ readData(7); });
    connect(socketCom8, &QTcpSocket::readyRead, [this](){ readData(8); });
}

bool ZQWL::connectSocket()
{
    bool state = true;
    for (int i=0; i < socketList.size(); i++)
    {
        socketList.at(i)->connectToHost(ZQWL_IP, socketPort+i);
        state &= (socketList.at(i)->waitForConnected(3000));
    }

    if (state)
        initConnect();

    return state;
}

void ZQWL::setValueCommond(int comIndex, const QString &text)
{
    socketList.at(comIndex)->write(text.toLatin1().data());
}

QString ZQWL::setValueReturn(int comIndex, const QString &text, time_t msec)
{
    setValueCommond(comIndex, text);
    if (socketList.at(comIndex)->waitForReadyRead(msec)) {
        return socketData.at(comIndex);
    }

    return NULL;
}
