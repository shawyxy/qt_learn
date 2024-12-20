#include "mainwindow.h"

#include <QApplication>
#include <thread>


int main(int argc, char *argv[])
{
    // QApplication a(argc, argv);
    // MainWindow w;
    // w.show();


    SerialPort *sendPort = new SerialPort("COM4");
    SerialPort *recvPort = new SerialPort("COM2");
    sendPort->start();
    recvPort->start();

    std::string msg = "xxxxxxxxxYYYYYYY";
    int i = 3;
    while (i)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << sendPort->portName << " send: " << msg << std::endl;
        sendPort->send(msg);
    }

    sendPort->stop();
    delete sendPort;
    recvPort->stop();
    delete recvPort;



    // return a.exec();
}
