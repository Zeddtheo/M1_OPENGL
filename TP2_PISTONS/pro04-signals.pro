#
# QtCreator project
#

QT += core gui widgets

equals     (QT_MAJOR_VERSION, 5): QT += opengl
greaterThan(QT_MAJOR_VERSION, 5): QT += openglwidgets

CONFIG += c++14

unix {
    LIBS += -lGLU
    TARGET = runme
}
win32 {
    LIBS += -lGLU32 -lOpengl32
    TARGET = runme.exe
}

TEMPLATE = app


SOURCES += main.cpp\
        princ.cpp \
        glarea.cpp

HEADERS  += princ.h \
        glarea.h

FORMS    += princ.ui
