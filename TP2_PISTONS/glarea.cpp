// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#include "glarea.h"
#include <GL/glu.h>
#include <QDebug>
#include <QSurfaceFormat>
#include <qmath.h>
#include <cmath>
#include <iostream>
GLArea::GLArea(QWidget *parent) :
    QOpenGLWidget(parent)
{
    qDebug() << "init GLArea" ;
    // Ce n'est pas indispensable
    QSurfaceFormat sf;
    sf.setDepthBufferSize(24);
    setFormat(sf);
    qDebug() << "Depth is"<< format().depthBufferSize();

    setEnabled(true);  // événements clavier et souris
    setFocusPolicy(Qt::StrongFocus); // accepte focus
    setFocus();                      // donne le focus

    m_timer = new QTimer(this);
    m_timer->setInterval(50);  // msec
    connect (m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    connect (this, SIGNAL(radiusChanged(double)), this, SLOT(setRadius(double)));
}

GLArea::~GLArea()
{
    qDebug() << "destroy GLArea";

    delete m_timer;

    // Contrairement aux méthodes virtuelles initializeGL, resizeGL et repaintGL,
    // dans le destructeur le contexte GL n'est pas automatiquement rendu courant.
    makeCurrent();

    // ici destructions de ressources GL

    doneCurrent();
}

void GLArea::initializeGL()
{
    qDebug() << __FUNCTION__ ;
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
}

void GLArea::doProjection()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLfloat hr = m_radius, wr = hr * m_ratio;
    glFrustum(-wr, wr, -hr, hr, 1.0, 5.0);
    //glOrtho(-wr,wr,-hr,hr,1.0,5.0);

    glMatrixMode(GL_MODELVIEW);
    //gluLookAt();
}

void GLArea::resizeGL(int w, int h)
{
    qDebug() << __FUNCTION__ << w << h;

    // C'est fait par défaut
    glViewport(0, 0, w, h);

    m_ratio = (double) w / h;
    doProjection();
}
GLdouble GLArea::eyeAngle = 0.0;
GLdouble GLArea::yAngle = 0.0;
void GLArea::paintGL()
{
    //qDebug() << __FUNCTION__ ;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    glScalef(0.3,0.3,0.3);
    GLdouble eyeX = 5.0 * sin(GLArea::eyeAngle), eyeY = 0.0, eyeZ = 5.0 * cos(GLArea::eyeAngle);
    GLdouble centerX = 0.0, centerY = 0.0, centerZ = 0.0;
    GLdouble upX = 0.0, upY = 1.0, upZ = 0.0;
    gluLookAt (eyeX,eyeY,eyeZ,centerX,centerY,centerZ,upX,upY,upZ);
    GLArea::dessiner_cylindre_ortho(0.0,0.0,0,0.0,0.0,0.7,1.0,1.0);//G
    GLArea::dessiner_cylindre_ortho(0.0,0.0,-0.49,0.0,0.5,0.5,2.0,0.15);//O

    alpha = m_angle*PI/180;
    gh = 0.8;
    x_H = 0.0+gh*cos(m_angle*PI/180);
    y_H = 0.0+gh*sin((m_angle*PI/180));
    GLArea::dessiner_cylindre_ortho(x_H,y_H,0.5,0.2,0.8,0.2,0.3,0.2);//H
    GLArea::dessiner_cylindre_ortho(x_H,y_H,0.4,0.0,0.9,0.0,0.7,0.1);//H'

    jh = 1.2;
    x_J = x_H-sqrt(pow(jh,2)-pow(gh*sin((m_angle*PI/180)),2));
    y_J = 0.0;
    GLArea::dessiner_cylindre_ortho(x_J,0,0.5,0.2,0.8,0.2,0.3,0.2);//J
    GLArea::dessiner_cylindre_ortho(x_J,0,0.45,1.0,1.0,0.1,0.5,0.1);//J'
    GLArea::dessiner_cylindre_ortho(x_J,0,0.6,1.0,0.1,0.9,0.5,0.18);//J''

    x_K = -2.5;
    glPushMatrix();
    glRotatef(90,0,1,0);
    GLArea::dessiner_cylindre_ortho(-0.7,0,x_K,0.2,0.7,0.8,0.4,0.2);//K
    GLArea::dessiner_cylindre_ortho(-0.7,0,(x_K+x_J)*0.5,0.76,0.78,0.0,std::abs(x_K-x_J),0.1);//K
    glPopMatrix();


    glPushMatrix();
    glTranslatef((x_H+x_J)*0.5,(y_H+y_J)*0.5,0.5);
    glRotatef((std::atan(y_H/(std::abs(x_J-x_H)))*180/PI),0,0,1);
    glRotatef(90,0,1,0);
    glTranslatef(-(x_H+x_J)*0.5,-(y_H+y_J)*0.5,-0.5);
    GLArea::dessiner_cylindre_ortho((x_H+x_J)*0.5,(y_H+y_J)*0.5,0.5,0.3,0.7,0.3,1.5,0.1);

    glPopMatrix();
    //glutSwapBuffers();
    update();
}

void GLArea::keyPressEvent(QKeyEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->text();

    switch(ev->key()) {
        case Qt::Key_Space :
            m_angle -= 1;
            if (m_angle <= -360) m_angle = 0;
            update();
            break;
        case Qt::Key_A :
            if (m_timer->isActive())
                m_timer->stop();
            else m_timer->start();
            break;
        case Qt::Key_R :
            if (ev->text() == "r")
                 setRadius(m_radius-0.05);
            else setRadius(m_radius+0.05);
            break;
        case Qt::Key_1:
            eyeAngle +=0.1;
            break;
        case Qt::Key_2:
            eyeAngle -=0.1;
            break;
    }
}

void GLArea::keyReleaseEvent(QKeyEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->text();
}

void GLArea::mousePressEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->position().x() << ev->position().y() << 
        ev->button();
}

void GLArea::mouseReleaseEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->position().x() << ev->position().y() << 
        ev->button();
}

void GLArea::mouseMoveEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->position().x() << ev->position().y();
}

void GLArea::onTimeout()
{
    qDebug() << __FUNCTION__ ;
    m_alpha += 0.01;
    if (m_alpha > 1) m_alpha = 0;
    update();
}

void GLArea::setRadius(double radius)
{
    qDebug() << __FUNCTION__ << radius << sender();
    if (radius != m_radius && radius > 0.01 && radius <= 10) {
        m_radius = radius;
        qDebug() << "  emit radiusChanged()";
        emit radiusChanged(radius);
        makeCurrent();
        doProjection();
        doneCurrent();
        update();
    }
}

void GLArea::dessiner_cylindre_ortho(GLfloat xOffset,GLfloat yOffset,GLfloat zOffset,GLfloat coul_r,GLfloat coul_v,GLfloat coul_b,GLfloat ep_cyl, GLfloat r_cyl){
    double new_angle_cyl = 0;

    while(new_angle_cyl<=360){
        glPushMatrix();
        //glScalef(0.3,0.3,0.3);
        glTranslatef(xOffset,yOffset,zOffset);
        glRotatef(m_angle,0,0,1);
        glRotatef(new_angle_cyl + angle_cyl,0,0,1);
        //upside
        glBegin(GL_POLYGON);
        glColor3f (coul_r* 0.5, coul_v*0.5, coul_b*0.5);

        glVertex3f(0,0,ep_cyl/2);//A
        glVertex3f(r_cyl,0,ep_cyl/2);//C
        glVertex3f(r_cyl*cos(angle_cyl*PI/180),r_cyl*sin(angle_cyl*PI/180),ep_cyl/2);//F

        glEnd();
        //downside
        glBegin(GL_POLYGON);
        glColor3f (coul_r* 0.5, coul_v*0.5, coul_b*0.5);

        glVertex3f(0,0,-ep_cyl/2);//B
        glVertex3f(r_cyl,0,-ep_cyl/2);//D
        glVertex3f(r_cyl*cos(angle_cyl*PI/180),r_cyl*sin(angle_cyl*PI/180),-ep_cyl/2);//E

        glEnd();
        //facettes
        glBegin(GL_QUADS);
        glColor3f (coul_r* 0.8, coul_v*0.8, coul_b*0.8);
        glVertex3f(r_cyl,0,ep_cyl/2);//C
        glVertex3f(r_cyl*cos(angle_cyl*PI/180),r_cyl*sin(angle_cyl*PI/180),ep_cyl/2);//F
        glVertex3f(r_cyl*cos(angle_cyl*PI/180),r_cyl*sin(angle_cyl*PI/180),-ep_cyl/2);//E
        glVertex3f(r_cyl,0,-ep_cyl/2);//D
        glEnd();

        glPopMatrix();
        new_angle_cyl+= angle_cyl;
    }

}
void GLArea::dessiner_cylindre_laterale(GLfloat xOffset,GLfloat yOffset,GLfloat zOffset,GLfloat coul_r,GLfloat coul_v,GLfloat coul_b,GLfloat ep_cyl, GLfloat r_cyl){
    double new_angle_cyl = 0;

    while(new_angle_cyl<=360){
        glPushMatrix();
        glScalef(0.3,0.3,0.3);
        glRotatef(90,1,0,0);
        glTranslatef(xOffset,yOffset,zOffset);
        glRotatef(m_angle,0,0,1);
        glRotatef(new_angle_cyl + angle_cyl,0,0,1);
        //upside
        glBegin(GL_POLYGON);
        glColor3f (coul_r* 0.5, coul_v*0.5, coul_b*0.5);

        glVertex3f(0,0,ep_cyl/2);//A
        glVertex3f(r_cyl,0,ep_cyl/2);//C
        glVertex3f(r_cyl*cos(angle_cyl*PI/180),r_cyl*sin(angle_cyl*PI/180),ep_cyl/2);//F

        glEnd();
        //downside
        glBegin(GL_POLYGON);
        glColor3f (coul_r* 0.5, coul_v*0.5, coul_b*0.5);

        glVertex3f(0,0,-ep_cyl/2);//B
        glVertex3f(r_cyl,0,-ep_cyl/2);//D
        glVertex3f(r_cyl*cos(angle_cyl*PI/180),r_cyl*sin(angle_cyl*PI/180),-ep_cyl/2);//E

        glEnd();
        //facettes
        glBegin(GL_QUADS);
        glColor3f (coul_r* 0.8, coul_v*0.8, coul_b*0.8);
        glVertex3f(r_cyl,0,ep_cyl/2);//C
        glVertex3f(r_cyl*cos(angle_cyl*PI/180),r_cyl*sin(angle_cyl*PI/180),ep_cyl/2);//F
        glVertex3f(r_cyl*cos(angle_cyl*PI/180),r_cyl*sin(angle_cyl*PI/180),-ep_cyl/2);//E
        glVertex3f(r_cyl,0,-ep_cyl/2);//D
        glEnd();

        glPopMatrix();
        new_angle_cyl+= angle_cyl;
    }
}


