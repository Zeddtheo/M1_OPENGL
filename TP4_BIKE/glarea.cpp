// CC-BY Edouard.Thiel@univ-amu.fr - 31/01/2022

#include "glarea.h"
#include <cmath>
#include <QDebug>
#include <QSurfaceFormat>
#include <QMatrix4x4>
#include <QVector3D>
#include "roue.h"
const char *vertexShaderFile   = ":/vertex.glsl";
const char *fragmentShaderFile = ":/fragment.glsl";


GLArea::GLArea(QWidget *parent) :
    QOpenGLWidget(parent)
{
    qDebug() << "init GLArea" ;

    QSurfaceFormat sf;
    sf.setDepthBufferSize(24);
    sf.setSamples(16);  // suréchantillonnage pour l'antialiasing
    setFormat(sf);
    qDebug() << "Depth is"<< format().depthBufferSize();

    setEnabled(true);  // événements clavier et souris
    setFocusPolicy(Qt::StrongFocus); // accepte focus
    setFocus();                      // donne le focus

    connect (this, SIGNAL(radiusChanged(double)), this, SLOT(setRadius(double)));

    m_timer = new QTimer(this);
    m_timer->setInterval(50);  // msec
    connect (m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    m_timer->start();
}


GLArea::~GLArea()
{
    qDebug() << "destroy GLArea";

    delete m_timer;

    makeCurrent();
    tearGLObjects();
    doneCurrent();
}


void GLArea::initializeGL()
{
    qDebug() << __FUNCTION__ ;

    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    makeGLObjects();

    // shaders
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderFile);  // compile
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderFile);
    if (! m_program->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << m_program->log();
    }
}


void GLArea::makeGLObjects()
{
    GLfloat nb_dents = 10;
    GLfloat r_roue = 0.5;
    GLfloat h_dent = 0.15;
    GLfloat ep_roue = 0.2;
    QVector3D color_pignon = {0.0,1.0,0.0};
    QVector3D color_plateau = {0.0,1.0,0.5};
    roue_pignon = new Roue(nb_dents, ep_roue, h_dent, r_roue, color_pignon);
    roue_plateau = new Roue(nb_dents*2, ep_roue, h_dent, r_roue*2, color_plateau);

    GLfloat nb_fac = 20;
    GLfloat r_cyl = 0.05;
    GLfloat ep_cyl = 0.5;
    QVector3D color_axes = {1.0, 0.0, 0.0};
    axes_pignon = new Cylinder(nb_fac, roue_pignon->radius()/3, ep_cyl,color_axes, true);
    axes_plateau = new Cylinder(nb_fac, roue_plateau->radius()/5, ep_cyl,color_axes, true);

    GLfloat p_length = 0.4;
    GLfloat p_width = 0.3;
    GLfloat p_height = 0.2;
    QVector3D color_pedale = {0.2, 0.5, 0.2};
    GLfloat p_ratio = 0.7;
    L_pedale = new Pedale(p_length, p_width, p_height, color_pedale, p_ratio);
    R_pedale = new Pedale(p_length, p_width, p_height, color_pedale, p_ratio);

    GLfloat ep_m = 0.2;
    GLfloat rm_cyl = 0.08;
    GLfloat width = rm_cyl*2;
    GLfloat height = 0.15;

    QVector3D color_maillon = {0.2, 0.2, 0.8};
    maillon_pignon_externe = new Maillon(ep_m, rm_cyl, width, height, color_maillon, true);
    maillon_pignon_interne = new Maillon(ep_m, rm_cyl, width, height, color_maillon, false);
    maillon_plateau_externe = new Maillon(ep_m, rm_cyl, width, height, color_maillon, true);
    maillon_plateau_interne = new Maillon(ep_m, rm_cyl, width, height, color_maillon, false);

    QVector3D color_axe_pedale = {0.4, 0.5, 0.4};
    L_axe_pedale = new Cylinder(20,0.15, ep_cyl*0.7,color_axe_pedale, true);
    R_axe_pedale = new Cylinder(20,0.15, ep_cyl*0.7,color_axe_pedale, true);
}
void GLArea::tearGLObjects()
{
    delete roue_pignon;
    delete roue_plateau;
    delete axes_pignon;
    delete axes_plateau;
    delete L_pedale;
    delete R_pedale;
    delete maillon_pignon_externe;
    delete maillon_pignon_interne;
    delete maillon_plateau_externe;
    delete maillon_plateau_interne;
    delete L_axe_pedale;
    delete R_axe_pedale;
}


void GLArea::resizeGL(int w, int h)
{
    qDebug() << __FUNCTION__ << w << h;

    // C'est fait par défaut
    glViewport(0, 0, w, h);

    m_ratio = (double) w / h;
}


void GLArea::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    QOpenGLFunctions *glFuncs = context()->functions();

    m_program->bind();

    QMatrix4x4 proj_mat;
    GLfloat hr = m_radius, wr = hr * m_ratio;
    proj_mat.frustum(-wr, wr, -hr, hr, 0.5, 5.0);
    m_program->setUniformValue("projMatrix", proj_mat);

    QMatrix4x4 cam_mat;
    cam_mat.translate(0, 0,-3.0);
    QMatrix4x4 world_mat;
    world_mat.rotate(m_angle, 0, 1, 0);

    QMatrix4x4 shape_mat;

    GLfloat pignon_x = 0.0;
    GLfloat pignon_y = 0;
    GLfloat plateau_x = -3.0;
    GLfloat plateau_y = 0;

    //Dessin du pignon
    shape_mat = world_mat;
    shape_mat.translate(pignon_x, pignon_y, 0);
    shape_mat.rotate((m_anim+0.5)*360*2,0,0,1);
    setTransforms(cam_mat, shape_mat);
    roue_pignon->draw(m_program, glFuncs);
    //Dessin du plateau
    shape_mat = world_mat;
    shape_mat.translate(plateau_x, plateau_y, 0);
    shape_mat.rotate((m_anim+0.5)*180*2,0,0,1);
    setTransforms(cam_mat, shape_mat);
    roue_plateau->draw(m_program, glFuncs);
    //Dessin du axes de pignon
    shape_mat = world_mat;
    shape_mat.translate(pignon_x, pignon_y, 0);
    setTransforms(cam_mat, shape_mat);
    axes_pignon->draw(m_program, glFuncs);
    //Dessin du axes de plateau
    shape_mat = world_mat;
    shape_mat.translate(plateau_x, plateau_y, 0);
    setTransforms(cam_mat, shape_mat);
    axes_plateau->draw(m_program, glFuncs);
    //Dessin left pedale
    shape_mat = world_mat;
    shape_mat.translate(plateau_x, plateau_y, 1.0);
    setTransforms(cam_mat, shape_mat);
    L_pedale->draw(m_program, glFuncs);
    //Dessin Right pedale
    shape_mat = world_mat;
    shape_mat.translate(plateau_x, plateau_y, -1.0);
    setTransforms(cam_mat, shape_mat);
    R_pedale->draw(m_program, glFuncs);

    //Dessin Left axe pedale
    shape_mat = world_mat;
    shape_mat.translate(plateau_x, plateau_y, 1.0);
    setTransforms(cam_mat, shape_mat);
    L_axe_pedale->draw(m_program, glFuncs);
    //Dessin Right axe pedale
    shape_mat = world_mat;
    shape_mat.translate(plateau_x, plateau_y, -1.0);
    setTransforms(cam_mat, shape_mat);
    R_axe_pedale->draw(m_program, glFuncs);

    //Dessin maillon
    GLfloat d = abs(plateau_x - pignon_x);
    GLfloat x_D, y_D;
    GLfloat x_A, y_A;
    GLfloat x_B, y_B;
    GLfloat x_C, y_C;
    GLfloat r1 = roue_pignon->get_r_roue();
    GLfloat r2 = roue_plateau->get_r_roue();

    x_D = (r1*r1 - r2*r2) / (2 * d);;
    y_D = std::sqrt(r1*r1 - ((r1*r1-r2*r2)/(2*d))*((r1*r1-r2*r2)/(2*d)));
    qDebug()<<"x_D = "<<x_D<<Qt::endl;
    x_C = x_D;
    y_C = -y_D;

    x_A = -d - r2/r1*x_D;
    y_A = r2/r1*y_D;
    qDebug()<<"x_A = "<<x_A<<Qt::endl;
    x_B = x_A;
    y_B = -y_A;

    GLfloat centre_horizon;
    shape_mat = world_mat;
    GLfloat alpha = asin((r2-r1)/d);
    for(centre_horizon = x_D;centre_horizon>x_A;centre_horizon-=0.2){
        shape_mat = world_mat;
        shape_mat.translate(centre_horizon, (x_D-centre_horizon)*tan(alpha),0);
        shape_mat.translate(m_maillon_1 ,(x_D-m_maillon_1*tan(alpha)),0);
        shape_mat.translate(x_D, y_D+0.18);

        shape_mat.translate(0,0,roue_pignon->get_ep_roue()/2);
        setTransforms(cam_mat, shape_mat);
        maillon_pignon_externe->getPedale(1)->draw(m_program, glFuncs);

        shape_mat.translate(0,0,-roue_pignon->get_ep_roue());
        setTransforms(cam_mat, shape_mat);
        maillon_pignon_externe->getPedale(2)->draw(m_program, glFuncs);

        shape_mat.translate(-maillon_pignon_externe->getPedale(1)->get_width()*3.2, 0,roue_pignon->get_ep_roue()/2);
        setTransforms(cam_mat, shape_mat);
        maillon_pignon_externe->getCylindre(1)->draw(m_program, glFuncs);

        shape_mat.translate(maillon_pignon_externe->getPedale(1)->get_width()*6.4, 0, 0);
        setTransforms(cam_mat, shape_mat);
        maillon_pignon_externe->getCylindre(2)->draw(m_program, glFuncs);

        shape_mat.translate(maillon_pignon_externe->getPedale(1)->get_width()*3.2,0,roue_pignon->get_ep_roue()/2);
        setTransforms(cam_mat, shape_mat);
        maillon_pignon_interne->getPedale(1)->draw(m_program, glFuncs);

        shape_mat.translate(0,0,-roue_pignon->get_ep_roue());
        setTransforms(cam_mat, shape_mat);
        maillon_pignon_interne->getPedale(2)->draw(m_program, glFuncs);

        shape_mat.translate(-maillon_pignon_externe->getPedale(1)->get_width()*12.8,0,0);
        setTransforms(cam_mat, shape_mat);
        maillon_pignon_interne->getPedale(3)->draw(m_program, glFuncs);

        shape_mat.translate(0,0,roue_pignon->get_ep_roue());
        setTransforms(cam_mat, shape_mat);
        maillon_pignon_interne->getPedale(4)->draw(m_program, glFuncs);
    }

    //GLfloat centre_horizon;
    shape_mat = world_mat;
    for(centre_horizon= x_D;centre_horizon> x_A;centre_horizon-=0.2){
        shape_mat = world_mat;

        shape_mat.translate(centre_horizon ,-(x_D-centre_horizon*tan(alpha)),0);
        shape_mat.translate(m_maillon_2 ,-(x_D-m_maillon_2*tan(alpha)),0);
        shape_mat.translate(x_D, -y_D-0.3);

        shape_mat.translate(0,0,roue_pignon->get_ep_roue()/2);
        setTransforms(cam_mat, shape_mat);
        maillon_pignon_externe->getPedale(1)->draw(m_program, glFuncs);

        shape_mat.translate(0,0,-roue_pignon->get_ep_roue());
        setTransforms(cam_mat, shape_mat);
        maillon_pignon_externe->getPedale(2)->draw(m_program, glFuncs);

        shape_mat.translate(-maillon_pignon_externe->getPedale(1)->get_width()*3.2, 0,roue_pignon->get_ep_roue()/2);
        setTransforms(cam_mat, shape_mat);
        maillon_pignon_externe->getCylindre(1)->draw(m_program, glFuncs);

        shape_mat.translate(maillon_pignon_externe->getPedale(1)->get_width()*6.4, 0, 0);
        setTransforms(cam_mat, shape_mat);
        maillon_pignon_externe->getCylindre(2)->draw(m_program, glFuncs);

        shape_mat.translate(maillon_pignon_externe->getPedale(1)->get_width()*3.2,0,roue_pignon->get_ep_roue()/2);
        setTransforms(cam_mat, shape_mat);
        maillon_pignon_interne->getPedale(1)->draw(m_program, glFuncs);

        shape_mat.translate(0,0,-roue_pignon->get_ep_roue());
        setTransforms(cam_mat, shape_mat);
        maillon_pignon_interne->getPedale(2)->draw(m_program, glFuncs);

        shape_mat.translate(-maillon_pignon_externe->getPedale(1)->get_width()*12.8,0,0);
        setTransforms(cam_mat, shape_mat);
        maillon_pignon_interne->getPedale(3)->draw(m_program, glFuncs);

        shape_mat.translate(0,0,roue_pignon->get_ep_roue());
        setTransforms(cam_mat, shape_mat);
        maillon_pignon_interne->getPedale(4)->draw(m_program, glFuncs);
    }


    //pignon_maillon
    GLfloat pignon_rotate_angle = 0;
    GLfloat pignon_rotate_x;
    GLfloat pignon_rotate_y;
    shape_mat = world_mat;
    while(pignon_rotate_angle<360){
        shape_mat = world_mat;
        shape_mat.rotate((m_anim+0.5)*360*2,0,0,1);
        pignon_rotate_x = sin(pignon_rotate_angle*M_PI/180)*(roue_pignon->get_r_roue());
        pignon_rotate_y = cos(pignon_rotate_angle*M_PI/180)*(roue_pignon->get_r_roue());
        shape_mat.translate(pignon_rotate_x, pignon_rotate_y, 0);
        shape_mat.rotate(-pignon_rotate_angle,0,0,1);

        shape_mat.translate(0,0,roue_pignon->get_ep_roue()/2);
        setTransforms(cam_mat, shape_mat);
        maillon_pignon_externe->getPedale(1)->draw(m_program, glFuncs);

        shape_mat.translate(0,0,-roue_pignon->get_ep_roue());
        setTransforms(cam_mat, shape_mat);
        maillon_pignon_externe->getPedale(2)->draw(m_program, glFuncs);

        shape_mat.translate(-maillon_pignon_externe->getPedale(1)->get_width()*3.2, 0,roue_pignon->get_ep_roue()/2);
        qDebug()<<maillon_pignon_externe->getPedale(1)->get_width()<<Qt::endl;
        setTransforms(cam_mat, shape_mat);
        maillon_pignon_externe->getCylindre(1)->draw(m_program, glFuncs);

        shape_mat.translate(maillon_pignon_externe->getPedale(1)->get_width()*6.4, 0, 0);
        setTransforms(cam_mat, shape_mat);
        maillon_pignon_externe->getCylindre(2)->draw(m_program, glFuncs);

        shape_mat.translate(maillon_pignon_externe->getPedale(1)->get_width()*3.2,0,roue_pignon->get_ep_roue()/2);
        setTransforms(cam_mat, shape_mat);
        maillon_pignon_interne->getPedale(1)->draw(m_program, glFuncs);

        shape_mat.translate(0,0,-roue_pignon->get_ep_roue());
        setTransforms(cam_mat, shape_mat);
        maillon_pignon_interne->getPedale(2)->draw(m_program, glFuncs);

        shape_mat.translate(-maillon_pignon_externe->getPedale(1)->get_width()*12.8,0,0);
        setTransforms(cam_mat, shape_mat);
        maillon_pignon_interne->getPedale(3)->draw(m_program, glFuncs);

        shape_mat.translate(0,0,roue_pignon->get_ep_roue());
        setTransforms(cam_mat, shape_mat);
        maillon_pignon_interne->getPedale(4)->draw(m_program, glFuncs);

        pignon_rotate_angle+= 36;
    }
    //plateau_maillon

    GLfloat plateau_rotate_angle = 0;
    GLfloat plateau_rotate_x;
    GLfloat plateau_rotate_y;
    shape_mat = world_mat;
    while(plateau_rotate_angle<360){
        shape_mat = world_mat;
        shape_mat.translate(-3.0,0,0);
        shape_mat.rotate((m_anim+0.5)*180*2,0,0,1);
        plateau_rotate_x = sin(plateau_rotate_angle*M_PI/180)*(roue_plateau->get_r_roue());
        plateau_rotate_y = cos(plateau_rotate_angle*M_PI/180)*(roue_plateau->get_r_roue());

        shape_mat.translate(plateau_rotate_x, plateau_rotate_y, 0);
        shape_mat.rotate(-plateau_rotate_angle,0,0,1);

        shape_mat.translate(0,0,roue_plateau->get_ep_roue()/2);
        setTransforms(cam_mat, shape_mat);
        maillon_plateau_externe->getPedale(1)->draw(m_program, glFuncs);

        shape_mat.translate(0,0,-roue_plateau->get_ep_roue());
        setTransforms(cam_mat, shape_mat);
        maillon_plateau_externe->getPedale(2)->draw(m_program, glFuncs);

        shape_mat.translate(-maillon_plateau_externe->getPedale(1)->get_width()*3.2, 0,roue_plateau->get_ep_roue()/2);
        setTransforms(cam_mat, shape_mat);
        maillon_plateau_externe->getCylindre(1)->draw(m_program, glFuncs);

        shape_mat.translate(maillon_plateau_externe->getPedale(1)->get_width()*6.4, 0, 0);
        setTransforms(cam_mat, shape_mat);
        maillon_plateau_externe->getCylindre(2)->draw(m_program, glFuncs);

        shape_mat.translate(maillon_plateau_externe->getPedale(1)->get_width()*3.2,0,roue_plateau->get_ep_roue()/2);
        setTransforms(cam_mat, shape_mat);
        maillon_plateau_interne->getPedale(1)->draw(m_program, glFuncs);

        shape_mat.translate(0,0,-roue_plateau->get_ep_roue());
        setTransforms(cam_mat, shape_mat);
        maillon_plateau_interne->getPedale(2)->draw(m_program, glFuncs);

        shape_mat.translate(-maillon_plateau_interne->getPedale(1)->get_width()*12.8,0,0);
        setTransforms(cam_mat, shape_mat);
        maillon_plateau_interne->getPedale(3)->draw(m_program, glFuncs);

        shape_mat.translate(0,0,roue_plateau->get_ep_roue());
        setTransforms(cam_mat, shape_mat);
        maillon_plateau_interne->getPedale(4)->draw(m_program, glFuncs);

        plateau_rotate_angle+=24;
    }


    m_program->release();
}

void GLArea::setTransforms(QMatrix4x4 &cam_mat, QMatrix4x4 &shape_mat)
{
    QMatrix4x4 mv_mat = cam_mat*shape_mat;
    m_program->setUniformValue("mvMatrix", mv_mat);

    QMatrix3x3 nor_mat = shape_mat.normalMatrix();
    m_program->setUniformValue("norMatrix", nor_mat);
}

void GLArea::keyPressEvent(QKeyEvent *ev)
{
    //qDebug() << __FUNCTION__ << ev->text();

    switch(ev->key()) {
        case Qt::Key_Space :
            m_angle += 1;
            if (m_angle >= 360) m_angle -= 360;
            update();
            break;
        case Qt::Key_B:
            m_angle -= 1;
            if (m_angle <= 0) m_angle += 360;
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
    }
}

void GLArea::keyReleaseEvent(QKeyEvent *ev)
{
    (void) ev;
    //qDebug() << __FUNCTION__ << ev->text();
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
    //qDebug() << __FUNCTION__ ;
    m_anim += 0.01;
    if (m_anim > 1) m_anim = 0;
    update();

    m_maillon_1 -= 0.01;
    if(m_maillon_1 <= -0.2) m_maillon_1 = 0;
    update();


    m_maillon_2 += 0.01;
    if(m_maillon_2 >= 0.2) m_maillon_2 = 0;
    update();


}

void GLArea::setRadius(double radius)
{
    qDebug() << __FUNCTION__ << radius << sender();
    if (radius != m_radius && radius > 0.01 && radius <= 10) {
        m_radius = radius;
        qDebug() << "  emit radiusChanged()";
        emit radiusChanged(radius);
        update();
    }
}
