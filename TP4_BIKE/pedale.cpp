#include "pedale.h"
#include "qcolor.h"

Pedale::Pedale(GLfloat length, GLfloat width, GLfloat height, QVector3D color, GLfloat ratio):
    length(length),width(width),height(height),color_pedale(color), ratio(ratio)
{
    p_x = this->length*0.5;
    p_y = this->height*0.5;
    p_z = this->width*0.5;
    m_vbo.create();
    m_vbo.bind();
    QVector<GLfloat> vertData;
    buildVertData(vertData);
    m_vbo.allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));
    m_vbo.release();
}

Pedale::~Pedale(){
    m_vbo.destroy();
}

void Pedale::dessiner_pedale(QVector<GLfloat> &data, GLint signal){
    QColor color = QColor::fromRgbF(coul_r, coul_v, coul_b);
    QVector<QVector3D> vs_ABCDEFGH = {
        QVector3D(-p_x,-p_y*ratio,p_z*signal), //A
        QVector3D(-p_x*ratio,-p_y,p_z*signal), //B
        QVector3D(p_x*ratio,-p_y,p_z*signal), //C
        QVector3D(p_x,-p_y*ratio,p_z*signal), //D
        QVector3D(p_x,p_y*ratio,p_z*signal), //E
        QVector3D(p_x*ratio,p_y,p_z*signal), //F
        QVector3D(-p_x*ratio,p_y,p_z*signal), //G
        QVector3D(-p_x,p_y*ratio,p_z*signal), //H
    };
                  // A  B  C  D  E  F  G  H
    int ind_ver[] = {0, 1, 2, 3, 4, 5, 6, 7};
    QVector3D vAB = vs_ABCDEFGH[0] - vs_ABCDEFGH[1];
    QVector3D vAH = vs_ABCDEFGH[0] - vs_ABCDEFGH[7];

    QVector3D nABCDCDEFGH = QVector3D::normal(vAB, vAH);

    if(signal<0) nABCDCDEFGH = -nABCDCDEFGH;

    for(int i = 0; i< 8; ++i){
        data.append(vs_ABCDEFGH[ind_ver[i]].x());
        data.append(vs_ABCDEFGH[ind_ver[i]].y());
        data.append(vs_ABCDEFGH[ind_ver[i]].z());

        data.append(color.redF());
        data.append(color.greenF());
        data.append(color.blueF());

        data.append(nABCDCDEFGH.x());
        data.append(nABCDCDEFGH.y());
        data.append(nABCDCDEFGH.z());
    }
    QVector<QVector3D> vs_CDDpCp = {
        QVector3D(p_x*ratio*signal,-p_y,p_z), //C
        QVector3D(p_x*signal,-p_y*ratio,p_z), //D
        QVector3D(p_x*signal,-p_y*ratio,-p_z), //D'
        QVector3D(p_x*ratio*signal,-p_y,-p_z), //C'
    };

    QVector3D vCD = vs_CDDpCp[0] - vs_CDDpCp[1];
    QVector3D vCDp = vs_CDDpCp[0] - vs_CDDpCp[2];

    QVector3D nCDDpCp = QVector3D::normal(vCD, vCDp);

    if(signal<0) nCDDpCp = -nCDDpCp;

    for(int i = 0; i< 4; ++i){
        data.append(vs_CDDpCp[ind_ver[i]].x());
        data.append(vs_CDDpCp[ind_ver[i]].y());
        data.append(vs_CDDpCp[ind_ver[i]].z());

        data.append(color.redF());
        data.append(color.greenF());
        data.append(color.blueF());

        data.append(nCDDpCp.x());
        data.append(nCDDpCp.y());
        data.append(nCDDpCp.z());
    }

    QVector<QVector3D> vs_DEEpDp = {
        QVector3D(p_x*signal,-p_y*ratio,p_z), //D
        QVector3D(p_x*signal,p_y*ratio,p_z), //E
        QVector3D(p_x*signal,p_y*ratio,-p_z), //E'
        QVector3D(p_x*signal,-p_y*ratio,-p_z), //D'
    };

    QVector3D vDE = vs_DEEpDp[0] - vs_DEEpDp[1];
    QVector3D vDEp = vs_DEEpDp[0] - vs_DEEpDp[2];

    QVector3D nDEEpDp = QVector3D::normal(vDE, vDEp);

    if(signal<0) nDEEpDp = -nDEEpDp;

    for(int i = 0; i< 4; ++i){
        data.append(vs_DEEpDp[ind_ver[i]].x());
        data.append(vs_DEEpDp[ind_ver[i]].y());
        data.append(vs_DEEpDp[ind_ver[i]].z());

        data.append(color.redF());
        data.append(color.greenF());
        data.append(color.blueF());

        data.append(nDEEpDp.x());
        data.append(nDEEpDp.y());
        data.append(nDEEpDp.z());
    }

    QVector<QVector3D> vs_EFFpEp = {
        QVector3D(p_x*signal,p_y*ratio,p_z), //E
        QVector3D(p_x*ratio*signal,p_y,p_z), //F
        QVector3D(p_x*ratio*signal,p_y,-p_z), //F'
        QVector3D(p_x*signal,p_y*ratio,-p_z)  //E'
    };

    QVector3D vEF = vs_EFFpEp[0] - vs_EFFpEp[1];
    QVector3D vEFp = vs_EFFpEp[0] - vs_EFFpEp[2];

    QVector3D nEFFpEp = QVector3D::normal(vEF, vEFp);

    if(signal<0) nEFFpEp = -nEFFpEp;

    for(int i = 0; i< 4; ++i){
        data.append(vs_EFFpEp[ind_ver[i]].x());
        data.append(vs_EFFpEp[ind_ver[i]].y());
        data.append(vs_EFFpEp[ind_ver[i]].z());

        data.append(color.redF());
        data.append(color.greenF());
        data.append(color.blueF());

        data.append(nEFFpEp.x());
        data.append(nEFFpEp.y());
        data.append(nEFFpEp.z());
    }

    QVector<QVector3D> vs_FGGpFp = {
        QVector3D(p_x*ratio,p_y*signal,p_z), //F
        QVector3D(-p_x*ratio,p_y*signal,p_z), //G
        QVector3D(-p_x*ratio,p_y*signal,-p_z), //G'
        QVector3D(p_x*ratio,p_y*signal,-p_z), //F'
    };

    QVector3D vFG = vs_FGGpFp[0] - vs_FGGpFp[1];
    QVector3D vFGp = vs_FGGpFp[0] - vs_FGGpFp[2];

    QVector3D nFGGpFp = QVector3D::normal(vFG, vFGp);

    if(signal<0) nFGGpFp = -nFGGpFp;

    for(int i = 0; i< 4; ++i){
        data.append(vs_FGGpFp[ind_ver[i]].x());
        data.append(vs_FGGpFp[ind_ver[i]].y());
        data.append(vs_FGGpFp[ind_ver[i]].z());

        data.append(color.redF());
        data.append(color.greenF());
        data.append(color.blueF());

        data.append(nFGGpFp.x());
        data.append(nFGGpFp.y());
        data.append(nFGGpFp.z());
    }
}

void Pedale::buildVertData(QVector<GLfloat> &data){
    dessiner_pedale(data, 1);
    dessiner_pedale(data, -1);
}

void Pedale::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *glFuncs){
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

    glFuncs->glDrawArrays(GL_POLYGON,0,8);
    glFuncs->glDrawArrays(GL_QUADS,8,4);
    glFuncs->glDrawArrays(GL_QUADS,12,4);
    glFuncs->glDrawArrays(GL_QUADS,16,4);
    glFuncs->glDrawArrays(GL_QUADS,20,4);
    glFuncs->glDrawArrays(GL_POLYGON,24,8);
    glFuncs->glDrawArrays(GL_QUADS,32,4);
    glFuncs->glDrawArrays(GL_QUADS,36,4);
    glFuncs->glDrawArrays(GL_QUADS,40,4);
    glFuncs->glDrawArrays(GL_QUADS,44,4);

    program->disableAttributeArray("posAttr");
    program->disableAttributeArray("colAttr");
    program->disableAttributeArray("norAttr");

    m_vbo.release();
}

GLfloat Pedale::get_width(){
    return width;
}

GLfloat Pedale::get_height(){
    return height;
}
