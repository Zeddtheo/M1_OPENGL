// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#include "glarea.h"
#include "cylindre.h"
#include <QDebug>
#include <QSurfaceFormat>
#include <QMatrix4x4>
#include <QPushButton>
// Remarque : si on déclare une chaîne de cette façon,
//     const QString vertexShaderFile = QStringLiteral(":/basic.vsh");
// ça provoque un warning "non-POD static (QString)" de l'analyseur Clazy du compilateur
// (un POD (Plain Old Data) est un type simple ou un struct sans appel de constructeur).
// Pour éviter le warning, il vaut mieux la déclarer comme ceci :
//     const char *vertexShaderFile = ":/basic.vsh";

const char *vertexShaderFile   = ":/basic.vsh";
const char *fragmentShaderFile = ":/basic.fsh";


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

    // shaders
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderFile);  // compile
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderFile);
    if (! m_program->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << m_program->log();
    }

    // récupère identifiants de "variables" dans les shaders
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");
}

void GLArea::resizeGL(int w, int h)
{
    qDebug() << __FUNCTION__ << w << h;

    // C'est fait par défaut
    glViewport(0, 0, w, h);

    m_ratio = (double) w / h;
    // doProjection();
}

void GLArea::paintGL()
{
    qDebug() << __FUNCTION__ ;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program->bind();

    QMatrix4x4 matrix;
    GLfloat hr = m_radius, wr = hr * m_ratio;
    matrix.frustum(-wr, wr, -hr, hr, 1.0, 5.0);
    matrix.translate(0, 0, -4.0);
    matrix.rotate(m_angle, 1, 1, 0);
    //Let's go
    Cylindre cyl;
    //G
    QVector3D v1(0,0,0);
    QVector3D c1(0.0,0.2,0.8);
    cyl.stock_data(v1,c1,1.0,0.5);
    cyl.draw_cylindre(m_posAttr, m_colAttr ,m_program, matrix, m_matrixUniform,cyl_rotate);
    //O
    QVector3D v2(0,0,-0.1);
    QVector3D c2(0.0,0.5,0.5);
    cyl.stock_data(v2,c2,0.15,1.0);
    cyl.draw_cylindre(m_posAttr, m_colAttr ,m_program, matrix, m_matrixUniform,cyl_rotate);
    //H
    GLfloat gh = 0.8;
    GLfloat x_H = 0.0 + gh*cos(cyl_rotate*PI/180);
    GLfloat y_H = 0.0 + gh*sin(cyl_rotate*PI/180);
    QVector3D v3(x_H,y_H,0.25);
    QVector3D c3(0.2,0.8,0.2);
    cyl.stock_data(v3,c3,0.2,0.25);
    cyl.draw_cylindre(m_posAttr, m_colAttr ,m_program, matrix, m_matrixUniform,cyl_rotate);
    //H'
    QVector3D v4(x_H,y_H,0.5);
    QVector3D c4(0.2,0.9,0.3);
    cyl.stock_data(v4,c4,0.18,0.3);
    cyl.draw_cylindre(m_posAttr, m_colAttr ,m_program, matrix, m_matrixUniform,cyl_rotate);
    //J
    GLfloat jh = 1.2;
    GLfloat x_J = x_H - sqrt(pow(jh,2) - pow(gh*sin(cyl_rotate*PI/180),2));
    GLfloat y_J = 0.0;
    QVector3D v5(x_J,0,0.5);
    QVector3D c5(0.5,0.2,0.8);
    cyl.stock_data(v5,c5,0.2,0.3);
    cyl.draw_cylindre(m_posAttr, m_colAttr ,m_program, matrix, m_matrixUniform,cyl_rotate);
    //J'
    QVector3D v6(x_J,0,0.65);
    QVector3D c6(1.0,1.0,0.1);
    cyl.stock_data(v6,c6,0.1,0.5);
    cyl.draw_cylindre(m_posAttr, m_colAttr ,m_program, matrix, m_matrixUniform,cyl_rotate);
    //J''
    QVector3D v7(x_J,0,0.6);
    QVector3D c7(1.0,0.1,0.9);
    cyl.stock_data(v7,c7,0.18,0.5);
    cyl.draw_cylindre(m_posAttr, m_colAttr ,m_program, matrix, m_matrixUniform,cyl_rotate);
    //K
    GLfloat x_K = -2.5;
    QVector3D v8(-0.7,0,x_K);
    QVector3D c8(0.2,0.7,0.8);
    cyl.stock_data(v8,c8,0.2,0.4);
    cyl.draw_cylindre2(m_posAttr, m_colAttr ,m_program, matrix, m_matrixUniform,cyl_rotate);
    //KJ
    QVector3D v9(-0.7,0,(x_K+x_J)*0.5);
    QVector3D c9(0.5,0.8,0.1);
    cyl.stock_data(v9,c9,0.1,std::abs(x_K - x_J));
    cyl.draw_cylindre2(m_posAttr, m_colAttr ,m_program, matrix, m_matrixUniform,cyl_rotate);
    //JH
    QVector3D v10((x_H+x_J)*0.5,(y_H+y_J)*0.5,0.5);
    QVector3D c10(0.5,0.3,0.7);
    cyl.stock_data(v10,c10,0.1,1.5);
    cyl.draw_cylindre3(m_posAttr, m_colAttr ,m_program, matrix, m_matrixUniform,cyl_rotate, x_H, y_H,x_J,y_J);

    m_program->setUniformValue(m_matrixUniform, matrix);
    m_program->release();
}

void GLArea::keyPressEvent(QKeyEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->text();

    switch(ev->key()) {
        case Qt::Key_Space :
            m_angle += 1;
            if (m_angle >= 360) m_angle -= 360;
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
            update();
            break;
        case Qt::Key_C:
            cyl_rotate += 1;
            if (cyl_rotate >= 360) cyl_rotate -= 360;
            update();
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
    m_anim += 0.01;
    if (m_anim > 1) m_anim = 0;
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

double GLArea::getRadius()
{
    qDebug()<<m_radius<<Qt::endl;
    return m_radius;
}




