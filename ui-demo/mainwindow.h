#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QVBoxLayout>

#include "camera.h"
#include "serialport.h"
#include "timer.h"

#include "MatchTemplate.h"
#include "loc_opencv_predictor_interface.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  int CameraInit();
  int SerialInit();
  void msgHandler(QString &msg);

public slots:
  void SLOT_cameraOpen();
  void SLOT_cameraTakePic();
  void SLOT_templateMatch();
  void SLOT_serialOpen();
  void SLOT_serialEditSend();
  void SLOT_serialTimerSend();
signals:
  void SIGNAL_serialOpened();

private:
  Ui::MainWindow *ui;

  QThread *cameraThread;
  Camera *camera;
  int preCameraIndex;
  QString srcPath;
  QString temPath;

  QThread *serialPortThread;
  SerialPort *serialPort;
  int preSerialIndex;

  QVBoxLayout *layout_log;

  int timerCount = 0;
  bool isTimerMsg = false;
  QThread *timerThread;
  Timer *timer;
  QString buffer;

  double cur_center_x = 0.0;
  double cur_center_y = 0.0;
  double pre_center_x = 0.0;
  double pre_center_y = 0.0;
  cv::Mat matchPic;
  cv::Mat templPic;
  double angle = 0.0;

  class TempInfo {
  public:
    void init() {
      x1 = 0, y1 = 0;
      x2 = width, y3 = 0;
      x3 = width, y3 = height;
      x4 = 0, y4 = height;
      center_x = 1.0;
      center_y = 1.0;
      // center_x = (x1 + x2 + x3 + x4) / 4;
      // center_y = (y1 + y2 + y3 + y4) / 4;
    }
    double width = 53.0;
    double height = 14.0;
    double x1 = 0, y1 = 0;
    double x2 = width, y2 = 0;
    double x3 = width, y3 = height;
    double x4 = 0, y4 = height;
    double center_x = 1.0;
    double center_y = 1.0;
  };
  TempInfo tempinfo;
  bool isMatched = false;

  std::vector<cv::Point2f> outputPoints;
};

#endif // MAINWINDOW_H
