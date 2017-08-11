#ifndef MTRCRTHREAD_H
#define MTRCRTHREAD_H

#include <QMutex>
#include <QObject>
#include <QThread>
#include <QTimer>

#ifdef _MSC_BUILD
#pragma execution_character_set("utf-8")
#endif

class QTcpSocket;

/*!
 * \brief The temperature rise thread class
 */
class mTRCRThread : public QObject
{
    Q_OBJECT
    QMutex mutex;
public:
    explicit mTRCRThread(QObject *parent = 0);
    ~mTRCRThread();

    QString getData(const QString &text) const;
    QString getData2(const QString &text) const;

signals:
    void tRCRData(const QStringList &); /*!* \brief 温升数据更新信号 */

public slots:
    void updateTRCR();           /*!* \brief 接收无纸记录仪数据槽函数 */
    void connectZQWL();
    void disConnectZQWL();

private:
    QString _data;
    QString oldTc;              /*!* \brief 旧温升数据 */
    QStringList readList;       /*!* \brief 数据list */

    QTcpSocket *_trClient;
    QTcpSocket *_tr2Client;

    bool _trIsConnected;
    bool _tr2IsConnected;
};

class trController : public QObject
{
    Q_OBJECT
    QTimer *_trTimer;
    QThread workerThread;
public:
    trController() {
        _trTimer = new QTimer(this);
        connect(_trTimer, &QTimer::timeout, this, &trController::trTimeout);

        mTRCRThread *worker = new mTRCRThread;
        worker->moveToThread(&workerThread);

        connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &trController::trTimeout, worker, &mTRCRThread::updateTRCR);
        connect(this, &trController::connectTRSignal, worker, &mTRCRThread::connectZQWL);
        connect(this, &trController::disConnectTRSignal, worker, &mTRCRThread::disConnectZQWL);
        connect(worker, &mTRCRThread::tRCRData, this, &trController::threadTrData);

        workerThread.start();
    }
    ~trController() {
        workerThread.quit();
        workerThread.wait();
    }

public:
    void connectTR() { emit connectTRSignal(); }//因本线程初始化在测试前，所以温升记录仪串口号有可能被用户改动
                                                //用于测试前连接网段

signals:
    void trTimeout();
    void threadTrData(const QStringList &);
    void connectTRSignal();
    void disConnectTRSignal();

public Q_SLOTS:
    void threadTrTimerStart() { if(!_trTimer->isActive()) _trTimer->start(5000); }
    void threadTrTimerStop() { if(_trTimer->isActive()) _trTimer->stop(); }

};

#endif // MTRCRTHREAD_H
