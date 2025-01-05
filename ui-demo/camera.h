#ifndef CAMERA_H
#define CAMERA_H

#include "CameraCore.h"
#include "CameraDefault/CameraDefault.h"
#include <QJsonArray>
#include <QVector>

struct cameraInfo {
  std::string infoJson;
  CameraHandle handle;
  bool isOpened = false;
};

class Camera : public QObject {
  Q_OBJECT
public:
  Gauss_CameraCore *cameraCore;
  QMap<int, cameraInfo> indexToCameraInfo;
  QMap<CameraHandle, std::string> handleToPortName;

  unsigned char *_imgBuf_ptr = nullptr;
  QJsonArray ids;

public:
  int create(int index,
             std::function<void(CameraHandle handle, unsigned int width,
                                unsigned int height, unsigned int channel,
                                unsigned int type, unsigned int step,
                                unsigned char *data, const char *info)>
                 imageCall);

  QJsonArray getCameraList();

  int open(int index);

  int take(int index);

  int close(int index);

public:
  Camera();
  ~Camera();
};

#endif // CAMERA_H
