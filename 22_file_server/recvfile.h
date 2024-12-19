#ifndef RECVFILE_H
#define RECVFILE_H

#include <QThread>
#include <QTcpSocket>

class RecvFile : public QThread
{
    Q_OBJECT
public:
    explicit RecvFile(QTcpSocket *socket, QObject *parent = nullptr);
protected:
    void run();
signals:
    void recvFinished();
private:
    QTcpSocket *socket;
};

#endif // RECVFILE_H
