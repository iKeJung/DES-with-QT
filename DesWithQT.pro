#-------------------------------------------------
#
# Project created by QtCreator 2018-09-23T14:06:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DesWithQT
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    desqt.cpp \
    tableslocation.cpp

HEADERS += \
        mainwindow.h \
    desqt.h \
    tableslocation.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Tables/expandword.txt \
    Tables/keyshift.txt \
    Tables/permute1.txt \
    Tables/permute2.txt \
    Tables/permutekey1.txt \
    Tables/permutekey2.txt \
    Tables/permutesbox.txt \
    Tables/s1.txt \
    Tables/s2.txt \
    Tables/s3.txt \
    Tables/s4.txt \
    Tables/s5.txt \
    Tables/s6.txt \
    Tables/s7.txt \
    Tables/s8.txt
