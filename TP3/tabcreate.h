#ifndef TABCREATE_H
#define TABCREATE_H

#include <GL/gl.h>
#include <QWidget>

namespace Ui{
class TabCreate;
}
class TabCreate : public QWidget
{
    Q_OBJECT
public:
    explicit TabCreate(QWidget *parent);
    ~TabCreate();

private:
    Ui::TabCreate *ui;
    float radius;
    float near;
    float far;
    float distance;
    float angle;
};

#endif // TABCREATE_H
