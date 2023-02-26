#include "dialog.h"
#include "ui_dialog.h"
#include "glarea.h"
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    glArea(new GLArea())
{
    ui->setupUi(this);
    ui->doubleSpinBox_z->setSingleStep(0.1);
    ui->doubleSpinBox_r->setSingleStep(0.1);
    ui->doubleSpinBox_n->setSingleStep(0.1);
    ui->doubleSpinBox_f->setSingleStep(0.1);
    ui->doubleSpinBox_a->setSingleStep(0.1);


    connect(glArea, SIGNAL(radiusChanged(double)),this,SLOT(set_doubleSpinBox_r_value(double)));
    connect(ui->doubleSpinBox_r,SIGNAL(valueChanged(double)),this,SLOT(on_doubleSpinBox_r_valueChanged(double)));

}
Dialog::~Dialog(){
    delete ui;
}

void Dialog::set_doubleSpinBox_r_value(double radius){
    radius = glArea->getRadius();
    qDebug()<<radius<<Qt::endl;
    ui->doubleSpinBox_r->setValue(radius);
}

void Dialog::on_doubleSpinBox_r_valueChanged(double radius)
{
    glArea->setRadius(radius);
}

