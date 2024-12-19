#include "recvfile.h"
#include <QFile>
#include <QMessageBox>

RecvFile::RecvFile(QTcpSocket *socket, QObject *parent)
    : QThread{parent}
{
    this->socket = socket;
}

void RecvFile::run()
{
    QFile *file = new QFile("recv.txt");
    file->open(QFile::WriteOnly);
    connect(socket, &QTcpSocket::readyRead, this, [=]() {
        static int count = 0, total = 0;
        if (count == 0)
        {
            socket->read((char*)&total, 4);
        }
        QByteArray all = socket->readAll();
        count += all.size();
        file->write(all);

        if (count == total)
        {
            socket->close();
            socket->deleteLater();

            file->close();
            file->deleteLater();

            emit recvFinished();
        }
    });
    // 进入事件循环
    exec();
}

