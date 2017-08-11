#include "mupdatethread.h"
#include "Collect.h"

#include <QTcpSocket>

mUpdateThread::mUpdateThread(QObject *parent)
    : QObject(parent)
    , _updateClient(new QTcpSocket(this))
{
    connect(_updateClient, &QTcpSocket::readyRead, [this](){ socketData = _updateClient->readAll(); });
}

QString mUpdateThread::getData(const QString &text) const
{
    _updateClient->write(text.toLatin1().data());
    if (_updateClient->waitForReadyRead(2000)) {//等待数据接收
        return socketData;
    }

    return NULL;
}

void mUpdateThread::updateState()
{
    //逐一连接各端口，并发送、接收及储存端口对应串口下位机数据
    _updateClient->connectToHost(ZQWL_IP, ZQWL_PORT + comVolt);
    QString volt = getData("RDW VF");

    devInformation.removeAt(0);
    UART_t voltUart;
    voltUart.data = volt;
    voltUart.com = comVolt;
    devInformation.insert(0,voltUart);
    _updateClient->disconnectFromHost();
//    _updateClient->waitForDisconnected();

    _updateClient->connectToHost(ZQWL_IP, ZQWL_PORT + comTR_YOKOGAWAGP10);
    QString tc = getData("FData,0,0001,0110");

    devInformation.removeAt(1);
    UART_t trUart;
    trUart.data = tc;
    trUart.com = comTR_YOKOGAWAGP10;
    devInformation.insert(1,trUart);
    _updateClient->disconnectFromHost();
//    _updateClient->waitForDisconnected();

    _updateClient->connectToHost(ZQWL_IP, ZQWL_PORT + comLoadA);
    QString loadA = getData("RDM%101%102%\r");

    devInformation.removeAt(2);
    UART_t loadAUart;
    loadAUart.data = loadA;
    loadAUart.com = comLoadA;
    devInformation.insert(2,loadAUart);
    _updateClient->disconnectFromHost();
//    _updateClient->waitForDisconnected();

    _updateClient->connectToHost(ZQWL_IP, ZQWL_PORT + comLoadB);
    QString loadB = getData("RDM%101%102%\r");

    devInformation.removeAt(3);
    UART_t loadBUart;
    loadBUart.data = loadB;
    loadBUart.com = comLoadB;
    devInformation.insert(3,loadBUart);
    _updateClient->disconnectFromHost();
//    _updateClient->waitForDisconnected();

    _updateClient->connectToHost(ZQWL_IP, ZQWL_PORT + comLoadC);
    QString loadC = getData("RDM%101%102%\r");

    devInformation.removeAt(4);
    UART_t loadCUart;
    loadCUart.data = loadC;
    loadCUart.com = comLoadC;
    devInformation.insert(4,loadCUart);
    _updateClient->disconnectFromHost();
//    _updateClient->waitForDisconnected();

    _updateClient->connectToHost(ZQWL_IP, ZQWL_PORT + comTest);
    QString testA = getData("RDWD020603");

    devInformation.removeAt(5);
    UART_t testAUart;
    testAUart.data = testA;
    testAUart.com = comTest;
    devInformation.insert(5,testAUart);

    QString testB = getData("RDWD030603");

    devInformation.removeAt(6);
    UART_t testBUart;
    testBUart.data = testB;
    testBUart.com = comTest;
    devInformation.insert(6,testBUart);

    QString testC = getData("RDWD040603");

    devInformation.removeAt(7);
    UART_t testCUart;
    testCUart.data = testC;
    testCUart.com = comTest;
    devInformation.insert(7,testCUart);
    _updateClient->disconnectFromHost();
//    _updateClient->waitForDisconnected();

    _updateClient->connectToHost(ZQWL_IP, ZQWL_PORT + comTR_AGILENT34970);
    QString tc2 = getData("FData,0,0001,0110");

    devInformation.removeAt(8);
    UART_t tr2Uart;
    tr2Uart.data = tc2;
    tr2Uart.com = comTR_AGILENT34970;
    devInformation.insert(8,tr2Uart);
    _updateClient->disconnectFromHost();
//    _updateClient->waitForDisconnected();
}
