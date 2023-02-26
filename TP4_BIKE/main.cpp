// CC-BY Edouard.Thiel@univ-amu.fr - 31/01/2022

#include "princ.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Princ w;
    w.show();

    return a.exec();
}
