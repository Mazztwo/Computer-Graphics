/*
 * triangle.c
 *
 *  Created on: Aug 28, 2017
 *      Author: Thumrongsak Kosiyatrakul
 */


#ifdef __APPLE__  // include Mac OS X verions of headers

#include <OpenGL/OpenGL.h>
#include <OpenGL/gl3.h>
#include <GLUT/glut.h>

#else // non-Mac OS X operating systems

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#endif  // __APPLE__

#include "initShader.h"
#include <stdio.h>
#include "funct.h"
#include <math.h>

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))



// Create 3D object verticies
Vec4 vertices[36] =
{
    // Front Face 1
    {0.2, 0.2, 0.2, 1.0},              // top right
    {-0.2, 0.2, 0.2, 1.0},             // top left
    {-0.2, -0.2, 0.2, 1.0},            // bottom left
    
    {-0.2, -0.2, 0.2, 1.0},              // top right
    {0.2, -0.2, 0.2, 1.0},            // bottom left
    {0.2, 0.2, 0.2, 1.0},             // bottom right
    
    // Right Face 2
    {0.2, 0.2, -0.2, 1.0},             // top right
    {0.2, 0.2, 0.2, 1.0},              // top left
    {0.2, -0.2, 0.2, 1.0},             // bottom left
    
    {0.2, -0.2, 0.2, 1.0},             // top right
    {0.2, -0.2, -0.2, 1.0},             // bottom left
    {0.2, 0.2, -0.2, 1.0},            // bottom right
    
    // Back Face 3
    {0.2, 0.2, -0.2, 1.0},           // Bottom left
    {-0.2, 0.2, -0.2, 1.0},            // Top left
    {-0.2, -0.2, -0.2, 1.0},             // Top Right
    
    {-0.2, -0.2, -0.2, 1.0},              // Top Right
    {0.2, -0.2, -0.2, 1.0},            // Bottom Right
    {0.2, 0.2, -0.2, 1.0},             // Bottom Left
    
    // Left Face 4
    {-0.2, 0.2, -0.2, 1.0},              // top right
    {-0.2, 0.2, 0.2, 1.0},             // top left
    {-0.2, -0.2, 0.2, 1.0},            // bottom left
    
    {-0.2, -0.2, 0.2, 1.0},              // top right
    {-0.2, -0.2, -0.2, 1.0},            // bottom left
    {-0.2, 0.2, -0.2, 1.0},              // bottom right
    
    // Top Face 5
    {0.2, 0.2, -0.2, 1.0},              // top right
    {-0.2, 0.2, -0.2, 1.0},             // top left
    {-0.2, 0.2, 0.2, 1.0},            // bottom left
    
    {-0.2, 0.2, 0.2, 1.0},              // top right
    {0.2, 0.2, 0.2, 1.0},            // bottom left
    {0.2, 0.2, -0.2, 1.0},              // bottom right
    
    // Botto Face 6
    {0.2, -0.2, -0.2, 1.0},              // bottom left
    {-0.2, -0.2, -0.2, 1.0},             // top left
    {-0.2, -0.2, 0.2, 1.0},            // top right
    
    {-0.2, -0.2, 0.2, 1.0},              // top right
    {0.2, -0.2, 0.2, 1.0},            // bottom right
    {0.2, -0.2, -0.2, 1.0}              // bottom left
};


// Color each face of object
Vec4 colors[36] =
{
    {1.0, 0.0, 0.0, 1.0},    // red
    {1.0, 0.0, 0.0, 1.0},
    {1.0, 0.0, 0.0, 1.0},
    {1.0, 0.0, 0.0, 1.0},
    {1.0, 0.0, 0.0, 1.0},
    {1.0, 0.0, 0.0, 1.0},
    
    {0.0, 0.0, 1.0, 1.0},    // Blue
    {0.0, 0.0, 1.0, 1.0},
    {0.0, 0.0, 1.0, 1.0},
    {0.0, 0.0, 1.0, 1.0},
    {0.0, 0.0, 1.0, 1.0},
    {0.0, 0.0, 1.0, 1.0},
    
    {1.0, 0.0, 0.0, 1.0},    // red
    {1.0, 0.0, 0.0, 1.0},
    {1.0, 0.0, 0.0, 1.0},
    {1.0, 0.0, 0.0, 1.0},
    {1.0, 0.0, 0.0, 1.0},
    {1.0, 0.0, 0.0, 1.0},
    
    {0.0, 0.0, 1.0, 1.0},    // Blue
    {0.0, 0.0, 1.0, 1.0},
    {0.0, 0.0, 1.0, 1.0},
    {0.0, 0.0, 1.0, 1.0},
    {0.0, 0.0, 1.0, 1.0},
    {0.0, 0.0, 1.0, 1.0},
    
    {0.0, 1.0, 0.0, 1.0},    // Green
    {0.0, 1.0, 0.0, 1.0},
    {0.0, 1.0, 0.0, 1.0},
    {0.0, 1.0, 0.0, 1.0},
    {0.0, 1.0, 0.0, 1.0},
    {0.0, 1.0, 0.0, 1.0},
    
    {0.0, 1.0, 0.0, 1.0},    // Green
    {0.0, 1.0, 0.0, 1.0},
    {0.0, 1.0, 0.0, 1.0},
    {0.0, 1.0, 0.0, 1.0},
    {0.0, 1.0, 0.0, 1.0},
    {0.0, 1.0, 0.0, 1.0}
};
 
// Declare number of verticies
int num_vertices = 36;



// Declare point & vector pointing from initial mouse click to origin
Vec4 originVector = {0.0,0.0,0.0,0.0};

// Declare vector of motion starting from origin point
Vec4 motionVector = {0.0,0.0,0.0,0.0};

// Declare axis of rotation
Vec4 rotationAxis = {0.0,0.0,0.0,0.0};

GLuint ctm_location;

Mat4 tr_matrix =
{
    {1.0, 0.0, 0.0, 0.0},
    {0.0, 1.0, 0.0, 0.0},
    {0.0, 0.0, 1.0, 0.0},
    {0.0, 0.0, 0.0, 1.0}
    
};

int enableIdle = 0;
int leftDown = 1;

void init(void)
{
    GLuint program = initShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);
    
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);
    
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    
    GLuint vColor = glGetAttribLocation(program, "vColor");
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *) sizeof(vertices));
    
    ctm_location = glGetUniformLocation(program, "ctm");
    
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDepthRange(1,0);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &tr_matrix);
    
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);
    
    glutSwapBuffers();
}


void keyboard(unsigned char key, int mousex, int mousey)
{
    // Quit program
    if(key == 'q')
    {
        exit(0);
    }
    // Zoom In
    else if(key == 'o')
    {
        Mat4 tempMatrix = *scaleMatrix(&tr_matrix, 1.02, &tempMatrix);
        tr_matrix = tempMatrix;
        glutPostRedisplay();
    }
    // Zoom Out
    else if(key == 'l')
    {
        Mat4 tempMatrix = *scaleMatrix(&tr_matrix, 1.0/1.02, &tempMatrix);
        tr_matrix = tempMatrix;
        glutPostRedisplay();
    }
    else if(key == 'x')
    {
        Mat4 tempMatrix = *matRotateAboutX(&tr_matrix,15.0, &tempMatrix);
        tr_matrix = tempMatrix;
        glutPostRedisplay();
    }
    else if(key == 'X')
    {
        Mat4 tempMatrix = *matRotateAboutX(&tr_matrix,-15.0,&tempMatrix);
        tr_matrix = tempMatrix;
        glutPostRedisplay();
    }
    else if(key == 'y')
    {
        Mat4 tempMatrix = *matRotateAboutY(&tr_matrix,15.0, &tempMatrix);
        tr_matrix = tempMatrix;
        glutPostRedisplay();
    }
    else if(key == 'Y')
    {
        Mat4 tempMatrix = *matRotateAboutY(&tr_matrix,-15.0,&tempMatrix);
        tr_matrix = tempMatrix;
        glutPostRedisplay();
    }
    else if(key == 'z')
    {
        Mat4 tempMatrix = *matRotateAboutZ(&tr_matrix,15.0, &tempMatrix);
        tr_matrix = tempMatrix;
        glutPostRedisplay();
    }
    else if(key == 'Z')
    {
        Mat4 tempMatrix = *matRotateAboutZ(&tr_matrix,-15.0,&tempMatrix);
        tr_matrix = tempMatrix;
        glutPostRedisplay();
    }
    else if(key == ' ')
    {
        enableIdle = 1;
    }
    
    glutPostRedisplay();
    
}

void idle(void)
{
    if(enableIdle)
    {
        if(leftDown)
        {
            tr_matrix.col4.x -= 0.01;
            tr_matrix.col4.y -= 0.01;
            
            if(tr_matrix.col4.x < -0.5)
            leftDown = 0;
        }
        else
        {
            tr_matrix.col4.x += 0.01;
            tr_matrix.col4.y += 0.01;
            
            if(tr_matrix.col4.x > 0.5)
            leftDown = 1;
        }
        
        glutPostRedisplay();
    }
}

// Listener for mouse button events
void mouse(int button, int state, int x, int y)
{
    // Add scaling matrix to object
    // Scroll UP --> button = 3
    // Scroll DOWN --> button = 4
    
    // Scroll up to enlarge
    /*
    if(button == 3)
    {
         tr_matrix = *scaleMatrix(&tr_matrix, 1.02, &tr_matrix);
         glutPostRedisplay();
    }
    // Scroll down shrink
    else if (button == 4)
    {
        tr_matrix = *scaleMatrix(&tr_matrix, 1.0/1.02, &tr_matrix);
        glutPostRedisplay();
    }
    */
    
    // If button is pressed
    // button = GLUT LEFT BUTTON
    // state = GLUT_UP or GLUT_DOWN
    // The top-left corner of the screen is at (0, 0)
    // x y represent pointer position on screen
    if(button == GLUT_LEFT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
           
            // Can get initial click point to use for
            // calculation of axis of rotation

            originVector.x = x-256;
            originVector.y = 256-y;
            originVector.z = sqrt((256*256)-((x-256)*(x-256)));
            
        }
    }
    
    
    glutPostRedisplay();
}

void motion(int x, int y)
{
    // Capture moving x,y
    
    motionVector.x = x-256;
    motionVector.y = 256-y;
    motionVector.z = sqrt((256*256)-((x-256)*(x-256)));
    
    // Caclculate rotation axis and normalize it
    rotationAxis = *crossProduct(&originVector, &motionVector, &rotationAxis);
    float rotationAxisMagnitude = vecMagnitude(&rotationAxis);
    
    if(rotationAxisMagnitude != 0)
    {
        rotationAxis = *scalarMultVector(1/rotationAxisMagnitude, &rotationAxis, &rotationAxis);
    
        // Generate rotation matrix by calculating
        // Rx, Ry, Rz, being rotation about x,y,z axes.
        // Then, Rotation matrix R = R-xR-yRzRyRx
    
    
        float theta = angleBetweenVectors(&originVector, &motionVector);
        float d = sqrt((rotationAxis.y*rotationAxis.y) + (rotationAxis.z*rotationAxis.z));
        
        
        Mat4 rz =
        {
       
        };
    
        Mat4 ry =
        {
        
        };
    
        Mat4 rx =
        {
         
        };
        
        glutPostRedisplay();
        
    }
    
    glutPostRedisplay();
}



int main(int argc, char **argv)
{
    
    
    
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Project 1");
    glewInit();
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutIdleFunc(idle);
    glutMainLoop();
    
    return 0;
}
