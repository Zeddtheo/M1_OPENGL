#ifndef ROUE_H
#define ROUE_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
class Roue
{
public:
    Roue(GLfloat nb_dents, GLfloat ep_roue, GLfloat h_dent,GLfloat r_roue, QVector3D color);
    ~Roue();
    GLfloat radius();
    void buildVertData(QVector<GLfloat> &data);
    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *glFuncs);
    void dessiner_roue_plan(QVector<GLfloat> &data, GLint self_rotate, GLint signal);
    void dessiner_roue_facette(QVector<GLfloat> &data, GLint self_rotate);
    GLfloat get_r_roue();
    GLfloat get_ep_roue();
private:
    QOpenGLBuffer m_vbo;
    GLfloat nb_dents;
    GLfloat angle_roue = 360.0/nb_dents*M_PI/180;
    GLfloat ep_roue;
    GLfloat h_dent;
    GLfloat r_roue;
    QVector3D color_roue;
    GLfloat coul_r = color_roue.x();
    GLfloat coul_v = color_roue.y();
    GLfloat coul_b = color_roue.z();
};

#endif // ROUE_H
