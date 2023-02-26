#ifndef CYLINDRE_H
#define CYLINDRE_H

#include <QKeyEvent>
#include <QTimer>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#define PI 3.141592
class Cylindre :public QOpenGLWidget,
                protected QOpenGLFunctions
{
    Q_OBJECT

public:
    Cylindre();
    ~Cylindre();
    void stock_data(QVector3D v, QVector3D c, GLfloat r_cyl, GLfloat ep_cyl);
    void draw_cylindre(int m_posAttr, int m_colAttr, QOpenGLShaderProgram *program, QMatrix4x4 matrix, int m_matrixUniform,double cyl_rotate);
    void draw_cylindre2(int m_posAttr, int m_colAttr, QOpenGLShaderProgram *program, QMatrix4x4 matrix, int m_matrixUniform,double cyl_rotate);
    void draw_cylindre3(int m_posAttr, int m_colAttr, QOpenGLShaderProgram *program, QMatrix4x4 matrix, int m_matrixUniform,double cyl_rotate, GLfloat x_H, GLfloat y_H,GLfloat x_J, GLfloat y_J);
private:
    int nb_fac = 20;
    GLfloat angle_cyl = 360.0/nb_fac;
    GLfloat ep_cyl,r_cyl;
    GLfloat color[30];
    GLfloat vertices[30];

    int m_posAttr;
    int m_colAttr;
    int m_matrixUniform;

    GLfloat x,y,z;
    GLfloat col_r, col_g, col_b;
};

#endif // CYLINDRE_H
