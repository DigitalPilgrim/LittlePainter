QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    floattexture.cpp \
    image_manager.cpp \
    little_painter.cpp \
    main.cpp \
    mainwindow.cpp \
    painter_brush.cpp \
    painter_manager.cpp

HEADERS += \
    KolkoCasuSomStravil.h \
    floattexture.h \
    graphic_helper_functions.h \
    image_manager.h \
    little_painter.h \
    mainwindow.h \
    painter_brush.h \
    painter_manager.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Resources/texture1.png \
    Resources/texture2.png
