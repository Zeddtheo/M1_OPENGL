#ifndef MAILLON_H
#define MAILLON_H
#include "cylinder.h"
#include "pedale.h"

class Maillon
{
public:
    Maillon(GLfloat ep_m, GLfloat r_cyl, GLfloat width, GLfloat height, QVector3D color, bool external);
    ~Maillon();
    Cylinder* getCylindre(int index);
    Pedale* getPedale(int index);
private:
    bool m_external = true;
    QOpenGLBuffer m_vbo;
    Cylinder *m_cylindre1 = nullptr;
    Cylinder *m_cylindre2 = nullptr;
    Pedale *m_pedale1 = nullptr;
    Pedale *m_pedale2 = nullptr;
    Pedale *m_pedale3 = nullptr;
    Pedale *m_pedale4 = nullptr;
    GLfloat ep_m;
    GLfloat r_cyl;
    GLfloat width;
    GLfloat height;

    QVector3D color_maillon;
};

#endif // MAILLON_H
