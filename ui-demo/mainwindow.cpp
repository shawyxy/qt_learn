#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDateTime>
#include <iostream>

#include "json/json.h"
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), camera(new Camera),
      serialPort(new SerialPort) {
  ui->setupUi(this);
  this->setWindowTitle("xyz-demo");
  this->setFixedSize(QSize(1249, 758));

  CameraInit();

  cameraThread = new QThread(this);
  camera->moveToThread(cameraThread);
  cameraThread->start();

  connect(ui->pushButton_cameraOpen, &QPushButton::clicked, this,
          &MainWindow::SLOT_cameraOpen);
  connect(ui->pushButton_cameraTake, &QPushButton::clicked, this,
          &MainWindow::SLOT_cameraTakePic);
  connect(ui->pushButton_templateMatch, &QPushButton::clicked, this,
          &MainWindow::SLOT_templateMatch);

  SerialInit();

  serialPortThread = new QThread(this);
  serialPort->moveToThread(serialPortThread);
  serialPortThread->start();

  connect(ui->pushButton_serialOpen, &QPushButton::clicked, this,
          &MainWindow::SLOT_serialOpen);
  connect(ui->pushButton_send, &QPushButton::clicked, this,
          &MainWindow::SLOT_serialEditSend);

  timerThread = new QThread(this);
  timer = new Timer;
  timer->moveToThread(timerThread);

  connect(this, &MainWindow::SIGNAL_serialOpened,
          [=]() { timerThread->start(); });
  connect(timerThread, &QThread::started, timer, &Timer::timerStart);
  connect(timer, &Timer::timeoutSignal, this,
          &MainWindow::SLOT_serialTimerSend);

  connect(ui->pushButton_xHome, &QPushButton::clicked, this, [=]() {
    tempinfo.init();
    serialPort->send(preSerialIndex, "$HX\r\n");
  });
  connect(ui->pushButton_yHome, &QPushButton::clicked, this, [=]() {
    tempinfo.init();
    serialPort->send(preSerialIndex, "$HY\r\n");
  });
  connect(ui->pushButton_zHome, &QPushButton::clicked, this, [=]() {
    tempinfo.init();
    serialPort->send(preSerialIndex, "$HZ\r\n");
  });
  connect(ui->pushButton_cameraHome, &QPushButton::clicked, this, [=]() {
    serialPort->send(preSerialIndex, "$HX\r\n");
    serialPort->send(preSerialIndex, "$X\r\n");
    serialPort->send(preSerialIndex, "$HY\r\n");
    serialPort->send(preSerialIndex, "$X\r\n");
    serialPort->send(preSerialIndex, "G00X36Y98\r\n");
  });

  connect(ui->pushButton_xPlus, &QPushButton::clicked, this, [=]() {
    std::string step = ui->lineEdit_xStep->text().toStdString();
    serialPort->send(preSerialIndex, "G91X" + step + "F100\r\n");
  });
  connect(ui->pushButton_yPlus, &QPushButton::clicked, this, [=]() {
    std::string step = ui->lineEdit_yStep->text().toStdString();
    serialPort->send(preSerialIndex, "G91Y" + step + "F100\r\n");
  });
  connect(ui->pushButton_zPlus, &QPushButton::clicked, this, [=]() {
    std::string step = ui->lineEdit_zStep->text().toStdString();
    serialPort->send(preSerialIndex, "G91Z" + step + "F100\r\n");
  });

  connect(ui->pushButton_xMinus, &QPushButton::clicked, this, [=]() {
    std::string step = "-" + ui->lineEdit_xStep->text().toStdString();
    serialPort->send(preSerialIndex, "G91X" + step + "F100\r\n");
  });
  connect(ui->pushButton_yMinus, &QPushButton::clicked, this, [=]() {
    std::string step = "-" + ui->lineEdit_yStep->text().toStdString();
    serialPort->send(preSerialIndex, "G91Y" + step + "F100\r\n");
  });
  connect(ui->pushButton_zMinus, &QPushButton::clicked, this, [=]() {
    std::string step = "-" + ui->lineEdit_zStep->text().toStdString();
    serialPort->send(preSerialIndex, "G91Z" + step + "F100\r\n");
  });

  connect(ui->pushButton_unlock, &QPushButton::clicked, this,
          [=]() { serialPort->send(preSerialIndex, "$X\r\n"); });

  connect(ui->pushButton_moveCenter, &QPushButton::clicked, this, [=]() {
    if (isMatched) {
      QString moveCmd = QString("G00X%1Y%2\r\n")
                            .arg(tempinfo.center_x)
                            .arg(tempinfo.center_y);
      serialPort->send(preSerialIndex, moveCmd.toStdString());
      qDebug() << "c:" << moveCmd;
    }
  });
  connect(ui->pushButton_moveNextCenter, &QPushButton::clicked, this, [=]() {
    if (isMatched) {

      pre_center_x = cur_center_x;
      pre_center_y = cur_center_y;
      // cur_center_x += tempinfo.center_x;
      // cur_center_y += tempinfo.center_y;
      qDebug() << "pre_center_x: " << pre_center_x
               << ", pre_center_y: " << pre_center_y;
      qDebug() << "cur_center_x: " << cur_center_x
               << ", cur_center_y: " << cur_center_y;
    }
  });
  connect(ui->pushButton_moveCircle, &QPushButton::clicked, this, [=]() {
    if (isMatched) {
      qDebug() << "tempinfo.x1: " << tempinfo.x1
               << ", tempinfo.y1: " << tempinfo.y1;
      qDebug() << "tempinfo.x2: " << tempinfo.x2
               << ", tempinfo.y2: " << tempinfo.y2;
      qDebug() << "tempinfo.x3: " << tempinfo.x3
               << ", tempinfo.y3: " << tempinfo.y3;
      qDebug() << "tempinfo.x4: " << tempinfo.x4
               << ", tempinfo.y4: " << tempinfo.y4;
      qDebug() << "pre_center_x: " << pre_center_x
               << ", pre_center_y: " << pre_center_y;
      qDebug() << "cur_center_x: " << cur_center_x
               << ", cur_center_y: " << cur_center_y;
      QString moveCmd;
      moveCmd = QString("G00X%1Y%2\r\n").arg(tempinfo.x1).arg(tempinfo.y3);
      // serialPort->send(preSerialIndex, moveCmd.toStdString());
      qDebug() << "1:" << moveCmd;
      moveCmd = QString("G00X%1Y%2\r\n").arg(tempinfo.x2).arg(tempinfo.y4);
      // serialPort->send(preSerialIndex, moveCmd.toStdString());
      qDebug() << "2:" << moveCmd;
      moveCmd = QString("G00X%1Y%2\r\n").arg(tempinfo.x3).arg(tempinfo.y1);
      // serialPort->send(preSerialIndex, moveCmd.toStdString());
      qDebug() << "3:" << moveCmd;
      moveCmd = QString("G00X%1Y%2\r\n").arg(tempinfo.x4).arg(tempinfo.y2);
      // serialPort->send(preSerialIndex, moveCmd.toStdString());
      qDebug() << "4:" << moveCmd;
      moveCmd = QString("G00X%1Y%2\r\n").arg(tempinfo.x1).arg(tempinfo.y3);
      // serialPort->send(preSerialIndex, moveCmd.toStdString());
      qDebug() << "1:" << moveCmd;
      // 移动到中心
      moveCmd = (QString("G00X%1Y%2\r\n")
                     .arg(tempinfo.center_x)
                     .arg(tempinfo.center_y));
      // serialPort->send(preSerialIndex, moveCmd.toStdString());
      qDebug() << "c1:" << moveCmd;
      qDebug() << "tempinfo.center_x: " << tempinfo.center_x
               << ", tempinfo.center_y: " << tempinfo.center_y;
    }
  });
}

MainWindow::~MainWindow() {
  timerThread->quit();
  timerThread->wait();

  cameraThread->quit();
  cameraThread->wait();

  serialPortThread->quit();
  serialPortThread->wait();

  if (serialPort->indexToPortInfo[preSerialIndex].isOpened) {
    serialPort->close(preSerialIndex);
  }
  camera->close(preCameraIndex);

  delete camera;
  delete serialPort;
  delete ui;
}

int MainWindow::CameraInit() {
  temPath = "C:/Users/nj055/Desktop/xyz-demo/xyz-demo/build/"
            "Desktop_Qt_6_7_2_MSVC2019_64bit-Release/test/template5.jpg";
  ui->comboBox_cameraList->blockSignals(true);

  QJsonArray cameraIds = camera->getCameraList();
  for (auto id : cameraIds) {
    ui->comboBox_cameraList->addItem(id.toString());
  }
  preCameraIndex = -1;
  ui->comboBox_cameraList->setCurrentIndex(0);

  ui->comboBox_cameraList->blockSignals(false);
  return 0;
}

void MainWindow::SLOT_cameraOpen() {
  qDebug() << "SLOT_cameraOpen()";
  auto imageCall = [=](CameraHandle handle, unsigned int width,
                       unsigned int height, unsigned int channel,
                       unsigned int type, unsigned int step,
                       unsigned char *data, const char *info) {
    camera->_imgBuf_ptr = data;
    QImage image(data, width, height, step * width, QImage::Format_BGR888);

    qint64 timestamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
    QString savePath = "C:/Users/nj055/Desktop/xyz-demo/xyz-demo/build/"
                       "Desktop_Qt_6_7_2_MSVC2019_64bit-Release/test" +
                       QString("/match_%1.jpg").arg(timestamp);
    srcPath = savePath;

    if (!image.save(savePath)) {
      qDebug() << "save image failed:" << savePath;
    } else {
      qDebug() << "image saved to:" << savePath;
    }

    QMetaObject::invokeMethod(
        this,
        [=]() {
          ui->label_image->setScaledContents(true);
          ui->label_image->setPixmap(QPixmap::fromImage(image));

          QSize imageSize = ui->label_image->pixmap().size();

          int maxWidth = 800;
          int maxHeight = 600;
          int minWidth = 400;
          int minHeight = 300;
          float aspectRatio = (float)imageSize.width() / imageSize.height();
          int scaledWidth = imageSize.width();
          int scaledHeight = imageSize.height();

          if (scaledWidth > maxWidth) {
            scaledWidth = maxWidth;
            scaledHeight = scaledWidth / aspectRatio;
          }
          if (scaledHeight > maxHeight) {
            scaledHeight = maxHeight;
            scaledWidth = scaledHeight * aspectRatio;
          }
          if (scaledWidth < minWidth) {
            scaledWidth = minWidth;
            scaledHeight = scaledWidth / aspectRatio;
          }
          if (scaledHeight < minHeight) {
            scaledHeight = minHeight;
            scaledWidth = scaledHeight * aspectRatio;
          }
          ui->label_image->setFixedSize(scaledWidth, scaledHeight);
        },
        Qt::QueuedConnection);
  };

  int index = ui->comboBox_cameraList->currentIndex();
  if (index != preCameraIndex) {
    if (preCameraIndex != -1 &&
        camera->indexToCameraInfo[preCameraIndex].isOpened == true) {
      ui->label_image->clear();
      camera->close(preCameraIndex);
    }
    camera->create(index, imageCall);
    camera->open(index);
    preCameraIndex = index;
  } else { // for debug
    camera->open(index);
  }
}

void MainWindow::SLOT_cameraTakePic() {
  qDebug() << "SLOT_cameraTakePic()";
  ui->label_image->clear();
  camera->take(preCameraIndex);
}

// 在函数里弹出确认对话框
void MainWindow::SLOT_templateMatch() {
  matchPic = cv::imread(srcPath.toStdString());
  templPic = cv::imread(temPath.toStdString());
  std::vector<cv::Point> SerachArea = {
      cv::Point{0, 0},
      cv::Point{matchPic.size().width - 1, matchPic.size().height - 1}};
  std::vector<cv::Point> ShieledArea;
  double SourceValue = 0.6;
  int MinAngle = 0;
  int MaxAngle = 360;
  cv::Point SourcePoint = {0, 0};
  MatchTemplate *match = new MatchTemplate();
  auto res =
      match->MatchTemplate_Match(matchPic, templPic, SerachArea, ShieledArea,
                                 SourceValue, MinAngle, MaxAngle, SourcePoint);

  if (res.isSuccess()) {
    isMatched = true;
    QVariant variantResult = res.operatorResult<QVariant>();
    QMap<QString, double> resultMap =
        variantResult.value<QMap<QString, double>>();

    cv::Point2f points[4] = {
        cv::Point2f(resultMap["x1"], resultMap["y1"]),
        cv::Point2f(resultMap["x2"], resultMap["y2"]),
        cv::Point2f(resultMap["x3"], resultMap["y3"]),
        cv::Point2f(resultMap["x4"], resultMap["y4"]),
    };

    // 在匹配图像上绘制矩形
    for (int i = 0; i < 4; ++i) {
      cv::line(matchPic, points[i], points[(i + 1) % 4], cv::Scalar(0, 255, 0),
               2);
    }

    // 显示图像
    cv::Mat resizedMatchPic;
    cv::resize(matchPic, resizedMatchPic, cv::Size(), 0.5, 0.5);
    cv::imshow("Matched Template", resizedMatchPic);
    cv::waitKey(0);

    QMessageBox msgBox;
    msgBox.setWindowTitle("确认操作");
    msgBox.setText("是否接受匹配结果");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    int ret = msgBox.exec();

    if (ret == QMessageBox::Ok) {
      /////////仿射变换////////
      double cur_x = resultMap["center_x"];
      double cur_y = resultMap["center_y"];
      angle = resultMap["Angle"];

      // qDebug() << "cur_x: " << cur_x << ", cur_y: " << cur_y;
      // qDebug() << "resultMap[\"Angle\"]: " << resultMap["Angle"];
      // qDebug() << "矩形4点坐标：";
      // for (int i = 0; i < 4; i++) {
      //   qDebug() << "(" << points[i].x << ", " << points[i].y << ")";
      // }

      cv::Point2f inputCenterPoint(cur_x, cur_y);
      cv::Point2f inputPoint0(points[0].x, points[0].y);
      cv::Point2f inputPoint1(points[1].x, points[1].y);
      cv::Point2f inputPoint2(points[2].x, points[2].y);
      cv::Point2f inputPoint3(points[3].x, points[3].y);
      cv::Mat affineMatrix;
      cv::FileStorage fs("C:/Users/nj055/Desktop/xyz-demo/xyz-demo/build/"
                         "Desktop_Qt_6_7_2_MSVC2019_64bit-Release/test/"
                         "nine_calibration_file.yml",
                         cv::FileStorage::READ);
      if (!fs.isOpened()) {
        std::cout << "open yml file failed" << std::endl;
      }

      fs["matrix"] >> affineMatrix;
      fs.release();

      // if (affineMatrix.empty() || affineMatrix.rows != 2 ||
      //     affineMatrix.cols != 3) {
      //   std::cerr << "matrix format error" << std::endl;
      // }

      // std::cout << affineMatrix << std::endl;

      std::vector<cv::Point2f> inputPoints = {
          inputCenterPoint, inputPoint0, inputPoint1, inputPoint2, inputPoint3};

      cv::transform(inputPoints, outputPoints, affineMatrix);

      if (!outputPoints.empty()) {
        ui->lineEdit_send->clear();
        qDebug() << "当前中心坐标："
                 << "(" << tempinfo.center_x << ", " << tempinfo.center_y
                 << ")";
        qDebug() << "匹配中心坐标："
                 << "(" << outputPoints[0].x << ", " << outputPoints[0].y
                 << ")";
        qDebug() << "边框4点坐标：";
        for (int i = 1; i <= 4; i++) {
          qDebug() << "(" << outputPoints[i].x << ", " << outputPoints[i].y
                   << ")";
        }

        // 移动并旋转坐标
        double delta_x = outputPoints[0].x - tempinfo.center_x;
        double delta_y = outputPoints[0].y - tempinfo.center_y;
        tempinfo.x1 += delta_x;
        tempinfo.y1 += delta_y;
        tempinfo.x2 += delta_x;
        tempinfo.y2 += delta_y;
        tempinfo.x3 += delta_x;
        tempinfo.y3 += delta_y;
        tempinfo.x4 += delta_x;
        tempinfo.y4 += delta_y;
        tempinfo.center_x += delta_x + cur_center_x;
        tempinfo.center_y += delta_y + cur_center_y;

        qDebug() << "目的中心坐标："
                 << "(" << tempinfo.center_x << ", " << tempinfo.center_y
                 << ")";

        double angle_rad = angle * M_PI / 180.0; // 角度转弧度
        double cos_angle = cos(angle_rad);
        double sin_angle = sin(angle_rad);

        auto rotate = [&](double &x, double &y) {
          double new_x = cos_angle * (x - cur_center_x) -
                         sin_angle * (y - cur_center_y) + cur_center_x;
          double new_y = sin_angle * (x - cur_center_x) +
                         cos_angle * (y - cur_center_y) + cur_center_y;
          x = new_x;
          y = new_y;
        };

        cur_center_x = tempinfo.center_x;
        cur_center_y = tempinfo.center_y;

        // auto rotate = [&](double &x, double &y) {
        //     double new_x = cos_angle * (x - tempinfo.center_x) -
        //                    sin_angle * (y - tempinfo.center_y) +
        //                    tempinfo.center_x;
        //     double new_y = sin_angle * (x - tempinfo.center_x) +
        //                    cos_angle * (y - tempinfo.center_y) +
        //                    tempinfo.center_y;
        //     x = new_x;
        //     y = new_y;
        // };

        rotate(tempinfo.x1, tempinfo.y1);
        rotate(tempinfo.x2, tempinfo.y2);
        rotate(tempinfo.x3, tempinfo.y3);
        rotate(tempinfo.x4, tempinfo.y4);

        qDebug() << "delta_x: " << delta_x << ", delta_y: " << delta_y;
        qDebug() << "tempinfo.x1: " << tempinfo.x1
                 << ", tempinfo.y1: " << tempinfo.y1;
        qDebug() << "tempinfo.x2: " << tempinfo.x2
                 << ", tempinfo.y2: " << tempinfo.y2;
        qDebug() << "tempinfo.x3: " << tempinfo.x3
                 << ", tempinfo.y3: " << tempinfo.y3;
        qDebug() << "tempinfo.x4: " << tempinfo.x4
                 << ", tempinfo.y4: " << tempinfo.y4;

      } else {
        std::cout << "transformation failed" << std::endl;
      }
    } else {
      qDebug() << "取消匹配";
      isMatched = false;
    }
  } else {
    qDebug() << "模板匹配失败";
    isMatched = false;
  }
}

int MainWindow::SerialInit() {
  layout_log = new QVBoxLayout;
  ui->plainTextEdit_log->setReadOnly(true);
  layout_log->addWidget(ui->plainTextEdit_log);

  ui->lineEdit_xStep->setText("10");
  ui->lineEdit_yStep->setText("10");
  ui->lineEdit_zStep->setText("10");

  ui->comboBox_serialList->blockSignals(true);
  serialPort->getSerialPorts();
  for (auto &e : serialPort->ports) {
    ui->comboBox_serialList->addItem(e.c_str());
  }
  preSerialIndex = -1;
  ui->comboBox_serialList->setCurrentIndex(0);

  ui->comboBox_serialList->blockSignals(false);
  return 0;
}

void MainWindow::msgHandler(QString &msg) {
  QString str, statusStr;
  QRegularExpression regex("<.*Pos:.*");

  if (regex.match(msg).hasMatch()) {
    auto begin = msg.indexOf("|");
    auto end = msg.indexOf("|", begin + 1);
    str = msg.mid(begin + 6, end - begin - 6);

    auto statusBegin = msg.indexOf("<");
    statusStr = msg.mid(statusBegin + 1, begin - statusBegin - 1);
    ui->label_serial_deviceStatus->setText(statusStr);

    QStringList posList = str.split(",");
    if (posList.size() >= 3) {

      ui->label_xPos->setText(posList[0]);
      ui->label_yPos->setText(posList[1]);
      ui->label_zPos->setText(posList[2]);
    }
  } else {
    if (isTimerMsg) {
      timerCount++;
      if (timerCount == 2) {
        isTimerMsg = false;
        timerCount = 0;
      }
    } else {
      QString log = msg;

      QMetaObject::invokeMethod(
          this,
          [=]() {
            ui->plainTextEdit_log->appendPlainText(log);
            QTextCursor cursor = ui->plainTextEdit_log->textCursor();
            cursor.movePosition(QTextCursor::End);
            ui->plainTextEdit_log->setTextCursor(cursor);
          },
          Qt::QueuedConnection);
    }
  }
}

void MainWindow::SLOT_serialOpen() {
  int index = ui->comboBox_serialList->currentIndex();
  auto recvCall = [&](const ComHandle handle, const char *msg,
                      unsigned int len) {
    QString str = QString::fromUtf8(msg, len);

    buffer.append(str);
    while (buffer.contains("\r\n")) {
      int endIndex = buffer.indexOf("\r\n");
      QString completeMessage = buffer.left(endIndex);
      buffer = buffer.mid(endIndex + 2);
      msgHandler(completeMessage);
    }
  };

  if (index != preSerialIndex) {
    if (preSerialIndex != -1 &&
        serialPort->indexToPortInfo[preSerialIndex].isOpened == true) {
      ui->plainTextEdit_log->clear();
      serialPort->close(preSerialIndex);
    }
    std::string portName = ui->comboBox_serialList->currentText().toStdString();
    serialPort->create(index, portName, recvCall);
    serialPort->open(index);
    emit SIGNAL_serialOpened();
    preSerialIndex = index;
  }
}

void MainWindow::SLOT_serialEditSend() {
  std::string msg = ui->lineEdit_send->text().toStdString() + "\r\n";
  QString log = "=====================\n→: " + QString::fromStdString(msg);
  QMetaObject::invokeMethod(
      this,
      [=]() {
        ui->plainTextEdit_log->appendPlainText(log);
        QTextCursor cursor = ui->plainTextEdit_log->textCursor();
        cursor.movePosition(QTextCursor::End);
        ui->plainTextEdit_log->setTextCursor(cursor);
      },
      Qt::QueuedConnection);
  serialPort->send(preSerialIndex, msg);
}

void MainWindow::SLOT_serialTimerSend() {
  isTimerMsg = true;
  serialPort->send(preSerialIndex, "?\r\n");
}
