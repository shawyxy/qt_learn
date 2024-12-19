#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <thread>
#include <vector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , camera(new Camera)
    , serialPort(new SerialPort)
{
    ui->setupUi(this);

    // 设置图像接收回调
    auto imageCall = [=](CameraHandle handle, unsigned int width, unsigned int height, unsigned int channel,
                         unsigned int type, unsigned int step, unsigned char* data, const char* info) {
        ui->label_device_info->setText(info);
        camera->_imgBuf_ptr = data;

        QImage image(data, width, height, step * width, QImage::Format_BGR888);
        qDebug() << "image.size: " << image.size();

        ui->label_image->setScaledContents(true);
        ui->label_image->setPixmap(QPixmap::fromImage(image));
    };

    camera->imageCall = imageCall;
    // camera->start();


    serialPort->start();
    while (1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout << "send data" << std::endl;
        serialPort->send("xxxxxxxxxxxxx");
        std::vector<int> a = {1, 2, 3};
        // serialPort->send(a);
    }

}

MainWindow::~MainWindow()
{
    // camera->stop();
    serialPort->stop();

    delete camera;
    delete serialPort;

    camera = nullptr;
    serialPort = nullptr;
    delete ui;
}
