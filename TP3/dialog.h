#ifndef DIALOG_H
#define DIALOG_H

#include "qdialog.h"
#include "ui_dialog.h"
#include "glarea.h"

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
public slots:
    void set_doubleSpinBox_r_value(double radius);
private slots:
    void on_doubleSpinBox_r_valueChanged(double arg1);
private:
    Ui::Dialog *ui;
    GLArea *glArea;
};

#endif // DIALOG_H
