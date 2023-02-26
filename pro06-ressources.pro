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
        cylindre.cpp \
        dialog.cpp \
        princ.cpp \
        glarea.cpp


HEADERS  += princ.h \
        cylindre.h \
        dialog.h \
        glarea.h

FORMS    += princ.ui \
    dialog.ui

RESOURCES += \
    pro06-ressources.qrc
