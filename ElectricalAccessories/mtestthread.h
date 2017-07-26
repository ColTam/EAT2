#ifndef MTESTTHREAD_H
#define MTESTTHREAD_H

#include <QObject>
#include <QMutex>
#include <QThread>
#include <QTimer>
#include <QTcpSocket>

#ifdef _MSC_BUILD
#pragma execution_character_set("utf-8")
#endif

class QTimer;

/*!
 * \brief The life tester thread class
 */
class mTestThread : public QObject
{
    Q_OBJECT
    QMutex mutex;
public:
    explicit mTestThread(QObject *parent = 0);
    ~mTestThread();

    QString getData(const QString &text) const;

signals:
    void testData(const QString &, const QString &, const QString &);/*!* \brief 伺服数据信号 */

public slots:
    void updateTest();       /*!* \brief 接收寿命测试机数据槽函数 */

private:
    QString _data;
    QString oldNumberA;     /*!* \brief 旧寿命测试机数据 */
    QString oldNumberB;     /*!* \brief  */
    QString oldNumberC;     /*!* \brief  */

    QTcpSocket *_testClient;
};

class testController : public QObject
{
    Q_OBJECT
    QTimer *_testTimer;
    QThread workerThread;
public:
    testController() {
        _testTimer = new QTimer(this);
        connect(_testTimer, &QTimer::timeout, this, &testController::testTimeout);

        mTestThread *worker = new mTestThread;
        worker->moveToThread(&workerThread);

        connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &testController::testTimeout, worker, &mTestThread::updateTest);
        connect(worker, &mTestThread::testData, this, &testController::threadTestData);

        workerThread.start();
    }
    ~testController() {
        workerThread.quit();
        workerThread.wait();
    }
signals:
    void testTimeout();
    void threadTestData(const QString &, const QString &, const QString &);

public Q_SLOTS:
    void threadTestTimerStart() { if(!_testTimer->isActive()) _testTimer->start(1000); }
    void threadTestTimerStop() { if(_testTimer->isActive()) _testTimer->stop(); }
};

#endif // MTESTTHREAD_H
