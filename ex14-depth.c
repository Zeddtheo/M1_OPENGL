#define _GNU_SOURCE
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>

#define PI 3.141592

int anim_angle_roue = 0;
unsigned int anim_delay = 50; // ms
int anim_flag = 0;
float aspect_ratio = 1.0;
int depth_flag = 1;
float h_dent = 0.25;
int nb_dents;
int flag_fill = 1;

GLfloat gear1_rotation = 0.0;
GLfloat gear2_rotation = 0.0;
GLfloat gear3_rotation = 0.0;
GLfloat gear4_rotation = 0.0;


GLfloat coul_r[3] = {1.0f,0.0f,0.0f};
GLfloat coul_v[3] = {0.0f,1.0f,0.0f};
GLfloat coul_b[3] = {0.0f,0.0f,1.0f}; 

float cam_z, cam_hr, cam_near, cam_far;
enum { P_ORTHO, P_FRUSTUM, P_MAX } cam_proj;


void cam_init ()
{
    cam_z = 3; cam_hr = 1; cam_near = 1; cam_far = 5; cam_proj = P_ORTHO;
}

void do_projection (void)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float wr = cam_hr * aspect_ratio;
    switch (cam_proj) {
        case P_ORTHO :
            printf ("glOrtho (%.1f, %.1f, %.1f, %.1f, %.1f, %.1f)  cam_z = %.1f, flag_fill = %d\n",
                -wr, wr, -cam_hr, cam_hr, cam_near, cam_far, cam_z,flag_fill);
            glOrtho (-wr, wr, -cam_hr, cam_hr, cam_near, cam_far);
            break;
        case P_FRUSTUM :
            printf ("Frustum (%.1f, %.1f, %.1f, %.1f, %.1f, %.1f)  cam_z = %.1f\n",
                -wr, wr, -cam_hr, cam_hr, cam_near, cam_far, cam_z);
            glFrustum (-wr, wr, -cam_hr, cam_hr, cam_near, cam_far);
            break;
        default :;
    }

    glMatrixMode(GL_MODELVIEW);
}

void on_reshape_win(int width, int height)
{
    printf ("Reshape %d %d\n", width, height);
    glViewport(0, 0, width, height);
    aspect_ratio = (float) width / height;

    do_projection();
}

void on_timer (int id)
{
    if (!anim_flag) return;
    anim_angle_roue += 1;
    if (anim_angle_roue >= 360) anim_angle_roue = 0;
    glutPostRedisplay();
    glutTimerFunc (anim_delay, on_timer, id);  // on relance le même timer
}

void change_valf (float *val, int negate, float incr, float min_val)
{
    *val += negate ? -incr : incr;
    if (*val <= min_val) *val = min_val;
}

void print_help ()
{
    printf ("h help  i init  a anim  p proj  zZ cam_z  rR radius  nN near  fF far  dD dist  b zbuffer y changecolor space rotate\n");
}

void dessiner_bloc_dent(int angle_roue, GLfloat r_trou, GLfloat r_roue,GLfloat color[3]){
    glBegin(GL_TRIANGLE_FAN);
    if(!flag_fill){
        glColor3f(color[0],color[1],color[2]);
    }else{
        glColor3f(1.0,1.0,1.0);
    }
    //A
    glVertex3f(r_trou, 0, 0); 
    //B
    glVertex3f(r_roue-h_dent/2, 0, 0); 
    //C
    glVertex3f(cos(angle_roue/4*PI/180)*(r_roue-h_dent/2),sin(angle_roue/4*PI/180)*(r_roue-h_dent/2),0);
    //D
    glVertex3f(cos(angle_roue/2*PI/180)*(r_roue+h_dent/2),sin(angle_roue/2*PI/180)*(r_roue+h_dent/2),0);
    //E
    glVertex3f(cos(angle_roue/4*3*PI/180)*(r_roue+h_dent/2),sin(angle_roue/4*3*PI/180)*(r_roue+h_dent/2),0);
    //F
    glVertex3f(cos(angle_roue*PI/180)*(r_roue-h_dent/2),sin(angle_roue*PI/180)*(r_roue-h_dent/2),0);
    //G
    glVertex3f(cos(angle_roue*PI/180)*r_trou, sin(angle_roue*PI/180)*r_trou,0);
    //H
    glVertex3f(cos(angle_roue/2*PI/180)*r_trou, sin(angle_roue/2*PI/180)*r_trou,0);
    glEnd();
}

void dessiner_cote_roue(int angle_roue, GLfloat r_trou, GLfloat r_roue,GLfloat xOffset, GLfloat yOffset, GLfloat zOffset,GLfloat angle_offset, int isClockwise, int isGear1, GLfloat color[3]){
    GLfloat new_roue_angle = 0;
    while (new_roue_angle <= 360)
    {
        glPushMatrix();
        glTranslatef(xOffset,yOffset,zOffset);
        if(isClockwise){
            if(isGear1){
                glRotatef(gear1_rotation,0,0,1);
            }else{
                glRotatef(gear3_rotation,0,0,1);
            }   
        }else{
            if(isGear1){
                glRotatef(gear2_rotation,0,0,1);    
            }else{
                glRotatef(gear4_rotation,0,0,1);
            }      
        }
        glRotatef(new_roue_angle + angle_offset,0,0,1);
        dessiner_bloc_dent(angle_roue,r_trou,r_roue, color);
        glPopMatrix();

        new_roue_angle+=angle_roue;
    }
}

void dessiner_roue(int angle_roue, GLfloat r_trou, GLfloat r_roue, GLfloat xOffset, GLfloat yOffset, GLfloat zOffset,  GLfloat angle_offset, GLfloat ep_roue, int isClockwise, int isGear1, GLfloat color[3]){
    glPushMatrix();
    glTranslatef(0,0,ep_roue/2.0);
    dessiner_cote_roue(angle_roue,r_trou,r_roue,xOffset,yOffset,0,angle_offset,isClockwise,isGear1,color);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0,-(ep_roue/2.0));
    dessiner_cote_roue(angle_roue,r_trou,r_roue, xOffset,yOffset,0,angle_offset,isClockwise,isGear1, color);
    glPopMatrix();
}

void dessiner_facettes_bloc(int angle_roue, GLfloat r_trou, GLfloat r_roue, GLfloat color[3],GLfloat ep_roue){
    //AHH'A'
    glBegin(GL_QUADS);
    if(flag_fill){
        glColor3f(1.0,1.0,1.0);
    }
    else{
        glColor3f(color[0]*0.6,color[1]*0.6,color[2]*0.6);
    }
    glVertex3f(r_trou, 0, ep_roue/2);
    glVertex3f(cos(angle_roue/2*PI/180)*r_trou, sin(angle_roue/2*PI/180)*r_trou,ep_roue/2);
    glVertex3f(cos(angle_roue/2*PI/180)*r_trou, sin(angle_roue/2*PI/180)*r_trou,-ep_roue/2);
    glVertex3f(r_trou, 0, -ep_roue/2);
    glEnd();
    
    //HGG'H'
    glBegin(GL_QUADS);
    if(flag_fill){
        glColor3f(1.0,1.0,1.0);
    }else{
        glColor3f(color[0]*0.3,color[1]*0.3,color[2]*0.3);
    }
    glVertex3f(cos(angle_roue/2*PI/180)*r_trou, sin(angle_roue/2*PI/180)*r_trou,ep_roue/2);
    glVertex3f(cos(angle_roue*PI/180)*r_trou, sin(angle_roue*PI/180)*r_trou,ep_roue/2);
    glVertex3f(cos(angle_roue*PI/180)*r_trou, sin(angle_roue*PI/180)*r_trou,-ep_roue/2);
    glVertex3f(cos(angle_roue/2*PI/180)*r_trou, sin(angle_roue/2*PI/180)*r_trou,-ep_roue/2);
    glEnd();

    //BCC'B'
    glBegin(GL_QUADS);
    if(flag_fill){
        glColor3f(1.0,1.0,1.0);
    }else{
        glColor3f(color[0],color[1],color[2]);
    }
    glVertex3f(r_roue-h_dent/2, 0, ep_roue/2);
    glVertex3f(cos(angle_roue/4*PI/180)*(r_roue-h_dent/2),sin(angle_roue/4*PI/180)*(r_roue-h_dent/2),ep_roue/2);
    glVertex3f(cos(angle_roue/4*PI/180)*(r_roue-h_dent/2),sin(angle_roue/4*PI/180)*(r_roue-h_dent/2),-ep_roue/2);
    glVertex3f(r_roue-h_dent/2, 0, -ep_roue/2);
    glEnd();
    //CDD'C'
    glBegin(GL_QUADS);
    if(flag_fill){
        glColor3f(1.0,1.0,1.0);
    }else{
        glColor3f(color[0],color[1],color[2]);
    }
    glVertex3f(cos(angle_roue/4*PI/180)*(r_roue-h_dent/2),sin(angle_roue/4*PI/180)*(r_roue-h_dent/2),ep_roue/2);
    glVertex3f(cos(angle_roue/2*PI/180)*(r_roue+h_dent/2),sin(angle_roue/2*PI/180)*(r_roue+h_dent/2),ep_roue/2);
    glVertex3f(cos(angle_roue/2*PI/180)*(r_roue+h_dent/2),sin(angle_roue/2*PI/180)*(r_roue+h_dent/2),-ep_roue/2);
    glVertex3f(cos(angle_roue/4*PI/180)*(r_roue-h_dent/2),sin(angle_roue/4*PI/180)*(r_roue-h_dent/2),-ep_roue/2);
    glEnd();
    //DEE'D'
    glBegin(GL_QUADS);
    if(flag_fill){
        glColor3f(1.0,1.0,1.0);
    }else{
        glColor3f(color[0]*0.5,color[1]*0.5,color[2]*0.5);
    }
    glVertex3f(cos(angle_roue/2*PI/180)*(r_roue+h_dent/2),sin(angle_roue/2*PI/180)*(r_roue+h_dent/2),ep_roue/2);
    glVertex3f(cos(angle_roue/4*3*PI/180)*(r_roue+h_dent/2),sin(angle_roue/4*3*PI/180)*(r_roue+h_dent/2),ep_roue/2);
    glVertex3f(cos(angle_roue/4*3*PI/180)*(r_roue+h_dent/2),sin(angle_roue/4*3*PI/180)*(r_roue+h_dent/2),-ep_roue/2);
    glVertex3f(cos(angle_roue/2*PI/180)*(r_roue+h_dent/2),sin(angle_roue/2*PI/180)*(r_roue+h_dent/2),-ep_roue/2);
    glEnd();
    //EFF'E'
    glBegin(GL_QUADS);
    if(flag_fill){
        glColor3f(1.0,1.0,1.0);
    }else{
        glColor3f(color[0],color[1],color[2]);
    }
    glVertex3f(cos(angle_roue/4*3*PI/180)*(r_roue+h_dent/2),sin(angle_roue/4*3*PI/180)*(r_roue+h_dent/2),ep_roue/2);
    glVertex3f(cos(angle_roue*PI/180)*(r_roue-h_dent/2),sin(angle_roue*PI/180)*(r_roue-h_dent/2),ep_roue/2);
    glVertex3f(cos(angle_roue*PI/180)*(r_roue-h_dent/2),sin(angle_roue*PI/180)*(r_roue-h_dent/2),-ep_roue/2);
    glVertex3f(cos(angle_roue/4*3*PI/180)*(r_roue+h_dent/2),sin(angle_roue/4*3*PI/180)*(r_roue+h_dent/2),-ep_roue/2);
    glEnd();
}

void dessiner_facette(int angle_roue, GLfloat r_trou, GLfloat r_roue, GLfloat xOffset, GLfloat yOffset, GLfloat zOffset,GLfloat angle_offset,GLfloat ep_roue,int isClockwise, int isGear1, GLfloat color[3]){
    int new_angle_roue = 0;
    while(new_angle_roue<=360){
        glPushMatrix();
        glTranslatef(xOffset,yOffset,zOffset);
        if(isClockwise){
            if(isGear1){
                glRotatef(gear1_rotation,0,0,1);
            }else{
                glRotatef(gear3_rotation,0,0,1);
            }   
        }else{
            if(isGear1){
                glRotatef(gear2_rotation,0,0,1);    
            }else{
                glRotatef(gear4_rotation,0,0,1);
            }      
        }
        glRotatef(new_angle_roue+angle_offset,0,0,1);
        dessiner_facettes_bloc(angle_roue, r_trou, r_roue, color,ep_roue);
        glPopMatrix();

        new_angle_roue+=36;
    }
}

void gear_rotate()
{
    gear1_rotation += 1.0;
    gear2_rotation -= 1.0;
    gear3_rotation += 0.5;
    gear4_rotation -= 0.5;
    glutPostRedisplay();
}

void on_key_pressed (unsigned char key, int x, int y)
{
    switch (key) {
        case 'i' :
            cam_init ();
            break;
        case 'a' :
            anim_flag = !anim_flag;
            if (anim_flag) glutTimerFunc (anim_delay, on_timer, 0);
            break;
        case 'p' : 
            cam_proj += 1;
            if (cam_proj >= P_MAX) cam_proj = 0;
            break;
        case 'z' : 
        case 'Z' :
            change_valf (&cam_z, key == 'z', 0.1, -100);
            break;
        case 'r' : 
        case 'R' :
            change_valf (&cam_hr, key == 'r', 0.1, 0.1);
            break;
        case 'n' : 
        case 'N' :
            change_valf (&cam_near, key == 'n', 0.1, 0.1);
            break;
        case 'f' : 
        case 'F' :
            change_valf (&cam_far, key == 'f', 0.1, 0.1);
            break;
        case 'd' : 
        case 'D' :
            change_valf (&cam_z, key == 'd', 0.1, -100);
            change_valf (&cam_near, key == 'd', 0.1, 0.1);
            change_valf (&cam_far, key == 'd', 0.1, 0.1);
            break;
        case 'b' :
            depth_flag = !depth_flag;
            printf ("depth_flag is %d\n", depth_flag);
            if (depth_flag) glEnable(GL_DEPTH_TEST);
            else glDisable(GL_DEPTH_TEST);
            break;
        case 'h' :
            print_help ();
            break;
        case 'y':
            flag_fill = !flag_fill;
            break;
        case ' ':
            gear_rotate();
            break;
        case 27 : // ESC
            exit(1);
    }

    do_projection();
    glutPostRedisplay();
}

void on_display_win(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt (0, 0, cam_z, 0, 0, 0, 0, 1, 0);
    
    glRotatef (anim_angle_roue, 0, 1.0, 0.1);
    glColor3f (1.0, 1.0, 1.0);
    glScalef(0.4,0.4,0.4);
    //Roue rouge
    dessiner_roue(36,0.5,1.0,-1.0,1.0,0,0.0,0.2f,1,1,coul_r);
    dessiner_facette(36,0.5,1.0,-1,1.0,0,0.0,0.2f,1,1,coul_r);
    //Roue vert
    dessiner_roue(36,0.5,1.0,1.0, 1.0, 0, 9.0f, 0.4f, 0,1,coul_v);
    dessiner_facette(36,0.5,1.0,1.0 ,1.0, 0, 9.0f, 0.4f, 0,1, coul_v);
    //Roue bleu
    dessiner_roue(18,0.4,2.0,-1.0, -2.0, 0, 14.0f, 0.1f, 0,0, coul_b);
    dessiner_facette(18,0.4,2.0,-1.0 ,-2.0, 0, 14.0f, 0.1f, 0,0, coul_b);

    glutSwapBuffers();
}

int main(int argc, char **argv) 
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    cam_init();
    print_help();

    glutCreateWindow("Z-buffer");
    glutReshapeFunc(on_reshape_win);
    glutDisplayFunc(on_display_win);
    glutKeyboardFunc(on_key_pressed);

    glEnable(GL_DEPTH_TEST); // Après glutCreateWindow qui crée un context GL

    glutMainLoop();
    return 0;
}

