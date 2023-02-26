#ifndef PEDALE_H
#define PEDALE_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
class Pedale
{
public:
    Pedale(GLfloat length, GLfloat width, GLfloat height, QVector3D color, GLfloat ratio);
    ~Pedale();
    void buildVertData(QVector<GLfloat> &data);
    void draw(QOpenGLShaderProgram *program, QOpenGLFunctions *glFuncs);
    void dessiner_pedale(QVector<GLfloat> &data, GLint signal);
    GLfloat get_width();
    GLfloat get_height();
private:
    QOpenGLBuffer m_vbo;
    GLfloat length;
    GLfloat width;
    GLfloat height;
    GLfloat p_x, p_y, p_z;
    QVector3D color_pedale;
    GLfloat coul_r = color_pedale.x();
    GLfloat coul_v = color_pedale.y();
    GLfloat coul_b = color_pedale.z();
    GLfloat ratio;
};

#endif // PEDALE_H
