QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    file_manager.cpp \
    floattexture.cpp \
    graphic_helper_functions.cpp \
    image_manager.cpp \
    little_painter.cpp \
    main.cpp \
    mainwindow.cpp \
    painter_brush.cpp \
    painter_brush_circle.cpp \
    painter_brush_debug.cpp \
    painter_brush_texture.cpp \
    painter_canvas.cpp \
    painter_manager.cpp \
    undo_redo_basic.cpp \
    undo_redo_file.cpp \
    undo_redo_special.cpp \
    undo_redo_system.cpp

HEADERS += \
    KolkoCasuSomStravil.h \
    file_manager.h \
    floattexture.h \
    graphic_helper_functions.h \
    image_manager.h \
    little_painter.h \
    mainwindow.h \
    painter_brush.h \
    painter_brush_circle.h \
    painter_brush_debug.h \
    painter_brush_texture.h \
    painter_canvas.h \
    painter_manager.h \
    timer.h \
    undo_redo_basic.h \
    undo_redo_file.h \
    undo_redo_special.h \
    undo_redo_system.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Resources/texture1.png \
    Resources/texture2.png

#RESOURCES +=

