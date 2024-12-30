#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <CommunicationCore.h>
#include <CommunicationDefault/CommunicationDefault.h>
#include <QMap>
#include <QtSerialPort/QSerialPortInfo>
#include <string>

struct PortInfo {
  ComHandle handle;
  std::string portName;
  bool isOpened = false;
};

class SerialPort : public QObject {
  Q_OBJECT
public:
  Gauss_CommunicationCore *commCore;
  std::vector<std::string> ports;
  QMap<int, PortInfo> indexToPortInfo;
  QMap<ComHandle, std::string> handleToPortName;

public:
  SerialPort();
  ~SerialPort();

  void debug(int handle);

  int create(int index, std::string &portName,
             std::function<void(const ComHandle handle, const char *msg,
                                unsigned int len)>
                 recvCall);

  int getSerialPorts();

  int open(int index);

  int send(int index, const std::string &data);

  int recv(const ComHandle handle, const char *data, unsigned int length);

  int close(int index);
};

#endif // SERIALPORT_H
