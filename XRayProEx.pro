#-------------------------------------------------
#
# Project created by QtCreator 2022-07-29T16:09:37
#
#-------------------------------------------------

QT       += core gui sql network serialport axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = XRayProEx
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

DEFINES += QSLOG_IS_SHARED_LIBRARY_IMPORT
DEFINES += QS_LOG_LINE_NUMBERS
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++14
QMAKE_CXXFLAGS_RELEASE = -Od -ZI -MD
QMAKE_LFLAGS_RELEASE = /DEBUG /INCREMENTAL:NO

SOURCES += \
        aboutwgt.cpp \
        centralwgt.cpp \
        circlewidget.cpp \
        datacommserial.cpp \
        dynamiccirclewgt.cpp \
        elecmanualwgt.cpp \
        equipinfowgt.cpp \
        gdatafactory.cpp \
        imessagebox.cpp \
        leftwidget.cpp \
        main.cpp \
        mainwindow.cpp \
        monitorthread.cpp \
        paraconfigwgt.cpp \
        pixmapwgt.cpp \
        processinfowgt.cpp \
        processpixmapwgt.cpp \
        productmanagewgt.cpp \
        productstyledlg.cpp \
        split.cpp \
        templateconfigwgt.cpp \
        testwindow.cpp \
        title/TitleBar.cpp \
        title/titlewidget.cpp \
        topwidget.cpp \
        udpservice.cpp \
        visiontemplatewgt.cpp \
        waitcountwgt.cpp \
        welcomedlg.cpp \
        zoommoveobj.cpp

HEADERS += \
        BarCodeDecoding.h \
        CameraInterface.h \
        MvCamera.h \
        TemplateBasedMethod.h \
        aboutwgt.h \
        centralwgt.h \
        circlewidget.h \
        datacommserial.h \
        dynamiccirclewgt.h \
        elecmanualwgt.h \
        equipinfowgt.h \
        gdatafactory.h \
        imessagebox.h \
        leftwidget.h \
        mainwindow.h \
        monitorthread.h \
        paraconfigwgt.h \
        pixmapwgt.h \
        processinfowgt.h \
        processpixmapwgt.h \
        productmanagewgt.h \
        productstyledlg.h \
        split.h \
        templateconfigwgt.h \
        testwindow.h \
        title/TitleBar.h \
        title/titlewidget.h \
        topwidget.h \
        udpservice.h \
        visiontemplatewgt.h \
        waitcountwgt.h \
        welcomedlg.h \
        zoommoveobj.h

INCLUDEPATH += .\xrayinclude
INCLUDEPATH += .\dbandqsloginclude
INCLUDEPATH += .\Common
INCLUDEPATH += .\include
INCLUDEPATH += .\camera
INCLUDEPATH += .\include\halconcpp
INCLUDEPATH += .\DMDBAccess


LIBS += -L$$PWD\lib\x64\ -lopencv_world455
LIBS += -L$$PWD\lib\x64\ -lQsLog2
LIBS += -L$$PWD\lib\x64\ -lDMDBAccess
LIBS += -L$$PWD\lib\x64\ -liRayBase
LIBS += -L$$PWD\lib\x64\ -lMvCameraControl
LIBS += -L$$PWD\lib\x64\ -lMVS_interface_dll
LIBS += -L$$PWD\lib\x64\ -lBarcode_detection
LIBS += -L$$PWD\lib\x64\ -lhalconcpp
LIBS += -L$$PWD\lib\x64\ -lhalcon
LIBS += -L$$PWD\lib\x64\ -lMATCH
LIBS += User32.LIB
LIBS += Gdi32.LIB

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

win32: LIBS += -lWinMM
