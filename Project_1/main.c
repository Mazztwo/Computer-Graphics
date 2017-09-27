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

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

// Create 3D object verticies
Vec3 vertices[36] =
{
    // Front Face 1
    {0.2, 0.2, 0.2},              // top right
    {-0.2, 0.2, 0.2},             // top left
    {-0.2, -0.2, 0.2},            // bottom left
    {0.2, 0.2, 0.2},              // top right
    {-0.2, -0.2, 0.2},            // bottom left
    {0.2, -0.2, 0.2},              // bottom right
    
    // Right Face 2
    {0.2, 0.2, -0.2},             // top right
    {0.2, 0.2, 0.2},              // top left
    {0.2, -0.2, 0.2},             // bottom left
    {0.2, 0.2, -0.2},             // top right
    {0.2, -0.2, 0.2},             // bottom left
    {0.2, -0.2, -0.2},            // bottom right
    
    // Back Face 3
    {-0.2, -0.2, -0.2},           // Bottom left
    {-0.2, 0.2, -0.2},            // Top left
    {0.2, 0.2, -0.2},             // Top Right
    {0.2, 0.2, -0.2},              // Top Right
    {0.2, -0.2, -0.2},            // Bottom Right
    {-0.2, -0.2, 0.2},             // Bottom Left
    
    // Left Face 4
    {-0.2, 0.2, 0.2},              // top right
    {-0.2, 0.2, -0.2},             // top left
    {-0.2, -0.2, -0.2},            // bottom left
    {-0.2, 0.2, 0.2},              // top right
    {-0.2, -0.2, -0.2},            // bottom left
    {-0.2, -0.2, 0.2},              // bottom right
    
    // Top Face 5
    {0.2, 0.2, -0.2},              // top right
    {-0.2, 0.2, -0.2},             // top left
    {-0.2, 0.2, 0.2},            // bottom left
    {0.2, 0.2, -0.2,},              // top right
    {-0.2, 0.2, 0.2},            // bottom left
    {0.2, 0.2, 0.2},              // bottom right
    
    // Botto Face 6
    {-0.2, -0.2, 0.2},              // bottom left
    {-0.2, -0.2, -0.2},             // top left
    {0.2, -0.2, -0.2},            // top right
    {0.2, -0.2, -0.2},              // top right
    {0.2, -0.2, 0.2},            // bottom right
    {-0.2, -0.2, 0.2}              // bottom left
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
    
    {1.0, 0.0, 0.0, 1.0},    // Green
    {1.0, 0.0, 0.0, 1.0},
    {1.0, 0.0, 0.0, 1.0},
    {1.0, 0.0, 0.0, 1.0},
    {1.0, 0.0, 0.0, 1.0},
    {1.0, 0.0, 0.0, 1.0},
    
    {0.0, 1.0, 0.0, 1.0},    // Green
    {0.0, 1.0, 0.0, 1.0},
    {0.0, 1.0, 0.0, 1.0},
    {0.0, 1.0, 0.0, 1.0},
    {0.0, 1.0, 0.0, 1.0},
    {0.0, 1.0, 0.0, 1.0}
};

// Declare number of verticies
int num_vertices = 36;

// Declare initial x,y coordinates of cick
float initialX = 0.0;
float initialY = 0.0;

// Declare x and y variables that are
// being drawn by user rotating object
float currX = 0.0;
float currY = 0.0;


GLuint ctm_location;

Mat4 scaling_matrix =
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
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    
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
    
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &scaling_matrix);
    
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);
    
    glutSwapBuffers();
}


void keyboard(unsigned char key, int mousex, int mousey)
{
    if(key == 'q')
    exit(0);
    
    
    if(!enableIdle)
    {
        if(key == 'o')
        {
            scaling_matrix.col1.x *= 1.02;
            scaling_matrix.col2.y *= 1.02;
            scaling_matrix.col3.z *= 1.02;
            glutPostRedisplay();
        }
        else if(key == 'l')
        {
            scaling_matrix.col1.x *= 1/1.02;
            scaling_matrix.col2.y *= 1/1.02;
            scaling_matrix.col3.z *= 1/1.02;
            glutPostRedisplay();
        }
        else if(key == ' ')
        {
            scaling_matrix.col4.x = 0.5;
            scaling_matrix.col4.y = 0.5;
            scaling_matrix.col4.z = 0.0;
            enableIdle = 1;
        }
        
        glutPostRedisplay();
    }
    else
    {
        if(key == ' ')
        {
            scaling_matrix.col4.x = 0.0;
            scaling_matrix.col4.y = 0.0;
            scaling_matrix.col4.z = 0.0;
            enableIdle = 0;
        }
    }
    
}

void idle(void)
{
    if(enableIdle)
    {
        if(leftDown)
        {
            scaling_matrix.col4.x -= 0.01;
            scaling_matrix.col4.y -= 0.01;
            
            if(scaling_matrix.col4.x < -0.5)
            leftDown = 0;
        }
        else
        {
            scaling_matrix.col4.x += 0.01;
            scaling_matrix.col4.y += 0.01;
            
            if(scaling_matrix.col4.x > 0.5)
            leftDown = 1;
        }
        
        glutPostRedisplay();
    }
}



// Listner for mouse scroll up or down.
// Scroll up enlarges object, scroll down shrinks it.
void mouse(int button, int state, int x, int y)
{
    // Add scaling matrix to object
    // Scroll UP --> button = 3
    // Scroll DOWN --> button = 4
    
    // Scroll up enlarge
    /*
    printf("Button = %d\n",button);
    
    if(button == 3)
    {
        printf("Scrolled up!, Button = %d",button);
    }
    // Scroll down shrink
   // else if (button == 4)
    {
        printf("Scrolled down!, Button = %d",button);
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
            initialX = x;
            initialY = y;
            
            glutPostRedisplay();
        }
    }
    
    
    glutPostRedisplay();
}

void motion(int x, int y)
{
    
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
