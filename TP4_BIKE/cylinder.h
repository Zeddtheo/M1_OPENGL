#ifndef CYLINDER_H
#define CYLINDER_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
class Cylinder
{
public:
    Cylinder(GLfloat nb_fac, GLfloat r_cyl, GLfloat ep_cyl, QVector3D color_cyl, bool phong_shading);
    ~Cylinder();
    GLfloat radius();
    void buildVertData(QVector<GLfloat> &data);
    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *glFuncs);
    void dessiner_cyl_plan(QVector<GLfloat> &data, GLint self_rotate, GLint signal);
    void dessiner_cyl_facette(QVector<GLfloat> &data, GLint self_rotate);
private:
    QOpenGLBuffer m_vbo;
    GLfloat nb_fac;
    GLfloat r_cyl;
    GLfloat ep_cyl;
    QVector3D color_cyl;
    GLfloat coul_r = color_cyl.x();
    GLfloat coul_v = color_cyl.y();
    GLfloat coul_b = color_cyl.z();
    bool m_phong_shading = true;
    GLfloat angle_cyl = 360.0/nb_fac*M_PI/180;
};

#endif // CYLINDER_H
