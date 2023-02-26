#include "roue.h"
#include "qcolor.h"

Roue::Roue(GLfloat nb_dents, GLfloat ep_roue, GLfloat h_dent,GLfloat r_roue, QVector3D color):
    nb_dents(nb_dents), ep_roue(ep_roue), h_dent(h_dent), r_roue(r_roue), color_roue(color)
{
    m_vbo.create();
    m_vbo.bind();
    QVector<GLfloat> vertData;
    buildVertData(vertData);
    m_vbo.allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));
    m_vbo.release();
}

Roue::~Roue(){
    m_vbo.destroy();
}

void Roue:: dessiner_roue_plan(QVector<GLfloat> &data, GLint self_rotate, GLint signal){
    GLfloat z = signal * ep_roue * 0.5;
    QVector<QVector3D> vs = {
        QVector3D(0,0,z), //O
        QVector3D((r_roue-h_dent/2)*cos(angle_roue*self_rotate),
        (r_roue-h_dent/2)*sin(angle_roue*self_rotate), z), //B
        QVector3D((r_roue-h_dent/2)*cos(angle_roue*(self_rotate+0.25)),
        (r_roue-h_dent/2)*sin(angle_roue*(self_rotate+0.25)),  z),//C
        QVector3D((r_roue+h_dent/2)*cos(angle_roue*(self_rotate+0.5)),
        (r_roue+h_dent/2)*sin(angle_roue*(self_rotate+0.5)),  z),//D
        QVector3D((r_roue+h_dent/2)*cos(angle_roue*(self_rotate+0.75)),
        (r_roue+h_dent/2)*sin(angle_roue*(self_rotate+0.75)),  z), //E
        QVector3D((r_roue-h_dent/2)*cos(angle_roue*(self_rotate+1.0)),
        (r_roue-h_dent/2)*sin(angle_roue*(self_rotate+1.0)),  z), //F
    };
                   //O,B,C,D,E,F
    int ind_ver[] = {0,1,2,3,4,5};

    QColor color = QColor::fromRgbF(coul_r, coul_v, coul_b);

    QVector3D vOB = vs[1]-vs[0];
    QVector3D vOF = vs[5]-vs[0];

    QVector3D nOBF = QVector3D::normal(vOB, vOF);
    if(signal<0) nOBF = -nOBF;

    for(int i = 0; i < 6; ++i){
        data.append(vs[ind_ver[i]].x());
        data.append(vs[ind_ver[i]].y());
        data.append(vs[ind_ver[i]].z());

        data.append(color.redF());
        data.append(color.greenF());
        data.append(color.blueF());

        data.append(nOBF.x());
        data.append(nOBF.y());
        data.append(nOBF.z());
    }

}

void Roue:: dessiner_roue_facette(QVector<GLfloat> &data, GLint self_rotate){
    QVector<QVector3D> vs_BCCB= {
        //BCC'B'
        QVector3D((r_roue-h_dent/2)*cos(angle_roue*self_rotate),(r_roue-h_dent/2)*sin(angle_roue*self_rotate), ep_roue/2), //B
        QVector3D((r_roue-h_dent/2)*cos(angle_roue*(self_rotate+0.25)),(r_roue-h_dent/2)*sin(angle_roue*(self_rotate+0.25)), ep_roue/2), //C
        QVector3D((r_roue-h_dent/2)*cos(angle_roue*(self_rotate+0.25)),(r_roue-h_dent/2)*sin(angle_roue*(self_rotate+0.25)),  -ep_roue/2), //C'
        QVector3D((r_roue-h_dent/2)*cos(angle_roue*self_rotate),(r_roue-h_dent/2)*sin(angle_roue*self_rotate), -ep_roue/2), //B'
    };
    int ind_ver[] = {0, 1, 2, 3};

    QColor color = QColor::fromRgbF(0.8*coul_r, 0.8*coul_v, 0.8* coul_b);

    QVector3D vBC = vs_BCCB[0]-vs_BCCB[1],
              vBBp = vs_BCCB[0]-vs_BCCB[3];

    QVector3D nBCCpBp = QVector3D::normal(vBC,vBBp);

    for(int i = 0; i < 4;++i){
        data.append(vs_BCCB[ind_ver[i]].x());
        data.append(vs_BCCB[ind_ver[i]].y());
        data.append(vs_BCCB[ind_ver[i]].z());

        data.append(color.redF());
        data.append(color.greenF());
        data.append(color.blueF());

        data.append(nBCCpBp.x());
        data.append(nBCCpBp.y());
        data.append(nBCCpBp.z());
    }
    QVector<QVector3D> vs_CDDC= {
            //CDD'C'
            QVector3D((r_roue-h_dent/2)*cos(angle_roue*(self_rotate+0.25)),(r_roue-h_dent/2)*sin(angle_roue*(self_rotate+0.25)), ep_roue/2), //C
            QVector3D((r_roue+h_dent/2)*cos(angle_roue*(self_rotate+0.5)),(r_roue+h_dent/2)*sin(angle_roue*(self_rotate+0.5)),ep_roue/2), //D
            QVector3D((r_roue+h_dent/2)*cos(angle_roue*(self_rotate+0.5)),(r_roue+h_dent/2)*sin(angle_roue*(self_rotate+0.5)),-ep_roue/2), //D'
            QVector3D((r_roue-h_dent/2)*cos(angle_roue*(self_rotate+0.25)),(r_roue-h_dent/2)*sin(angle_roue*(self_rotate+0.25)),-ep_roue/2), //C'
    };
            QVector3D vCD = vs_CDDC[0]-vs_CDDC[1],
                      vCCp = vs_CDDC[0]-vs_CDDC[3];
            QVector3D nCDDpCp = QVector3D::normal(vCD,vCCp);
            for(int i = 0; i < 4;++i){
                data.append(vs_CDDC[ind_ver[i]].x());
                data.append(vs_CDDC[ind_ver[i]].y());
                data.append(vs_CDDC[ind_ver[i]].z());

                data.append(color.redF());
                data.append(color.greenF());
                data.append(color.blueF());

                data.append(nCDDpCp.x());
                data.append(nCDDpCp.y());
                data.append(nCDDpCp.z());
            }
    QVector<QVector3D> vs_DEED= {
        //DEE'D'
        QVector3D((r_roue+h_dent/2)*cos(angle_roue*(self_rotate+0.5)),(r_roue+h_dent/2)*sin(angle_roue*(self_rotate+0.5)),ep_roue/2), //D
        QVector3D((r_roue+h_dent/2)*cos(angle_roue*(self_rotate+0.75)),(r_roue+h_dent/2)*sin(angle_roue*(self_rotate+0.75)),ep_roue/2), //E
        QVector3D((r_roue+h_dent/2)*cos(angle_roue*(self_rotate+0.75)),(r_roue+h_dent/2)*sin(angle_roue*(self_rotate+0.75)),-ep_roue/2), //E'
        QVector3D((r_roue+h_dent/2)*cos(angle_roue*(self_rotate+0.5)),(r_roue+h_dent/2)*sin(angle_roue*(self_rotate+0.5)),-ep_roue/2), //D'
    };
            QVector3D vDE = vs_DEED[0]-vs_DEED[1],
                      vDDp = vs_DEED[0]-vs_DEED[3];
            QVector3D nDEEpDp = QVector3D::normal(vDE,vDDp);
            for(int i = 0; i < 4;++i){
                data.append(vs_DEED[ind_ver[i]].x());
                data.append(vs_DEED[ind_ver[i]].y());
                data.append(vs_DEED[ind_ver[i]].z());

                data.append(color.redF());
                data.append(color.greenF());
                data.append(color.blueF());

                data.append(nDEEpDp.x());
                data.append(nDEEpDp.y());
                data.append(nDEEpDp.z());
            }

    QVector<QVector3D> vs_EFFE= {
        //EFF'E'
        QVector3D((r_roue+h_dent/2)*cos(angle_roue*(self_rotate+0.75)),(r_roue+h_dent/2)*sin(angle_roue*(self_rotate+0.75)),ep_roue/2), //E
        QVector3D((r_roue-h_dent/2)*cos(angle_roue*(self_rotate+1.0)),(r_roue-h_dent/2)*sin(angle_roue*(self_rotate+1.0)),ep_roue/2), //F
        QVector3D((r_roue-h_dent/2)*cos(angle_roue*(self_rotate+1.0)),(r_roue-h_dent/2)*sin(angle_roue*(self_rotate+1.0)), -ep_roue/2), //F'
        QVector3D((r_roue+h_dent/2)*cos(angle_roue*(self_rotate+0.75)),(r_roue+h_dent/2)*sin(angle_roue*(self_rotate+0.75)),-ep_roue/2), //E'
    };
            QVector3D vEF = vs_EFFE[0]-vs_EFFE[1],
                      vEEp = vs_EFFE[0]-vs_EFFE[3];
            QVector3D nEFFpEp = QVector3D::normal(vEF,vEEp);
            for(int i = 0; i < 4;++i){
                data.append(vs_EFFE[ind_ver[i]].x());
                data.append(vs_EFFE[ind_ver[i]].y());
                data.append(vs_EFFE[ind_ver[i]].z());

                data.append(color.redF());
                data.append(color.greenF());
                data.append(color.blueF());

                data.append(nEFFpEp.x());
                data.append(nEFFpEp.y());
                data.append(nEFFpEp.z());
            }

}

void Roue:: buildVertData(QVector<GLfloat> &data){
    for(int i = 0; i < nb_dents; ++i){
        dessiner_roue_plan(data, i, 1);
        dessiner_roue_plan(data, i, -1);
        dessiner_roue_facette(data, i);
    }
}

void Roue::draw(QOpenGLShaderProgram *program, QOpenGLFunctions *glFuncs){
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


    int vertex_offset = 0;

    for (int i = 0; i < nb_dents; i++) {
        glFuncs->glDrawArrays(GL_POLYGON, vertex_offset, 6);
        glFuncs->glDrawArrays(GL_POLYGON, vertex_offset + 6, 6);
        glFuncs->glDrawArrays(GL_QUADS, vertex_offset + 12, 4);
        glFuncs->glDrawArrays(GL_QUADS, vertex_offset + 16, 4);
        glFuncs->glDrawArrays(GL_QUADS, vertex_offset + 20, 4);
        glFuncs->glDrawArrays(GL_QUADS, vertex_offset + 24, 4);
        vertex_offset += 28;
    }

    program->disableAttributeArray("posAttr");
    program->disableAttributeArray("colAttr");
    program->disableAttributeArray("norAttr");

    m_vbo.release();
}

GLfloat Roue::radius(){
    return r_roue;
}
GLfloat Roue::get_ep_roue(){
    return ep_roue;
}
GLfloat Roue::get_r_roue(){
    return r_roue;
}
