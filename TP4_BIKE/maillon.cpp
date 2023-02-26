#include "maillon.h"

Maillon::Maillon(GLfloat ep_m, GLfloat r_cyl, GLfloat width, GLfloat height, QVector3D color, bool external):
    ep_m(ep_m), r_cyl(r_cyl), width(width), height(height),color_maillon(color)
{
    m_external = external;
    QVector3D p_color(color_maillon.x()*1.0, color_maillon.y()*0.6, color_maillon.z()*0.6);
    if(m_external){
        m_cylindre1 = new Cylinder(20, r_cyl, ep_m*1.2, p_color, true);
        m_cylindre2 = new Cylinder(20, r_cyl, ep_m*1.2, p_color, true);
        m_pedale1 = new Pedale(width*3, width*0.2, ep_m, color_maillon, 0.8);
        m_pedale2 = new Pedale(width*3, width*0.2, ep_m, color_maillon, 0.8);
    }else{
        m_pedale1 = new Pedale(width*1.5*0.5, width*0.2, ep_m*0.7, color_maillon, 0.8);
        m_pedale2 = new Pedale(width*1.5*0.5, width*0.2, ep_m*0.7, color_maillon, 0.8);
        m_pedale3 = new Pedale(width*1.5*0.5, width*0.2, ep_m*0.7, color_maillon, 0.8);
        m_pedale4 = new Pedale(width*1.5*0.5, width*0.2, ep_m*0.7, color_maillon, 0.8);
    }
}
Maillon::~Maillon(){

}

Cylinder* Maillon::getCylindre(int index){
    return (index==1)? m_cylindre1:m_cylindre2;
}

Pedale* Maillon::getPedale(int index){
    if(index == 1) return m_pedale1;
    else if(index == 2) return m_pedale2;
    else if(index == 3) return m_pedale3;
    else if(index == 4) return m_pedale4;
    else return 0;
}
