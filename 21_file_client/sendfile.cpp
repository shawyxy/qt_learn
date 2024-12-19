#include "sendfile.h"
#include <QHostAddress>
#include <QFile>
#include <QFileInfo>

SendFile::SendFile(QObject *parent)
    : QObject{parent}
{}

void SendFile::connectServer(QString ip, unsigned short port)
{
    socket = new QTcpSocket();
    socket->connectToHost(QHostAddress(ip), port);
    connect(socket, &QTcpSocket::connected, this, &SendFile::connected);
    connect(socket, &QTcpSocket::disconnected, this, [=]() {
        socket->close();
        socket->deleteLater();
        emit disconnected();
    });
}

void SendFile::sendFile(QString path)
{
    // 打开文件
    QFile file(path);
    QFileInfo info(path);
    int fileSize = info.size();
    file.open(QFile::ReadOnly);

    // 按行读取文件
    while (!file.atEnd())
    {
        static int num = 0;
        if (num == 0)
        {
            socket->write((char *)&fileSize, 4);
        }
        QByteArray line = file.readLine();
        num += line.size();
        int percent = (num * 100 / fileSize);
        emit curPercent(percent);
        socket->write(line);
    }
}
