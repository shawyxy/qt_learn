QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    camera.cpp \
    main.cpp \
    mainwindow.cpp \
    serialport.cpp \
    timer.cpp

HEADERS += \
    camera.h \
    mainwindow.h \
    serialport.h \
    timer.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -L$$PWD/GaussRivipCore/CameraCore/install/C-x64-Windows-Debug/CameraCore/lib/ -lCameraCore

INCLUDEPATH += $$PWD/GaussRivipCore/CameraCore/install/C-x64-Windows-Debug/CameraCore/include/CameraCore
DEPENDPATH += $$PWD/GaussRivipCore/CameraCore/install/C-x64-Windows-Debug/CameraCore/include/CameraCore

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/GaussRivipCore/CameraCore/install/C-x64-Windows-Debug/CameraCore/lib/CameraCore.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/GaussRivipCore/CameraCore/install/C-x64-Windows-Debug/CameraCore/lib/libCameraCore.a

win32: LIBS += -L$$PWD/GaussRivipCore/CameraCore/install/C-x64-Windows-Release/CameraCore/lib/ -lCameraCore

INCLUDEPATH += $$PWD/GaussRivipCore/CameraCore/install/C-x64-Windows-Release/CameraCore/include/CameraCore
DEPENDPATH += $$PWD/GaussRivipCore/CameraCore/install/C-x64-Windows-Release/CameraCore/include/CameraCore

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/GaussRivipCore/CameraCore/install/C-x64-Windows-Release/CameraCore/lib/CameraCore.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/GaussRivipCore/CameraCore/install/C-x64-Windows-Release/CameraCore/lib/libCameraCore.a

win32: LIBS += -L$$PWD/GaussRivipCore/CommunicationCore/install/C-x64-Windows-Debug/CommunicationCore/lib/ -lCommunicationCore

INCLUDEPATH += $$PWD/GaussRivipCore/CommunicationCore/install/C-x64-Windows-Debug/CommunicationCore/include/CommunicationCore
DEPENDPATH += $$PWD/GaussRivipCore/CommunicationCore/install/C-x64-Windows-Debug/CommunicationCore/include/CommunicationCore

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/GaussRivipCore/CommunicationCore/install/C-x64-Windows-Debug/CommunicationCore/lib/CommunicationCore.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/GaussRivipCore/CommunicationCore/install/C-x64-Windows-Debug/CommunicationCore/lib/libCommunicationCore.a

win32: LIBS += -L$$PWD/GaussRivipCore/CommunicationCore/install/C-x64-Windows-Release/CommunicationCore/lib/ -lCommunicationCore

INCLUDEPATH += $$PWD/GaussRivipCore/CommunicationCore/install/C-x64-Windows-Release/CommunicationCore/include/CommunicationCore
DEPENDPATH += $$PWD/GaussRivipCore/CommunicationCore/install/C-x64-Windows-Release/CommunicationCore/include/CommunicationCore

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/GaussRivipCore/CommunicationCore/install/C-x64-Windows-Release/CommunicationCore/lib/CommunicationCore.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/GaussRivipCore/CommunicationCore/install/C-x64-Windows-Release/CommunicationCore/lib/libCommunicationCore.a
