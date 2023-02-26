// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#ifndef PRINC_H
#define PRINC_H

#include "ui_princ.h"
#include "dialog.h"
class Princ : public QMainWindow, private Ui::Princ
{
    Q_OBJECT

public:
    explicit Princ(QWidget *parent = 0);

private:
    Ui::Princ *ui;
    Dialog m_dialog;
public slots:

protected slots:

private slots:
    void on_btn_showCam_clicked();
};

#endif // PRINC_H
