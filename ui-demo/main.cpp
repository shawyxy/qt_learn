#include "mainwindow.h"

#include <QApplication>
#include <thread>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  return a.exec();
}
// 9->1
// (122, 82)  (140, 82)  (159, 82)
// (122, 135) (140, 135) (159, 135)
// (122, 188) (140, 188) (159, 188)

// 相机中心
// (36, 98)
