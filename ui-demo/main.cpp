#include "mainwindow.h"

// 顺序：9->1
// 原始坐标
// (122, 82)  (139, 82)  (159, 82)
// (122, 135) (139, 135) (159, 135)
// (122, 189) (139, 189) (159, 189)

// 原始减去中心
// (86, -16)  (103, -16)  (123, -16)
// (86, 37) (103, 37) (123, 37)
// (86, 91) (103, 91) (123, 91)

// 相机中心
// (36, 98, 70)
#include <QApplication>
#include <thread>

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Json::Value readJsonFile2(const std::string &path) {
//   std::ifstream ifile(path);
//   // ifile.open(path);
//   Json::CharReaderBuilder ReaderBuilder;
//   ReaderBuilder["emitUTF8"] = true;
//   Json::Value root;
//   std::string strerr;
//   if (!parseFromStream(ReaderBuilder, ifile, &root, &strerr))
//     std::cerr << path << " json parse fail." << std::endl;
//   ifile.close();
//   return root;
// }

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  return a.exec();
}
