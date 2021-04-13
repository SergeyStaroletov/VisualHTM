#-------------------------------------------------
#
# Project created by QtCreator 2014-04-14T16:21:22
#
#-------------------------------------------------

QT      += core gui opengl
LIBS    += -lGLU

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VisualHTM
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    GLWidget.cpp \
    CRegionDraw.cpp \
    OptionsDialog.cpp \
    CRegion.cpp \
    ScaleDialog.cpp

HEADERS  += MainWindow.h \
    GLWidget.h \
    Defines.h \
    CRegionDraw.h \
    OptionsDialog.h \
    CRegion.h \
    ScaleDialog.h

FORMS    += MainWindow.ui \
    OptionsDialog.ui \
    ScaleDialog.ui

OTHER_FILES += \
    ../VisualHTM-build-desktop-Qt_4_8_2_in_PATH__System__Release/input_binary \
    ../VisualHTM-build-desktop-Qt_4_8_2_in_PATH__System__Release/input_text \
    ../VisualHTM-build-desktop-Qt_4_8_2_in_PATH__System__Release/backup.txt
