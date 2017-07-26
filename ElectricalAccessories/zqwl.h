#ifndef ZQWL_H
#define ZQWL_H

#include <QObject>
#include <QVector>

class QTcpSocket;

class ZQWL : public QObject
{
    Q_OBJECT

    int socketPort;

    QTcpSocket *socketCom1;
    QTcpSocket *socketCom2;
    QTcpSocket *socketCom3;
    QTcpSocket *socketCom4;
    QTcpSocket *socketCom5;
    QTcpSocket *socketCom6;
    QTcpSocket *socketCom7;
    QTcpSocket *socketCom8;

    QVector<QTcpSocket *> socketList;

    QStringList socketData;
public:
    explicit ZQWL(QObject *parent = 0);
    ~ZQWL();

    bool connectSocket();
    void setValueCommond(int comIndex, const QString &text);
    QString setValueReturn(int comIndex, const QString &text, time_t msec);
signals:

public slots:
    void readData(int comIndex);

private:
    void initConnect();
};

#endif // ZQWL_H
