#ifndef MLOADTHREAD_H
#define MLOADTHREAD_H

#include <QMutex>
#include <QObject>
#include <QThread>
#include <QTimer>

#ifdef _MSC_BUILD
#pragma execution_character_set("utf-8")
#endif

class QTcpSocket;

/*!
 * \brief The load thread class
 */
class mLoadThread : public QObject
{
    Q_OBJECT
    QMutex mutex;
    QString _voltData;
    QString _loadData;
public:
    explicit mLoadThread(int uart, QObject *parent = 0);
    ~mLoadThread();

    QString getVoltData(const QString &text) const;
    QString getLoadData(const QString &text) const;

    bool    isFirst;                                        /*!* \brief 判断首次收到的有效数据 */
    QString judgeVoltage(double v1, double v2);             /*!* \brief 优化电压数据 */
    QString judgeCurrent(double c1, double c2);             /*!* \brief 优化电流数据 */
    QString judgeLPF(double l1, double l2);                 /*!* \brief 优化功率因素数据 */
    QString dataControl(QString data, double c, double pf); /*!* \brief 数据控制 */

signals:
    void loadData(const QString &, const QString &);                       /*!* \brief 数据更新信号 */
    void changeVoltage(QString);                            /*!* \brief 电压需要微调信号 */

public slots:
    void updateData();                                      /*!* \brief 接受负载数据 */

private:
    time_t  _time;
    QString servo;                                          /*!* \brief 项目伺服号 */
    QString oldVole;                                        /*!* \brief 旧电压数据 */
    QString oldLoadA;                                       /*!* \brief 旧负载数据 */
    QString oldLoadB;                                       /*!* \brief  */
    QString oldLoadC;                                       /*!* \brief  */

    double  trLoadU;                                         /*!* \brief 负载微调幅度 */
    double  trLoadD;                                         /*!* \brief 负载微调幅度 */
    bool    isOn;                                           /*!* \brief 判断此次数据是否为有效通值 */
    bool    isOff;                                          /*!* \brief 判断此次数据是否为有效断值 */

    QTcpSocket *_voltClient;
    QTcpSocket *_loadClient;
};

class loadController : public QObject
{
    Q_OBJECT
    QTimer *_loadTimer;
    QThread workerThread;
public:
    loadController(int uart) {
        _loadTimer = new QTimer(this);
        connect(_loadTimer, &QTimer::timeout, this, &loadController::loadTimeout);

        mLoadThread *worker = new mLoadThread(uart);
        worker->moveToThread(&workerThread);

        connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &loadController::loadTimeout, worker, &mLoadThread::updateData);
        connect(worker, &mLoadThread::loadData, this, &loadController::threadLoadData);

        workerThread.start();
    }
    ~loadController() {
        workerThread.quit();
        workerThread.wait();
    }
signals:
    void loadTimeout();
    void threadLoadData(const QString &, const QString &);

public Q_SLOTS:
    void threadLoadTimerStart(time_t msec) { if(!_loadTimer->isActive()) _loadTimer->start(msec); }
    void threadLoadTimerStop() { if(_loadTimer->isActive()) _loadTimer->stop(); }
};

#endif // MLOADTHREAD_H
