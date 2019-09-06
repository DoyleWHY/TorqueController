#-------------------------------------------------
#
# Project created by QtCreator 2019-08-22T15:52:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = torqueControlTest1
TEMPLATE = app


DEFINES += QT_DEPRECATED_WARNINGS


CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    eposcontroller.cpp \
    qcustomplot.cpp \
    torquecontroller.cpp \
    filter.cpp

HEADERS += \
        mainwindow.h \
    eposcontroller.h \
    logger_robo.h \
    Definitions.h \
    qcustomplot.h \
    torquecontroller.h \
    filter.h

FORMS += \
        mainwindow.ui

unix:!macx: LIBS += -L$$PWD/lib/ -lEposCmd -lftd2xx

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
