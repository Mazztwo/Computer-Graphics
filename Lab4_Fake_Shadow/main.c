/*
 Alessio Mazzone
 Lab4 Fake Shadows
 
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
#include <time.h>
#include <stdlib.h>

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

const float DegreesToRadians = M_PI / 180.0; // M_PI = 3.14159...


// Create 3D object verticies
Vec4 vertices[1176];

int v_index = 0;

Vec4 cube_vertices[36] =
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


void initSphereVertices()
{
    for (float phi = -90.0; phi <= 90.0; phi += 20.0)
    {
        float phir = phi * DegreesToRadians;
        float phir20 = (phi + 20.0)*DegreesToRadians;
        
        for(float theta = -180.0; theta <= 180.0; theta += 20.0)
        {
            float thetar = theta*DegreesToRadians;
            float thetar20 = (theta+20.0)*DegreesToRadians;
              
            vertices[v_index].x = sin(thetar)*cos(phir);
            vertices[v_index].y = cos(thetar)*cos(phir);
            vertices[v_index].z = sin(phir);
            vertices[v_index].w = 1.0;
            v_index++;
            
            vertices[v_index].x = sin(thetar)*cos(phir20);
            vertices[v_index].y = cos(thetar)*cos(phir20);
            vertices[v_index].z = sin(phir20);
            vertices[v_index].w = 1.0;
            v_index++;
            
            vertices[v_index].x = sin(thetar20)*cos(phir20);
            vertices[v_index].y = cos(thetar20)*cos(phir20);
            vertices[v_index].z = sin(phir20);
            vertices[v_index].w = 1.0;
            v_index++;
            
            
            vertices[v_index].x = sin(thetar20)*cos(phir20);
            vertices[v_index].y = cos(thetar20)*cos(phir20);
            vertices[v_index].z = sin(phir20);
            vertices[v_index].w = 1.0;
            v_index++;
            
            vertices[v_index].x = sin(thetar20)*cos(phir);
            vertices[v_index].y = cos(thetar20)*cos(phir);
            vertices[v_index].z = sin(phir);
            vertices[v_index].w = 1.0;
            v_index++;
            
            vertices[v_index].x = sin(thetar)*cos(phir);
            vertices[v_index].y = cos(thetar)*cos(phir);
            vertices[v_index].z = sin(phir);
            vertices[v_index].w = 1.0;
            v_index++;
        }
    }
}

void initCubeVertices()
{
    
}

// Color each face of object
Vec4 colors[1140];

void initColors()
{
    srand ( time(NULL) );
    
    for(int i = 0; i < 1140; i++)
    {
        colors[i].x = rand() / (float)RAND_MAX;
        colors[i].y = rand() / (float)RAND_MAX;
        colors[i].z = rand() / (float)RAND_MAX;
        colors[i].w = 1.0;
    }
}


// Declare number of verticies
int num_vertices = 1140;


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
    
    glUniformMatrix4fv(ctm_location, 1, GL_FALSE, (GLfloat *) &scaling_matrix);
    
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_LINE);
    glDrawArrays(GL_TRIANGLES, 0, num_vertices);
    
    glutSwapBuffers();
}


void idle(void)
{
    glutPostRedisplay();
    
}



int main(int argc, char **argv)
{
    initSphereVertices();
    initColors();
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Lab3 Sphere");
    //glewInit();
    init();
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutMainLoop();
    
    return 0;
}
