#
# QtCreator project
#

QT += core gui widgets

equals     (QT_MAJOR_VERSION, 5): QT += opengl
greaterThan(QT_MAJOR_VERSION, 5): QT += openglwidgets

CONFIG += c++17

unix {
    LIBS += -lGLU
}
win32 {
    LIBS += -lGLU32 -lOpengl32
}

TARGET = runme
TEMPLATE = app

SOURCES += main.cpp\
        cylinder.cpp \
        maillon.cpp \
        pedale.cpp \
        princ.cpp \
        glarea.cpp \
        roue.cpp

HEADERS  += princ.h \
        cylinder.h \
        glarea.h \
        maillon.h \
        pedale.h \
        roue.h

FORMS    += princ.ui \
    dialog.ui

RESOURCES += \
    pro10-kite.qrc
