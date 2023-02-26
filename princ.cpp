// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#include "princ.h"
#include <QDebug>
#include "dialog.h"
#include "ui_princ.h"
Princ::Princ(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    Dialog m_dialog(this);

    connect(btn_quit, &QPushButton::clicked, qApp, &QApplication::quit);
    connect(btn_showCam, &QPushButton::clicked, this, &Princ::on_btn_showCam_clicked);

}



void Princ::on_btn_showCam_clicked()
{
    m_dialog.show();
}



