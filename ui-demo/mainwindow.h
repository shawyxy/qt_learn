#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "CameraCore.h"
#include "CameraDefault/CameraDefault.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <QJsonArray>
#include <QString>
#include <QDebug>
#include <QVector>
#include <iostream>
#include <functional>
#include <cstring>



#include <CommunicationCore.h>
#include <CommunicationDefault/CommunicationDefault.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <QJsonArray>
#include <QString>
#include <QDebug>
#include <QVector>
#include <vector>
#include <iostream>

class Camera
{
public:
    Gauss_CameraCore *cameraCore;
    QVector<std::string> *deviceInfoList;
    CameraHandle handle;
    unsigned char* _imgBuf_ptr = nullptr;
public:
    std::function<void(CameraHandle handle, unsigned int width, unsigned int height, unsigned int channel,
                       unsigned int type, unsigned int step, unsigned char* data, const char* info)> imageCall;

    void start()
    {
        // 创建相机管理对象
        cameraCore = new Gauss_CameraCore;
        // 获取相机列表
        cameraCore->Gauss_CameraCore_FindCamera();
        std::string cameraListJson = cameraCore->Gauss_CameraCore_GetCameraList();
        // 转换
        QByteArray jsonData = QByteArray::fromStdString(cameraListJson);
        // 解析
        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        QJsonObject jsonObj = doc.object();
        // 获取相机ID列表
        QJsonArray ids = jsonObj["Gige"].toArray();
        // 构造相机信息Json
        QVector<std::string> deviceInfoList;
        for (const QJsonValue &id : ids)
        {
            QJsonObject deviceInfo;
            deviceInfo["DeviceType"] = (int)(GaussCameraDeviceType::HKCamera);
            deviceInfo["DeviceID"] = id.toString();

            QJsonArray viewList;
            viewList.append("View0001");
            viewList.append("View0002");
            deviceInfo["ViewIDList"] = viewList;

            deviceInfo["TriggerMode"] = false;
            deviceInfo["TriggerSource"] = 0x00;
            deviceInfo["ExposureAuto"] = false;
            deviceInfo["ExposureTime"] = 1000.0;
            deviceInfo["ViewID"] = "";
            deviceInfo["WorkPiceID"] = "";
            deviceInfo["Barcode"] = "";
            deviceInfo["BarCodeList"] = "";

            QJsonDocument newDoc(deviceInfo);
            std::string newJson = newDoc.toJson(QJsonDocument::Compact).toStdString();
            deviceInfoList.push_back(newJson);
        }

        // 创建相机设备, 获取句柄(for test)
        qDebug() << "deviceInfoList size: " << deviceInfoList.size();
        int x = 1;
        handle = cameraCore->Gauss_CameraCore_CreateCamera(deviceInfoList[x]);
        qDebug() << "handle: " << handle;
        int res = 0;

        // 设置图像回调
        if ((res = cameraCore->Gauss_CameraCore_SetImageReady(handle, imageCall)) < 0)
        {
            qDebug() << "Gauss_CameraCore_SetImageReady() failed: " << res;
        }

        // 打开相机
        if ((res = cameraCore->Gauss_CameraCore_OpenCamera(handle)) < 0)
        {
            qDebug() << "Gauss_CameraCore_OpenCamera() failed: " << res;
        }

        // 触发相机拍照
        if ((res = cameraCore->Gauss_CameraCore_TriggerOnce(handle, deviceInfoList[x])) < 0)
        {
            qDebug() << "Gauss_CameraCore_TriggerOnce() failed: " << res;
        }
    }

    void stop()
    {
        // 关闭相机
        if (cameraCore->Gauss_CameraCore_CloseCamera(handle) < 0)
        {
            qDebug() << "Gauss_CameraCore_CloseCamera() failed";
        }
        if (cameraCore)
        {
            delete cameraCore;
            cameraCore = nullptr;
        }
        if (_imgBuf_ptr)
        {
            delete _imgBuf_ptr;
            _imgBuf_ptr = nullptr;
        }
    }
};



class SerialPort
{
public:
    Gauss_CommunicationCore *commCore;
    ComHandle handle;
    std::string portName;
public:
    SerialPort(std::string name)
        : portName(name)
    {}
public:
    // 回调
    void recv(const ComHandle handle, const char* data, unsigned int length)
    {
        std::string msg(data, length);
        std::cout << "recv: " << msg << std::endl;
    }

    void conn(const ComHandle handle, const char* info, unsigned int len, const bool isConnect)
    {
        qDebug() << "conn";
        std::string msg(info, len);
        std::cout << "info: " << msg << ", isConnect: " << isConnect << std::endl;
    }


public:
    void debug(int handle)
    {
        unsigned int errnu = commCore->Gauss_CommunicationCore_GetLastError(handle);
        qDebug() << "SetRecvCall error: " << QString("%1").arg(errnu, 8, 16);
    }

    void start()
    {
        qDebug() << "start()";
        commCore = new Gauss_CommunicationCore();
        // 构造串口信息Json
        QJsonObject serialPortJson;
        serialPortJson["DeviceType"] = 5;
        serialPortJson["Protocol"]   = 0;
        serialPortJson["LuaFilePath"] = "";
        serialPortJson["PortName"] = portName.c_str();
        serialPortJson["BaudRate"] = 9600;
        serialPortJson["DataBit"] = 8;
        serialPortJson["StopBit"] = 1;
        serialPortJson["ParityBit"] = 0;
        serialPortJson["FlowControl"] = 0;
        QJsonDocument doc(serialPortJson);
        std::string serialPortInfo = doc.toJson(QJsonDocument::Compact).toStdString();

        // 创建设备
        handle = commCore->Gauss_CommunicationCore_CreateDevice(serialPortInfo);
        qDebug() << "created, handle: " << handle;

        // 设置回调函数
        int res = 0;
        res = commCore->Gauss_CommunicationCore_SetConnectCallBack(handle, [=](const ComHandle handle, const char* info, unsigned int len, const bool isConnect) {
            std::string msg(info, len);
            std::cout << "info: " << msg << ", isConnect: " << isConnect << std::endl;
        });
        if (res != 0)
        {
            debug(handle);
        }

        res = commCore->Gauss_CommunicationCore_SetReadyReadCallBack(handle, [=](const ComHandle handle, const char* data, unsigned int length) {
            std::string msg(data, length);
            std::cout << portName << " recv: " << msg << std::endl;
        });
        if (res != 0)
        {
            debug(handle);
        }

        // 打开设备
        res = commCore->Gauss_CommunicationCore_OpenDevice(handle);
        if (res != 0)
        {
            debug(handle);
        }
    }

    void send(const std::string &data)
    {
        QJsonObject obj;
        obj["data"] = data.c_str();
        QJsonDocument doc(obj);
        std::string jsonData = doc.toJson(QJsonDocument::Compact).toStdString();
        int res = 0;
        if ((res = commCore->Gauss_CommunicationCore_WriteAsyn(handle, jsonData)) < 0)
        {
            debug(handle);
        }
    }

    void stop()
    {
        qDebug() << "stop()";
        if (commCore->Gauss_CommunicationCore_CloseDevice(handle) < 0)
        {
            debug(handle);
        }
        if (commCore)
        {
            delete commCore;
            commCore = nullptr;
        }
    }
};

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
