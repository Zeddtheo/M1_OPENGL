// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#ifndef GLAREA_H
#define GLAREA_H

#include <QKeyEvent>
#include <QTimer>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#define PI 3.141592

class GLArea : public QOpenGLWidget,
               protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GLArea(QWidget *parent = 0);
    ~GLArea();
    static GLdouble eyeAngle;
    static GLdouble yAngle;
public slots:
    void setRadius(double radius);

signals:  // On ne les implémente pas, elles seront générées par MOC ;
          // les paramètres seront passés aux slots connectés.
    void radiusChanged(double newRadius);

protected slots:
    void onTimeout();
    void dessiner_cylindre_ortho(GLfloat xOffset,GLfloat yOffset,GLfloat zOffset,GLfloat coul_r,GLfloat coul_v,GLfloat coul_b,GLfloat ep_cyl, GLfloat r_cyl);
    void dessiner_cylindre_laterale(GLfloat xOffset,GLfloat yOffset,GLfloat zOffset,GLfloat coul_r,GLfloat coul_v,GLfloat coul_b,GLfloat ep_cyl, GLfloat r_cyl);
protected:
    void initializeGL() override;
    void doProjection();
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent *ev) override;
    void keyReleaseEvent(QKeyEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
    void cam_init();
private:
    double m_angle = 0;
    QTimer *m_timer = nullptr;
    double m_anim = 0;
    double m_radius = 0.5;
    double m_ratio = 1;
    double m_alpha;

    int nb_fac = 20;
    GLfloat gh,jh;
    GLfloat angle_cyl = 360.0/nb_fac;

    GLfloat x_H,y_H,z_H;
    GLfloat x_K,y_K,z_K;
    GLfloat x_J,y_J,z_J;
    GLfloat ep_JK,ep_HJ;

    GLfloat alpha, beta;
};

#endif // GLAREA_H
