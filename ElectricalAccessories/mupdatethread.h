#ifndef MUPDATETHREAD_H
#define MUPDATETHREAD_H

#include <QObject>
#include <QMutex>
#include <QVector>
#include <QThread>

class QTcpSocket;

class mUpdateThread : public QObject
{
    Q_OBJECT
    QMutex mutex;
    QTcpSocket *_updateClient;
    QString socketData;
public:
    explicit mUpdateThread(QObject *parent = nullptr);

    QString getData(const QString &text) const;

signals:
    void updatedState();

public slots:
    void updateState();
};

class updateController : public QObject
{
    Q_OBJECT
    QThread workerThread;
public:
    updateController() {
        mUpdateThread *worker = new mUpdateThread;
        worker->moveToThread(&workerThread);

        connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &updateController::updateUart, worker, &mUpdateThread::updateState);
        connect(worker, &mUpdateThread::updatedState, this, &updateController::getUpdateUart);

        workerThread.start();
    }
    ~updateController() {
        workerThread.quit();
        workerThread.wait();
    }
signals:
    void updateUart();
    void getUpdateUart();
};

#endif // MUPDATETHREAD_H
