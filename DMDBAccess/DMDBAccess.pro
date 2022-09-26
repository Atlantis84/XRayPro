QT += core gui
QT += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TEMPLATE = lib
DEFINES += DMDBACCESS_LIBRARY

CONFIG += c++11
CONFIG += plugin

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dbaccessimpl.cpp \
    dmdbaccessinterface.cpp

HEADERS += \
    DMDBAccess_global.h \
    dbaccessimpl.h \
    dmdbaccessinterface.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
