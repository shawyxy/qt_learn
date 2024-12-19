#ifndef SENDFILE_H
#define SENDFILE_H

#include <QObject>
#include <QTcpSocket>


// 任务函数在子线程中被调用
class SendFile : public QObject
{
    Q_OBJECT
public:
    explicit SendFile(QObject *parent = nullptr);
    // 连接服务器
    void connectServer(QString ip, unsigned short port);
    // 发送文件
    void sendFile(QString path);
signals:
    // 子线程通过信号告知主线程连接情况
    void connected();
    void disconnected();
    // 返回百分比
    void curPercent(int percent);
private:
    QTcpSocket *socket = nullptr;
};

#endif // SENDFILE_H
