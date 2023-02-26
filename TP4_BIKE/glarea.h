// CC-BY Edouard.Thiel@univ-amu.fr - 31/01/2023

#ifndef GLAREA_H
#define GLAREA_H

#include <QKeyEvent>
#include <QTimer>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include "maillon.h"
#include "roue.h"
#include "cylinder.h"
#include "pedale.h"
class GLArea : public QOpenGLWidget,
               protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GLArea(QWidget *parent = 0);
    ~GLArea();
    double getRadius() { return m_radius; }

public slots:
    void setRadius(double radius);

signals:  // On ne les implémente pas, elles seront générées par MOC ;
          // les paramètres seront passés aux slots connectés.
    void radiusChanged(double newRadius);

protected slots:
    void onTimeout();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent *ev) override;
    void keyReleaseEvent(QKeyEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;
private:
    double m_angle = 0;
    QTimer *m_timer = nullptr;
    double m_anim = 0;
    double m_maillon_1 =0;
    double m_maillon_2 =0;
    double m_radius = 0.5;
    double m_ratio = 1;

    QOpenGLShaderProgram *m_program;

    void makeGLObjects();
    void tearGLObjects();
    void setTransforms(QMatrix4x4 &cam_mat, QMatrix4x4 &shape_mat);

    Roue *roue_pignon = nullptr;
    Roue *roue_plateau = nullptr;

    Cylinder *axes_pignon = nullptr;
    Cylinder *axes_plateau = nullptr;

    Pedale *L_pedale = nullptr;
    Pedale *R_pedale = nullptr;

    Cylinder *L_axe_pedale = nullptr;
    Cylinder *R_axe_pedale = nullptr;

    Maillon *maillon_pignon_externe = nullptr;
    Maillon *maillon_pignon_interne = nullptr;
    Maillon *maillon_plateau_externe = nullptr;
    Maillon *maillon_plateau_interne = nullptr;
};

#endif // GLAREA_H
