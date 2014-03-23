// Authors:
// James Walker   jwwalker at mtu dot edu
// Scott A. Kuhl  kuhl at mtu dot edu

#include <GL/glut.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <string>
#include <typeinfo>
#include "DGR_framework.h"


#ifdef DGR_MASTER
#else // if SLAVE:
int framesPassed = 0;



// command-line parameters
double frustum_left,frustum_right,frustum_bottom,frustum_top;
int screen_width,screen_height;
#endif

DGR_framework * myDGR;

float data1 = 0.0f;
float data2 = 0.0f;
float data3 = 0.0f;
float data4 = 0.0f;

void init(void) 
{
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_FLAT);
}

// OpenGL display function.
// Your graphics rendering code will go here.
// If you do any state updating in your display function, be sure to do it for the MASTER ONLY.
void display(void) 
{

#ifdef DGR_MASTER   // All code that updates state variables should be exclusive to the MASTER.
                    // Forbidding the SLAVES from updating state variables and only getting them
                    // from the MASTER is what guarantees that the processes all stay synchronized.
    data1 += 1.0f;
    data2 += 2.0f;
    data3 += 5.0f;
    data4 += 0.01f;

#else  // The slave automatically shuts itself off if it hasn't received
       // any packets within a few seconds (it gives itself longer if it
       // hasn't received any packets at all yet)
       // Assumes a 60fps framerate
    framesPassed++;
    if (myDGR->recvPack) 
    {
        if (framesPassed > 180) exit(EXIT_SUCCESS);
        framesPassed = 0.0;
    } 
    else 
    {
        if (framesPassed > 900) exit(EXIT_SUCCESS); // If your program takes a very long time to initialize,
                                                // you can increase this value so the slaves don't prematurely
                                                // shut themselves off.
    }

#endif

      // Display code common to both the MASTER and SLAVE (except the frustum call)
      // This simple example just displays a wireframe cube and slowly rotates it.
    glClear (GL_COLOR_BUFFER_BIT);
    glColor3f (1.0, 1.0, 1.0);
    glLoadIdentity ();
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    float x = 0;
    float y = 0;
    float z = 1.5;

#ifdef DGR_MASTER
    glFrustum (-1.03*3-x, 1.03*3-x, .28-z, 2.6-z, 3.9-y, 5000); // edit the 0.1,5000 if you want to change the near/far clipping distance
#else
    glFrustum (frustum_left-x, frustum_right-x, frustum_bottom-z, frustum_top-z, 3.9-y, 5000); // edit the 0.1,5000 if you want to change the near/far clipping distance
#endif
    glMatrixMode (GL_MODELVIEW);
    gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glTranslatef(0,0,-30);
    glScalef (8.0, 8.0, 8.0);
    glPushMatrix();
    glColor3ub(data2,data3,0);
    glRotatef(data1, 0.0f, 1.0f, 0.0f);
    glutWireCube (1.0);
    glPopMatrix();
    glColor3ub(data2,data3,0);
    glRotatef(data1, 0.0f, 1.0f, 0.0f);
    glutWireCube(data4);


    glutSwapBuffers();
    glutPostRedisplay();
}


// MAIN FUNCTION
int main(int argc, char** argv) 
{

#ifdef DGR_MASTER
    if (argc != 2) 
    {    
        printf("USAGE: %s relay-ip-address\n", argv[0]);
        return 1;
    }

    myDGR = new DGR_framework(argv[1]);

#else // if SLAVE:
    frustum_left = atof(argv[1]);
    frustum_right = atof(argv[2]);
    frustum_bottom = atof(argv[3]);
    frustum_top = atof(argv[4]);
    screen_width = atoi(argv[5]);
    screen_height = atoi(argv[6]);

    myDGR = new DGR_framework();
#endif
    //register variables
    myDGR->addNode<float>("data1",&data1);
    myDGR->addNode<float>("data2",&data2);
    myDGR->addNode<float>("data3",&data3);
    myDGR->addNode<float>("data4",&data4);



    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);

#ifdef DGR_MASTER
    glutInitWindowSize((1920*6)/8, (1080*4)/8);
    glutInitWindowPosition(0, 0);
    glutCreateWindow(argv[0]);
        // This simple example doesn't use input callbacks, but the lines commented out
        // below demonstrate that any input callbacks you use should be exclusive to the
        // MASTER and not used by the SLAVES.
        //glutKeyboardFunc(keyboard);
        //glutMouseFunc(processMouse);
        //glutMotionFunc(mousePressMove);
        //glutPassiveMotionFunc(mouseMove);
#else
    glutInitWindowSize (screen_width, screen_height); 
    glutInitWindowPosition (0, 0);
    glutCreateWindow ("DGR Slave Node");
#endif

    init();
    glutDisplayFunc(display);
      //glutReshapeFunc(reshape);
      //glutIdleFunc(animate);

      // go

    glutMainLoop();
    exit(EXIT_SUCCESS);
}
