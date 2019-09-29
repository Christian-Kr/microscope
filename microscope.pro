#-------------------------------------------------
#
# Project created by QtCreator 2019-09-28T21:29:57
#
#-------------------------------------------------

QT += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = microscope
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += \
    src/main.cpp \
    src/mainwin.cpp \
    src/controller.cpp \
    src/imagepreview.cpp \
    src/livecamera.cpp \
    src/stitchingwidget.cpp

HEADERS += \
    src/mainwin.hpp \
    src/controller.hpp \
    src/imagepreview.hpp \
    src/livecamera.hpp \
    src/stitchingwidget.hpp

FORMS += \
    ui/mainwin.ui \
    ui/about.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!macx {
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
}
