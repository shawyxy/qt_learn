#include "camera.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <QJsonArray>
#include <QString>
#include <QDebug>
#include <string>
#include <iostream>

#define LOG std::cout << __FILE__ << ":" << __LINE__

int Camera::create(int index, std::function<void(CameraHandle handle, unsigned int width, unsigned int height, unsigned int channel,
                                                 unsigned int type, unsigned int step, unsigned char* data, const char* info)> imageCall)
{
    qDebug() << "create()";

    // 创建相机管理对象
    cameraCore = new Gauss_CameraCore;
    // 构造相机信息Json
    QJsonObject deviceInfo;
    deviceInfo["DeviceType"] = (int)(GaussCameraDeviceType::HKCamera);
    QString id = ids.at(index).toString();
    deviceInfo["DeviceID"] = id;

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
    indexToCameraInfo[index].infoJson = newJson;
    qDebug() << id;
    // 创建相机设备, 获取句柄
    CameraHandle handle = cameraCore->Gauss_CameraCore_CreateCamera(indexToCameraInfo[index].infoJson);
    indexToCameraInfo[index].handle = handle;
    qDebug() << "index: " << index << " " << handle;

    int res = 0;
    // 设置图像回调
    if ((res = cameraCore->Gauss_CameraCore_SetImageReady(handle, imageCall)) < 0)
    {
        LOG;
        qDebug() << "Gauss_CameraCore_SetImageReady() failed: " << res;
    }

    return res;
}

QJsonArray Camera::getCameraList()
{
    qDebug() << "getCameraList()";

    if (ids.empty())
    {
        cameraCore->Gauss_CameraCore_FindCamera();
        std::string cameraListJson = cameraCore->Gauss_CameraCore_GetCameraList();
        // 转换
        QByteArray jsonData = QByteArray::fromStdString(cameraListJson);
        // 解析
        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        QJsonObject jsonObj = doc.object();
        // 获取相机ID列表
        ids = jsonObj["Gige"].toArray();
    }
    return ids;
}

int Camera::open(int index)
{
    qDebug() << "open()";
    int res = 0;
    // 打开相机
    if ((res = cameraCore->Gauss_CameraCore_OpenCamera(indexToCameraInfo[index].handle)) < 0)
    {
        LOG;
        qDebug() << "Gauss_CameraCore_OpenCamera() failed: " << res;
    }
    else
    {
        indexToCameraInfo[index].isOpened = true;
    }
    return res;
}

int Camera::take(int index)
{
    qDebug() << "take()";
    int res = 0;
    // 触发相机拍照
    if ((res = cameraCore->Gauss_CameraCore_TriggerOnce(indexToCameraInfo[index].handle, indexToCameraInfo[index].infoJson)) < 0)
    {
        LOG;
        qDebug() << "Gauss_CameraCore_TriggerOnce() failed: " << res;
    }
    return 0;
}

int Camera::close(int index)
{
    qDebug() << "close()";
    // 关闭相机
    if (cameraCore->Gauss_CameraCore_CloseCamera(indexToCameraInfo[index].handle) < 0)
    {
        LOG;
        qDebug() << "Gauss_CameraCore_CloseCamera() failed";
    }
    else
    {
        indexToCameraInfo[index].isOpened = false;
    }
    // if (indexToCameraInfo.contains(index))
    // {
    //     indexToCameraInfo.erase(indexToCameraInfo.find(index));
    // }
    if (_imgBuf_ptr)
    {
        free(_imgBuf_ptr);
        _imgBuf_ptr = nullptr;
    }
    return 0;
}

Camera::Camera()
{

}

Camera::~Camera()
{
    if (cameraCore)
    {
        delete cameraCore;
        cameraCore = nullptr;
    }
}
