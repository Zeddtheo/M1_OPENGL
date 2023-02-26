#include "cylinder.h"
#include "qcolor.h"

Cylinder::Cylinder(GLfloat nb_fac, GLfloat r_cyl, GLfloat ep_cyl, QVector3D color_cyl, bool phong_shading):
    nb_fac(nb_fac), r_cyl(r_cyl), ep_cyl(ep_cyl), color_cyl(color_cyl), m_phong_shading(phong_shading)
{
    m_vbo.create();
    m_vbo.bind();
    QVector<GLfloat> vertData;
    buildVertData(vertData);
    m_vbo.allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));
    m_vbo.release();
}

Cylinder::~Cylinder(){
    m_vbo.destroy();
}

void Cylinder::dessiner_cyl_plan(QVector<GLfloat> &data, GLint self_rotate, GLint signal){
    QVector<QVector3D> vs = {
        QVector3D( 0, 0, signal*ep_cyl/2), //A
        QVector3D(r_cyl*cos(angle_cyl*self_rotate), r_cyl*sin(angle_cyl*self_rotate), signal*ep_cyl/2), //C
        QVector3D(r_cyl*cos(angle_cyl*(1+self_rotate)),  r_cyl*sin(angle_cyl*(1+self_rotate)),  signal*ep_cyl/2) //F
    };

                   // A  C  F
    int ind_ver[] = { 0, 1, 2 };

    QColor color = QColor::fromRgbF(coul_r, coul_v, coul_b);

    QVector3D vAC = vs[1]-vs[0],
              vAF = vs[2]-vs[0];

    QVector3D nACF = QVector3D::normal(vAC, vAF);

    if(signal < 0) nACF = -nACF;

    for (int i = 0; i < 3; ++i) {
        data.append(vs[ind_ver[i]].x());
        data.append(vs[ind_ver[i]].y());
        data.append(vs[ind_ver[i]].z());

        data.append(color.redF());
        data.append(color.greenF());
        data.append(color.blueF());

        data.append(nACF.x());
        data.append(nACF.y());
        data.append(nACF.z());
    }
}

void Cylinder::dessiner_cyl_facette(QVector<GLfloat> &data, GLint self_rotate){
    QVector<QVector3D> vs = {
        QVector3D(r_cyl*cos(angle_cyl*self_rotate), r_cyl*sin(angle_cyl*self_rotate), -ep_cyl/2), //D
        QVector3D(r_cyl*cos(angle_cyl*(1+self_rotate)),  r_cyl*sin(angle_cyl*(1+self_rotate)),  -ep_cyl/2), //E
        QVector3D(r_cyl*cos(angle_cyl*(1+self_rotate)),  r_cyl*sin(angle_cyl*(1+self_rotate)), ep_cyl/2), //F

        QVector3D(r_cyl*cos(angle_cyl*(1+self_rotate)),  r_cyl*sin(angle_cyl*(1+self_rotate)), ep_cyl/2), //F
        QVector3D(r_cyl*cos(angle_cyl*self_rotate), r_cyl*sin(angle_cyl*self_rotate), ep_cyl/2), //C
        QVector3D(r_cyl*cos(angle_cyl*self_rotate), r_cyl*sin(angle_cyl*self_rotate), -ep_cyl/2), //D
    };

                   // D  E  F  F  C  D
    int ind_ver[] = { 0, 1, 2, 3, 4, 5};

    QColor color = QColor::fromRgbF(0.6*coul_r, 0.6*coul_v, 0.6*coul_b);

    QVector3D vDE = vs[0]-vs[1],
              vDF = vs[0]-vs[2],
              vFC = vs[3]-vs[4],
              vFD = vs[3]-vs[5];

    QVector3D nDEF = QVector3D::normal(vDE, vDF);
    QVector3D nFCD = QVector3D::normal(vFC, vFD);

    QVector<QVector3D> normals;
    QVector<int> ind_nor;


     //lissage de phong
    if (m_phong_shading) {
       QVector3D nDF = (nDEF + nFCD) / 2.0;
       normals = { nDEF, nFCD, nDF};
       ind_nor = { 2, 0, 2, 2, 1, 2 };

    }else {
       normals = { nDEF, nFCD };
       ind_nor = { 0, 0, 0, 1, 1, 1 };
    }

    for (int i = 0; i < 6; ++i) {
           data.append(vs[ind_ver[i]].x());
           data.append(vs[ind_ver[i]].y());
           data.append(vs[ind_ver[i]].z());

           data.append(color.redF());
           data.append(color.greenF());
           data.append(color.blueF());

           data.append(normals[ind_nor[i]].x());
           data.append(normals[ind_nor[i]].y());
           data.append(normals[ind_nor[i]].z());
       }
}

void Cylinder::buildVertData(QVector<GLfloat> &data){
    for(int i = 0; i < nb_fac; ++i){
        dessiner_cyl_plan(data, i, 1);
        dessiner_cyl_plan(data, i, -1);
        dessiner_cyl_facette(data, i);
    }
}

void Cylinder::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *glFuncs){
    m_vbo.bind();

    program->setAttributeBuffer("posAttr",
        GL_FLOAT, 0 * sizeof(GLfloat), 3, 9 * sizeof(GLfloat));
    program->setAttributeBuffer("colAttr",
        GL_FLOAT, 3 * sizeof(GLfloat), 3, 9 * sizeof(GLfloat));
    program->setAttributeBuffer("norAttr",
        GL_FLOAT, 6 * sizeof(GLfloat), 3, 9 * sizeof(GLfloat));
    program->enableAttributeArray("posAttr");
    program->enableAttributeArray("colAttr");
    program->enableAttributeArray("norAttr");

    for(int i = 0; i< 4*nb_fac; i++){
        glFuncs->glDrawArrays(GL_TRIANGLES, i*3, 3);
    }

    program->disableAttributeArray("posAttr");
    program->disableAttributeArray("colAttr");
    program->disableAttributeArray("norAttr");

    m_vbo.release();
}
