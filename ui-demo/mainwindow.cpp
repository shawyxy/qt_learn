// #include "mainwindow.h"
// #include "ui_mainwindow.h"

// #include <iostream>

// MainWindow::MainWindow(QWidget *parent)
//     : QMainWindow(parent), ui(new Ui::MainWindow), camera(new Camera),
//       serialPort(new SerialPort) {
//   ui->setupUi(this);
//   setWindowTitle("xyz-demo");
//   setMaximumSize(QSize(1249, 758));
//   setMinimumSize(QSize(1249, 758));

//   CameraInit();

//   connect(ui->pushButton_cameraOpen, &QPushButton::clicked, this,
//           &MainWindow::SLOT_cameraOpen);
//   connect(ui->pushButton_cameraTake, &QPushButton::clicked, this,
//           &MainWindow::SLOT_cameraTakePic);

//   SerialInit();

//   timerThread = new QThread;
//   timer = new Timer;
//   timer->moveToThread(timerThread);

//   connect(ui->pushButton_serialOpen, &QPushButton::clicked, this,
//           &MainWindow::SLOT_serialOpen);
//   connect(ui->pushButton_send, &QPushButton::clicked, this,
//           &MainWindow::SLOT_serialEditSend);

//   connect(this, &MainWindow::SIGNAL_serialOpened,
//           [=]() { timerThread->start(); });
//   connect(timerThread, &QThread::started, timer, &Timer::timerStart);
//   connect(timer, &Timer::timeoutSignal, this,
//           &MainWindow::SLOT_serialTimerSend);

//   connect(ui->pushButton_xHome, &QPushButton::clicked, this,
//           [=]() { serialPort->send(preSerialIndex, "$HX\r\n"); });
//   connect(ui->pushButton_yHome, &QPushButton::clicked, this,
//           [=]() { serialPort->send(preSerialIndex, "$HY\r\n"); });
//   connect(ui->pushButton_zHome, &QPushButton::clicked, this,
//           [=]() { serialPort->send(preSerialIndex, "$HZ\r\n"); });

//   connect(ui->pushButton_xPlus, &QPushButton::clicked, this, [=]() {
//     std::string step = ui->lineEdit_xStep->text().toStdString();
//     serialPort->send(preSerialIndex, "G91X" + step + "F100\r\n");
//   });
//   connect(ui->pushButton_yPlus, &QPushButton::clicked, this, [=]() {
//     std::string step = ui->lineEdit_yStep->text().toStdString();
//     serialPort->send(preSerialIndex, "G91Y" + step + "F100\r\n");
//   });
//   connect(ui->pushButton_zPlus, &QPushButton::clicked, this, [=]() {
//     std::string step = ui->lineEdit_zStep->text().toStdString();
//     serialPort->send(preSerialIndex, "G91Z" + step + "F100\r\n");
//   });

//   connect(ui->pushButton_xMinus, &QPushButton::clicked, this, [=]() {
//     std::string step = "-" + ui->lineEdit_xStep->text().toStdString();
//     serialPort->send(preSerialIndex, "G91X" + step + "F100\r\n");
//   });
//   connect(ui->pushButton_yMinus, &QPushButton::clicked, this, [=]() {
//     std::string step = "-" + ui->lineEdit_yStep->text().toStdString();
//     serialPort->send(preSerialIndex, "G91Y" + step + "F100\r\n");
//   });
//   connect(ui->pushButton_zMinus, &QPushButton::clicked, this, [=]() {
//     std::string step = "-" + ui->lineEdit_zStep->text().toStdString();
//     serialPort->send(preSerialIndex, "G91Z" + step + "F100\r\n");
//   });

//   connect(ui->pushButton_unlock, &QPushButton::clicked, this,
//           [=]() { serialPort->send(preSerialIndex, "$X\r\n"); });
// }

// MainWindow::~MainWindow() {
//   timerThread->quit();
//   timerThread->wait();
//   if (serialPort->indexToPortInfo[preSerialIndex].isOpened) {
//     serialPort->close(preSerialIndex);
//   }
//   delete ui;
// }

// ////////////////////////////////相机///////////////////////////////////
// int MainWindow::CameraInit() {
//   ui->comboBox_cameraList->blockSignals(true);

//   QJsonArray cameraIds = camera->getCameraList();
//   for (auto id : cameraIds) {
//     ui->comboBox_cameraList->addItem(id.toString());
//   }
//   preCameraIndex = -1;
//   ui->comboBox_cameraList->setCurrentIndex(0);

//   ui->comboBox_cameraList->blockSignals(false);
//   return 0;
// }

// void MainWindow::SLOT_cameraOpen() {
//   qDebug() << "SLOT_cameraOpen()";
//   // 图像接收回调
//   auto imageCall = [=](CameraHandle handle, unsigned int width,
//                        unsigned int height, unsigned int channel,
//                        unsigned int type, unsigned int step,
//                        unsigned char *data, const char *info) {
//     camera->_imgBuf_ptr = data;
//     QImage image(data, width, height, step * width, QImage::Format_BGR888);

//     // 将图像更新操作放到主线程
//     QMetaObject::invokeMethod(
//         this,
//         [=]() {
//           ui->label_image->setScaledContents(true);
//           ui->label_image->setPixmap(QPixmap::fromImage(image));

//           QSize imageSize = ui->label_image->pixmap().size();

//           int maxWidth = 800;
//           int maxHeight = 600;
//           int minWidth = 400;
//           int minHeight = 300;
//           // 计算比例
//           float aspectRatio = (float)imageSize.width() / imageSize.height();
//           int scaledWidth = imageSize.width();
//           int scaledHeight = imageSize.height();
//           // 缩放
//           if (scaledWidth > maxWidth) {
//             scaledWidth = maxWidth;
//             scaledHeight = scaledWidth / aspectRatio;
//           }
//           if (scaledHeight > maxHeight) {
//             scaledHeight = maxHeight;
//             scaledWidth = scaledHeight * aspectRatio;
//           }
//           if (scaledWidth < minWidth) {
//             scaledWidth = minWidth;
//             scaledHeight = scaledWidth / aspectRatio;
//           }
//           if (scaledHeight < minHeight) {
//             scaledHeight = minHeight;
//             scaledWidth = scaledHeight * aspectRatio;
//           }
//           ui->label_image->setFixedSize(scaledWidth, scaledHeight);
//         },
//         Qt::QueuedConnection);
//   };

//   // 新选中的设备
//   int index = ui->comboBox_cameraList->currentIndex();
//   // 如果新选中的设备和上次选择的设备是不同的
//   if (index != preCameraIndex) {
//     if (preCameraIndex != -1 &&
//         camera->indexToCameraInfo[preCameraIndex].isOpened == true) {
//       // 清除旧信息
//       ui->label_image->clear();
//       camera->close(preCameraIndex);
//     }
//     camera->create(index, imageCall);
//     camera->open(index);
//     preCameraIndex = index;
//   }
// }

// void MainWindow::SLOT_cameraTakePic() {
//   qDebug() << "SLOT_cameraTakePic()";
//   ui->label_image->clear();
//   camera->take(preCameraIndex);
// }

// ////////////////////////////////串口///////////////////////////////////
// ///
// /// TODO
// ///     1. 轮询坐标(线程计时器，100ms) x
// ///     2. xyz归位 $H x
// ///     3. step(xyz)：G91+step x
// int MainWindow::SerialInit() {
//   layout_log = new QVBoxLayout;
//   ui->plainTextEdit_log->setReadOnly(true);
//   layout_log->addWidget(ui->plainTextEdit_log);

//   ui->lineEdit_xStep->setText("10");
//   ui->lineEdit_yStep->setText("10");
//   ui->lineEdit_zStep->setText("10");

//   ui->comboBox_serialList->blockSignals(true);
//   serialPort->getSerialPorts();
//   for (auto &e : serialPort->ports) {
//     ui->comboBox_serialList->addItem(e.c_str());
//   }
//   preSerialIndex = -1;
//   ui->comboBox_serialList->setCurrentIndex(0);

//   ui->comboBox_serialList->blockSignals(false);
//   return 0;
// }

// void MainWindow::msgHandler(QString &msg) {
//   // qDebug() << "msgHandler: " << msg;

//   QString str, statusStr;
//   QRegularExpression regex("<.*Pos:.*");

//   if (regex.match(msg).hasMatch()) {
//     auto begin = msg.indexOf("|");
//     auto end = msg.indexOf("|", begin + 1);
//     str = msg.mid(begin + 6, end - begin - 6);

//     auto statusBegin = msg.indexOf("<");
//     statusStr = msg.mid(statusBegin + 1, begin - statusBegin - 1);
//     // qDebug() << "statusStr:" << statusStr;
//     ui->label_serial_deviceStatus->setText(statusStr);

//     QStringList posList = str.split(",");
//     if (posList.size() >= 3) {
//       ui->label_xPos->setText(posList[0]);
//       ui->label_yPos->setText(posList[1]);
//       ui->label_zPos->setText(posList[2]);
//     }
//   } else {
//     if (isTimerMsg) {
//       timerCount++;
//       if (timerCount == 2) {
//         isTimerMsg = false;
//         timerCount = 0;
//       }
//     } else {
//       QString log = "←: " + msg;

//       // 将日志更新操作放到主线程
//       QMetaObject::invokeMethod(
//           this,
//           [=]() {
//             ui->plainTextEdit_log->appendPlainText(log);
//             QTextCursor cursor = ui->plainTextEdit_log->textCursor();
//             cursor.movePosition(QTextCursor::End);
//             ui->plainTextEdit_log->setTextCursor(cursor);
//           },
//           Qt::QueuedConnection);
//     }
//   }
// }

// void MainWindow::SLOT_serialOpen() {
//   int index = ui->comboBox_serialList->currentIndex();
//   // 信息接收回调
//   auto recvCall = [&](const ComHandle handle, const char *msg,
//                       unsigned int len) {
//     QString str = QString::fromUtf8(msg, len);

//     buffer.append(str);
//     while (buffer.contains("\r\n")) {
//       int endIndex = buffer.indexOf("\r\n");
//       QString completeMessage = buffer.left(endIndex);
//       buffer = buffer.mid(endIndex + 2);
//       msgHandler(completeMessage);
//     }
//   };

//   if (index != preSerialIndex) {
//     // qDebug() << "pre:" << preSerialIndex << ", idx:" << index;
//     if (preSerialIndex != -1 &&
//         serialPort->indexToPortInfo[preSerialIndex].isOpened == true) {
//       ui->plainTextEdit_log->clear();
//       serialPort->close(preSerialIndex);
//     }
//     std::string portName = ui->comboBox_serialList->currentText().toStdString();
//     serialPort->create(index, portName, recvCall);
//     serialPort->open(index);
//     emit SIGNAL_serialOpened();
//     preSerialIndex = index;
//   }
// }

// void MainWindow::SLOT_serialEditSend() {
//   std::string msg = ui->lineEdit_send->text().toStdString() + "\r\n";
//   // QString log = QTime::currentTime().toString() + "[" +
//   //               QString::fromStdString(
//   //                   serialPort->indexToPortInfo[preSerialIndex].portName +
//   //                   "]send: " + msg);
//   QString log = "=====================\n→: " + QString::fromStdString(msg);
//   // 将日志更新操作放到主线程
//   QMetaObject::invokeMethod(
//       this,
//       [=]() {
//         ui->plainTextEdit_log->appendPlainText(log);
//         QTextCursor cursor = ui->plainTextEdit_log->textCursor();
//         cursor.movePosition(QTextCursor::End);
//         ui->plainTextEdit_log->setTextCursor(cursor);
//       },
//       Qt::QueuedConnection);
//   serialPort->send(preSerialIndex, msg);
// }

// // 用于定时器查询坐标
// void MainWindow::SLOT_serialTimerSend() {
//   isTimerMsg = true;
//   serialPort->send(preSerialIndex, "?\r\n");
// }

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), camera(new Camera),
    serialPort(new SerialPort) {
    ui->setupUi(this);
    setWindowTitle("xyz-demo");
    setMaximumSize(QSize(1249, 758));
    setMinimumSize(QSize(1249, 758));

    CameraInit();

    cameraThread = new QThread(this);
    camera->moveToThread(cameraThread);
    cameraThread->start();

    connect(ui->pushButton_cameraOpen, &QPushButton::clicked, this,
            &MainWindow::SLOT_cameraOpen);
    connect(ui->pushButton_cameraTake, &QPushButton::clicked, this,
            &MainWindow::SLOT_cameraTakePic);

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

    connect(ui->pushButton_xHome, &QPushButton::clicked, this,
            [=]() { serialPort->send(preSerialIndex, "$HX\r\n"); });
    connect(ui->pushButton_yHome, &QPushButton::clicked, this,
            [=]() { serialPort->send(preSerialIndex, "$HY\r\n"); });
    connect(ui->pushButton_zHome, &QPushButton::clicked, this,
            [=]() { serialPort->send(preSerialIndex, "$HZ\r\n"); });

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
    delete ui;
}

int MainWindow::CameraInit() {
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
    }
}

void MainWindow::SLOT_cameraTakePic() {
    qDebug() << "SLOT_cameraTakePic()";
    ui->label_image->clear();
    camera->take(preCameraIndex);
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
            QString log = "\u2190: " + msg;

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

