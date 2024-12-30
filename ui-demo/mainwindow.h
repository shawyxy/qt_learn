// #ifndef MAINWINDOW_H
// #define MAINWINDOW_H

// #include <QMainWindow>
// #include <QThread>
// #include <QVBoxLayout>

// #include "camera.h"
// #include "serialport.h"
// #include "threads.h"

// QT_BEGIN_NAMESPACE
// namespace Ui {
// class MainWindow;
// }
// QT_END_NAMESPACE

// class MainWindow : public QMainWindow {
//   Q_OBJECT

// public:
//   MainWindow(QWidget *parent = nullptr);
//   ~MainWindow();

//   int CameraInit();
//   int SerialInit();
//   void msgHandler(QString &msg);

// public slots:
//   void SLOT_cameraOpen();

//   void SLOT_cameraTakePic();

//   void SLOT_serialOpen();

//   void SLOT_serialEditSend();

//   void SLOT_serialTimerSend();

// signals:
//   void SIGNAL_serialOpened();

// private:
//   Ui::MainWindow *ui;

//   QThread *cameraThread;
//   Camera *camera;
//   int preCameraIndex;

//   SerialPort *serialPort;
//   int preSerialIndex;

//   QVBoxLayout *layout_log;
//   int timerCount = 0;
//   bool isTimerMsg = false;
//   QThread *timerThread;
//   Timer *timer;
//   QString buffer;
// };

// #endif // MAINWINDOW_H


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include "camera.h"
#include "serialport.h"
#include "timer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void SIGNAL_openCamera(int index);
    void SIGNAL_takePicture(int index);
    void SIGNAL_openSerial(int index);
    void SIGNAL_sendSerialData(int index, const std::string &data);

private:
    Ui::MainWindow *ui;
    Camera *camera;
    SerialPort *serialPort;
    QThread *cameraThread;
    QThread *serialThread;
    Timer *timerWorker;
    QThread *timerThread;
};

#endif // MAINWINDOW_H


