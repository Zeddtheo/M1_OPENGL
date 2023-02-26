#include "cylindre.h"
#include <QSurfaceFormat>
#include <QMatrix4x4>
#include <qmath.h>
#include <QDebug>
#include <QtOpenGL>
Cylindre::Cylindre()
{
    initializeOpenGLFunctions();
}
Cylindre::~Cylindre(){

}

void Cylindre::stock_data(QVector3D v, QVector3D c, GLfloat r_cyl, GLfloat ep_cyl){

    x = v.x();
    y = v.y();
    z = v.z();

    col_r = c.x();
    col_g = c.y();
    col_b = c.z();

    vertices[0] = 0.0; vertices[1] = 0.0; vertices[2] = ep_cyl/2; //A
    vertices[3] = r_cyl; vertices[4] = 0.0; vertices[5] = ep_cyl/2; //C
    vertices[6] = r_cyl*cos(angle_cyl*PI/180); vertices[7] = r_cyl*sin(angle_cyl*PI/180); vertices[8] = ep_cyl/2; //F

    vertices[9] = 0.0; vertices[10] = 0.0; vertices[11] = -ep_cyl/2; //B
    vertices[12] = r_cyl; vertices[13] = 0.0; vertices[14] = -ep_cyl/2; //D
    vertices[15] = r_cyl*cos(angle_cyl*PI/180); vertices[16] = r_cyl*sin(angle_cyl*PI/180); vertices[17] = -ep_cyl/2; //E

    vertices[18] = r_cyl; vertices[19] = 0.0; vertices[20] = ep_cyl/2; //C
    vertices[21] = r_cyl*cos(angle_cyl*PI/180); vertices[22] = r_cyl*sin(angle_cyl*PI/180); vertices[23] = ep_cyl/2; //F
    vertices[24] = r_cyl*cos(angle_cyl*PI/180); vertices[25] = r_cyl*sin(angle_cyl*PI/180); vertices[26] = -ep_cyl/2; //E
    vertices[27] = r_cyl; vertices[28] = 0.0; vertices[29] = -ep_cyl/2; //D


    color[0] = 1.0*col_r; color[1] = 1.0*col_g; color[2] = 1.0*col_b;
    color[3] = 1.0*col_r; color[4] = 1.0*col_g; color[5] = 1.0*col_b;
    color[6] = 1.0*col_r; color[7] = 1.0*col_g; color[8] = 1.0*col_b;

    color[9] = 1.0*col_r; color[10] = 1.0*col_g; color[11] = 1.0*col_b;
    color[12] = 1.0*col_r; color[13] = 1.0*col_g; color[14] = 1.0*col_b;
    color[15] = 1.0*col_r; color[16] = 1.0*col_g; color[17] = 1.0*col_b;

    color[18] = 0.7*col_r; color[19] = 0.8*col_g; color[20] = 0.8*col_b;
    color[21] = 0.7*col_r; color[22] = 0.8*col_g; color[23] = 0.8*col_b;
    color[24] = 0.7*col_r; color[25] = 0.8*col_g; color[26] = 0.8*col_b;
    color[27] = 0.7*col_r; color[28] = 0.8*col_g; color[29] = 0.8*col_b;


}

void Cylindre::draw_cylindre(int m_posAttr, int m_colAttr, QOpenGLShaderProgram *program, QMatrix4x4 matrix, int m_matrixUniform, double cyl_rotate){
    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, color);
    glEnableVertexAttribArray(m_posAttr);
    glEnableVertexAttribArray(m_colAttr);

    matrix.translate(x,y,z);
    program->setUniformValue(m_matrixUniform, matrix);
    matrix.rotate(cyl_rotate,0,0,1);
    program->setUniformValue(m_matrixUniform, matrix);

    for(int i = 0; i< nb_fac ;i++){
        matrix.rotate(angle_cyl,0,0,1.0);
        glDrawArrays(GL_POLYGON, 0, 3);
        glDrawArrays(GL_POLYGON, 3, 3);
        glDrawArrays(GL_QUADS, 6, 4);
        program->setUniformValue(m_matrixUniform, matrix);
    }
    glDisableVertexAttribArray(m_posAttr);
    glDisableVertexAttribArray(m_colAttr);
}

void Cylindre::draw_cylindre2(int m_posAttr, int m_colAttr, QOpenGLShaderProgram *program, QMatrix4x4 matrix, int m_matrixUniform,double cyl_rotate){
    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, color);
    glEnableVertexAttribArray(m_posAttr);
    glEnableVertexAttribArray(m_colAttr);

    matrix.rotate(90,0,1,0);
    program->setUniformValue(m_matrixUniform, matrix);
    matrix.translate(x,y,z);
    program->setUniformValue(m_matrixUniform, matrix);
    matrix.rotate(cyl_rotate,0,0,1);
    program->setUniformValue(m_matrixUniform, matrix);
    for(int i = 0; i< nb_fac ;i++){
        matrix.rotate(angle_cyl,0,0,1.0);
        glDrawArrays(GL_POLYGON, 0, 3);
        glDrawArrays(GL_POLYGON, 3, 3);
        glDrawArrays(GL_QUADS, 6, 4);
        program->setUniformValue(m_matrixUniform, matrix);
    }
    glDisableVertexAttribArray(m_posAttr);
    glDisableVertexAttribArray(m_colAttr);
}

void Cylindre::draw_cylindre3(int m_posAttr, int m_colAttr, QOpenGLShaderProgram *program, QMatrix4x4 matrix, int m_matrixUniform,double cyl_rotate, GLfloat x_H, GLfloat y_H, GLfloat x_J, GLfloat y_J){
    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, color);
    glEnableVertexAttribArray(m_posAttr);
    glEnableVertexAttribArray(m_colAttr);

    matrix.translate((x_H+x_J)*0.5,(y_H+y_J)*0.5,0.5);
    matrix.rotate((std::atan(y_H/(std::abs(x_J-x_H)))*180/PI),0,0,1);
    matrix.rotate(90,0,1,0);
    matrix.translate(-(x_H+x_J)*0.5,-(y_H+y_J)*0.5,-0.5);
    matrix.translate(x,y,z);
    matrix.rotate(cyl_rotate,0,0,1);
    program->setUniformValue(m_matrixUniform, matrix);
    for(int i = 0; i< nb_fac ;i++){
        matrix.rotate(angle_cyl,0,0,1.0);
        glDrawArrays(GL_POLYGON, 0, 3);
        glDrawArrays(GL_POLYGON, 3, 3);
        glDrawArrays(GL_QUADS, 6, 4);
        program->setUniformValue(m_matrixUniform, matrix);
    }
    glDisableVertexAttribArray(m_posAttr);
    glDisableVertexAttribArray(m_colAttr);
}
