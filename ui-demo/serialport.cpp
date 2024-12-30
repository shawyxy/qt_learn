#include "serialport.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <iostream>

#define LOG std::cout << __FILE__ << ":" << __LINE__

SerialPort::SerialPort() {}

SerialPort::~SerialPort() {
  if (commCore) {
    delete commCore;
    commCore = nullptr;
  }
}

void SerialPort::debug(int handle) {
  unsigned int errnu = commCore->Gauss_CommunicationCore_GetLastError(handle);
  qDebug() << "error: " << QString("%1").arg(errnu, 8, 16);
  LOG;
}

int SerialPort::create(int index, std::string &portName,
                       std::function<void(const ComHandle handle,
                                          const char *msg, unsigned int len)>
                           recvCall) {
  qDebug() << "create(): " << index;
  commCore = new Gauss_CommunicationCore();
  // 构造串口信息Json
  QJsonObject serialPortJson;
  serialPortJson["DeviceType"] = 5;
  serialPortJson["Protocol"] = 0;
  // serialPortJson["LuaFilePath"] = "";
  serialPortJson["PortName"] = portName.c_str();
  serialPortJson["BaudRate"] = 115200;
  serialPortJson["DataBit"] = 8;
  serialPortJson["StopBit"] = 1;
  serialPortJson["ParityBit"] = 0;
  serialPortJson["FlowControl"] = 0;
  QJsonDocument doc(serialPortJson);
  std::string serialPortInfo = doc.toJson(QJsonDocument::Compact).toStdString();

  // 创建设备
  int handle = commCore->Gauss_CommunicationCore_CreateDevice(serialPortInfo);
  indexToPortInfo[index] = {handle, portName, false};
  handleToPortName[handle] = portName;
  std::cout << "index: " << index << ", handle created: " << handle
            << ", portName: " << portName << std::endl;

  // 设置回调函数
  int res = 0;
  res =
      commCore->Gauss_CommunicationCore_SetReadyReadCallBack(handle, recvCall);
  if (res != 0) {
    debug(handle);
  }
  return res;
}

int SerialPort::getSerialPorts() {
  qDebug() << "getSerialPorts()";
  int res = 0;
  for (auto &info : QSerialPortInfo::availablePorts()) {
    ports.push_back(info.portName().toStdString());
  }

  return res;
}

int SerialPort::open(int index) {
  qDebug() << "open(), index: " << index << ", handle"
           << indexToPortInfo[index].handle;
  int res = commCore->Gauss_CommunicationCore_OpenDevice(
      indexToPortInfo[index].handle);
  if (res != 0) {
    qDebug() << "SerialPort::open res: " << res;
    debug(indexToPortInfo[index].handle);
  } else {
    indexToPortInfo[index].isOpened = true;
  }

  return res;
}

int SerialPort::send(int index, const std::string &data) {
  // qDebug() << "-----------------------------";
  // std::cout << indexToPortInfo[index].portName << " send(): " << data
  //           << std::endl;
  QJsonObject obj;
  obj["data"] = data.c_str();
  QJsonDocument doc(obj);
  std::string jsonData = doc.toJson(QJsonDocument::Compact).toStdString();

  int res = 0;
  if ((res = commCore->Gauss_CommunicationCore_WriteAsyn(
           indexToPortInfo[index].handle, jsonData)) < 0) {
    qDebug() << "Gauss_CommunicationCore_WriteAsyn failed";
    debug(indexToPortInfo[index].handle);
  }

  // std::cout << jsonData << std::endl;
  // qDebug() << "SerialPort::send res:" << res;
  return res;
}

int SerialPort::recv(const ComHandle handle, const char *data,
                     unsigned int length) {
  int res = 0;

  std::string msg(data, length);
  std::cout << "recv: " << msg << std::endl;

  return res;
}

int SerialPort::close(int index) {
  int res = 0;

  qDebug() << "close(): " << index;
  if ((res = commCore->Gauss_CommunicationCore_CloseDevice(
           indexToPortInfo[index].handle)) < 0) {
    qDebug() << "Gauss_CommunicationCore_CloseDevice";
    debug(indexToPortInfo[index].handle);
  } else {
    indexToPortInfo[index].isOpened = false;
  }
  return res;
}
