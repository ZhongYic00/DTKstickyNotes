#-------------------------------------------------
#
# Project created by QtCreator 2020-07-18T16:44:04
#
#-------------------------------------------------

QT += core gui svg
greaterThan(QT_MAJOR_VERSION, 5): QT += widgets network
QT += dtkcore dtkgui dtkwidget

TARGET = StickyNotes
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS QT_NO_DEBUG_OUTPUT

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

#CONFIG += link_pkgconfig
#PKGCONFIG += dtkwidget

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    backend.cpp \
    editor.cpp \
    mdialog.cpp \
    ztextedit.cpp \
    zlistview.cpp \
    znote.cpp \
    zlist.cpp \
    fhqtreap.cpp \
    searchresult.cpp \
    searchwidget.cpp \
    stickywidget.cpp \
    transparentwidget.cpp \
    roundedwidgets.cpp \
    systemtray.cpp \
    daemon.cpp \
    zlistmodel.cpp

HEADERS += \
        mainwindow.h \
    backend.h \
    editor.h \
    mdialog.h \
    ztextedit.h \
    zlistview.h \
    znote.h \
    zlist.h \
    searchresult.h \
    searchwidget.h \
    stickywidget.h \
    transparentwidget.h \
    roundedwidgets.h \
    systemtray.h \
    daemon.h \
    zlistmodel.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

VERSION = 0.0.5-beta
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

DISTFILES += \
    mainwindow.qss \
    mainwindow.qss
